// src/image_extract.cpp
#include "../inc/ros/image_extract.h"
#include "../inc/ros/bitstream.h"

#include <algorithm>
#include <cstring>
#include <cstdio>

namespace {
#pragma pack(push,1)
struct CcsdsPrimary {
    uint16_t vts_apid;  // ver(3) | type(1) | secHdr(1) | APID(11)
    uint16_t seq;       // seqFlags(2) | seqCount(14)
    uint16_t len;       // bytes_after_primary - 1
};
#pragma pack(pop)

inline uint16_t be16(const uint8_t* p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}

bool parse_one_ccsds(const uint8_t* base, size_t avail,
                     uint16_t& apid, uint8_t& seqFlags, uint16_t& seqCount,
                     const uint8_t*& user, size_t& userLen,
                     size_t& pktBytes, bool& hasSecHdr)
{
    if (avail < 6) return false;

    CcsdsPrimary ph {
        .vts_apid = be16(base + 0),
        .seq      = be16(base + 2),
        .len      = be16(base + 4),
    };

    apid      = ph.vts_apid & 0x07FF;
    hasSecHdr = (ph.vts_apid & 0x0800) != 0;

    seqFlags  = uint8_t((ph.seq >> 14) & 0x3); // 00:CONT 01:FIRST 10:LAST 11:STANDALONE
    seqCount  = uint16_t(ph.seq & 0x3FFF);

    size_t bytes_after_primary = size_t(ph.len) + 1;
    pktBytes = 6 + bytes_after_primary;
    if (avail < pktBytes) return false;

    user = base + 6;
    userLen = bytes_after_primary;
    return true;
}

// 휴리스틱: expedite preamble (예: 00 0F 47 7F)
inline bool looks_like_expedite(const uint8_t* p, size_t n) {
    static const uint8_t P[] = {0x00, 0x0F, 0x47, 0x7F};
    return (n >= sizeof(P) && std::memcmp(p, P, sizeof(P)) == 0);
}
} // namespace

bool assemble_image_from_acc(const std::vector<uint8_t>& acc,
                             const ImageAssemblyConfig& cfg,
                             Bitstream& out_bitstream,
                             ImageAssemblyReport* out_report,
                             const std::string& raw_src_path)
{
    if (out_report) {
        *out_report = ImageAssemblyReport{};
        out_report->expected_payload_len_per_pkt = cfg.has_fixed_payload ? cfg.fixed_payload_bytes : 0;
    }

    // 최종 조립 검증용 버퍼(비트스트림에도 동일 데이터가 기록됨)
    std::vector<uint8_t> assembled;
    assembled.reserve(32 * 1024);

    // 수집 상태
    bool started = false;
    bool saw_first = false, saw_cont = false, saw_last = false;
    std::vector<uint16_t> seq_seen;
    std::vector<size_t>   rx_bytes;

    bool expedite_present = false; // 세션 내에서 한 번이라도 보이면 true

    size_t off = 0;
    while (off + 6 <= acc.size()) {
        uint16_t apid = 0;
        uint8_t  seqFlags = 0;
        uint16_t seqCount = 0;
        const uint8_t* user = nullptr;
        size_t userLen = 0;
        size_t pktBytes = 0;
        bool   hasSecHdr = false;

        if (!parse_one_ccsds(acc.data() + off, acc.size() - off,
                             apid, seqFlags, seqCount, user, userLen, pktBytes, hasSecHdr)) {
            break; // 불완전 데이터 -> 루프 종료
        }

        if (apid == cfg.image_apid) {
            // Expedite 프리앰블 탐지(맨 앞 패킷에서 보통 등장하지만, 휴리스틱으로 첫 user에만 검사)
            if (!expedite_present && userLen >= 4 && looks_like_expedite(user, userLen)) {
                expedite_present = true;
            }

            // Secondary Header 제거(있다면)
            const uint8_t* payload = user;
            size_t payloadLen = userLen;
            if (hasSecHdr && cfg.sec_hdr_len > 0) {
                if (payloadLen >= cfg.sec_hdr_len) {
                    payload    += cfg.sec_hdr_len;
                    payloadLen -= cfg.sec_hdr_len;
                } else {
                    payloadLen = 0; // 비정상 → payload 없음 처리
                }
            }

            // FIRST: 새 파일 시작
            if (seqFlags == 0b01 && !started) {
                if (out_report) {
                out_report->first_seq_valid = true;
                out_report->first_seq = seqCount;   // ⬅️ 기록
                }
                if (out_bitstream.is_open()) out_bitstream.finish();
                out_bitstream.start_new(); // Bitstream 옵션은 외부에서 세팅했다고 가정
                started = true;
                saw_first = true;
            }

            if (seqFlags == 0b00) saw_cont = true;
            if (seqFlags == 0b10) {
                if (out_report) {
                out_report->last_seq_valid = true;
                out_report->last_seq = seqCount;
                }
                saw_last  = true;
            }

            // STANDALONE: 단일 패킷 이미지
            if (seqFlags == 0b11) {
                if (out_bitstream.is_open()) out_bitstream.finish();
                out_bitstream.start_new();
                if (payloadLen) {
                    out_bitstream.append(payload, payloadLen);
                    assembled.insert(assembled.end(), payload, payload + payloadLen);
                }
                out_bitstream.finish();

                // 시퀀스 기록도 남겨둔다
                seq_seen.push_back(seqCount);
                rx_bytes.push_back(payloadLen);

                off += pktBytes;
                continue;
            }

            // 일반(FIRST/CONT/LAST) 페이로드 기록
            if (started && payloadLen) {
                out_bitstream.append(payload, payloadLen);
                assembled.insert(assembled.end(), payload, payload + payloadLen);
            }

            // 시퀀스/수신길이 기록
            seq_seen.push_back(seqCount);
            rx_bytes.push_back(payloadLen);

            // LAST: 종료
            if (seqFlags == 0b10 && started) {
                out_bitstream.finish();
                started = false;
            }
        }

        off += pktBytes;
    }

    // 세션 끝났는데 파일 열린 채면 마무리
    if (out_bitstream.is_open()) {
        out_bitstream.finish();
    }

    // 리포트 작성
    if (out_report) {
        out_report->expedite_present = expedite_present;
        out_report->packets_ok = seq_seen.size();
        out_report->seq_seen   = seq_seen;
        out_report->rx_bytes   = rx_bytes;

        out_report->has_first = saw_first;
        out_report->has_cont  = saw_cont;
        out_report->has_last  = saw_last;

        out_report->total_image_bytes = assembled.size();
        out_report->output_image_path = out_bitstream.last_final_path();
        out_report->source_raw_path = raw_src_path;
        
        // 연속성/누락 seq 계산 (14-bit 순환은 고려하지 않고 단순 구간으로 보고)
        std::vector<uint16_t> s = seq_seen;
        std::sort(s.begin(), s.end());
        s.erase(std::unique(s.begin(), s.end()), s.end());

        bool continuity_ok = true;
        std::vector<uint16_t> missing;
        if (!s.empty()) {
            uint16_t prev = s.front();
            for (size_t i = 1; i < s.size(); ++i) {
                uint16_t cur = s[i];
                if (cur != uint16_t(prev + 1)) {
                    // prev+1 ~ cur-1 누락
                    uint16_t gap_from = uint16_t(prev + 1);
                    uint16_t gap_to   = uint16_t(cur - 1);
                    for (uint16_t g = gap_from; g != uint16_t(gap_to + 1); ++g) {
                        missing.push_back(g);
                    }
                }
                prev = cur;
            }
            continuity_ok = missing.empty();
        }
        out_report->continuity_ok = continuity_ok;
        out_report->missing_seq   = missing;

        // JPEG 무결성: SOI/EOI
        out_report->soi_ok = (assembled.size() >= 2 &&
                              assembled[0] == 0xFF && assembled[1] == 0xD8);
        out_report->eoi_ok = (assembled.size() >= 2 &&
                              assembled[assembled.size()-2] == 0xFF &&
                              assembled[assembled.size()-1] == 0xD9);

        // REREQUEST 바이트 범위는 파일 크기/청크 크기를 모르면 계산 못하니 빈 값 유지
        // (report 포맷은 유지)
    }

    // 성공 판정: 하나라도 이미지 페이로드가 누적되었는가
    return !assembled.empty();
}
