#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include <sys/stat.h>
#include <ctime>

#include "../inc/ros/telemetry.h"

// ===== CRC32 (IEEE 802.3) =====
uint32_t TelemetryHandler::crc32_ieee(const uint8_t* data, size_t len) {
    static uint32_t table[256];
    static bool init = false;
    if (!init) {
        for (uint32_t i=0; i<256; ++i) {
            uint32_t c = i;
            for (int j=0; j<8; ++j)
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            table[i] = c;
        }
        init = true;
    }
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i=0; i<len; ++i)
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

std::string TelemetryHandler::header_summary(const TelemetryHeader& h) {
    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "Preamble=0x%08X Len(N)=%u Type=0x%08X IRIGB=%llu SatID=%.0f Slant=%.3f Alt=%.3f Elev=%.3f",
        h.preamble, h.total_len, h.msg_type,
        (unsigned long long)h.irigb_time_tag,
        h.sat_id, h.slant, h.altitude, h.elevation);
    return std::string(buf);
}

int TelemetryHandler::parse_and_dispatch(const uint8_t* buf, size_t n) {
    if (!buf || n < TM_FIXED_HEADER + 8) { // 헤더 + CRC + POST 최소 보장
        std::fprintf(stderr, "[tm] too short: %zu\n", n);
        return -1;
    }

    // 헤더 추출(unaligned 안전)
    TelemetryHeader hdr{};
    std::memcpy(&hdr, buf, sizeof(hdr));

    // 엔디안은 문서에 명시 없으니 그대로 가정. 필요 시 ntohl/ntohll 적용.
    if (hdr.preamble != TM_PREAMBLE) {
        std::fprintf(stderr, "[tm] bad preamble: 0x%08X\n", hdr.preamble);
        return -2;
    }
    if (hdr.msg_type != TM_TYPE_TELEMETRY) {
        std::fprintf(stderr, "[tm] unsupported msg_type: 0x%08X\n", hdr.msg_type);
        // 필요하면 다른 타입도 열어둘 수 있음
    }

    const uint32_t N = hdr.total_len;             // payload 길이
    const size_t need = TM_FIXED_HEADER + (size_t)N + 8; // +CRC32(4)+POST(4)
    if (n < need) {
        std::fprintf(stderr, "[tm] incomplete: have=%zu need=%zu (N=%u)\n", n, need, N);
        return -3;
    }

    const uint8_t* payload = buf + TM_FIXED_HEADER;
    const uint8_t* p_crc   = payload + N;
    const uint8_t* p_post  = p_crc + 4;

    // CRC32 확인
    uint32_t rx_crc = 0;
    std::memcpy(&rx_crc, p_crc, 4);
    if (crc_check_) {
        uint32_t calc = crc32_ieee(payload, N);
        if (rx_crc != calc) {
            std::fprintf(stderr, "[tm] CRC mismatch: rx=0x%08X calc=0x%08X (N=%u)\n", rx_crc, calc, N);
            // 필요 시 계속 진행할지 여부 선택. 일단 계속 진행.
        }
    }

    uint32_t post = 0;
    std::memcpy(&post, p_post, 4);
    if (post != TM_POSTAMBLE) {
        std::fprintf(stderr, "[tm] bad postamble: 0x%08X (expect 0x%08X)\n", post, TM_POSTAMBLE);
        // 일단 경고만
    }

    (void)SaveRaw(buf, n);

    std::printf("[tm] %s\n", header_summary(hdr).c_str());
    return dispatch_payload(payload, N);
}

int TelemetryHandler::dispatch_payload(const uint8_t* payload, size_t len) {
    if (!payload || len == 0) return -10;

    // 1) (임시) CSP 헤더 없이 Beacon 바로 오는 경우
    if (len >= sizeof(Beacon)) {
        // CCMessage_ID는 payload 시작 0..1 에 있음(Beacon 정의)
        uint16_t cc = 0;
        std::memcpy(&cc, payload, sizeof(cc));
        // “Beacon 식별자”는 사용자 정책에 맞춰 바꿔도 됨. 여기서는 0xBEAC로 가정.
        if (cc == (uint16_t)0xBEAC) {
            return handle_beacon_direct(payload, len);
        }
        // HK 식별자(미정): 주석으로 가이드만 남김
        // else if (cc == HK_MESSAGE_ID) { return handle_hk_direct(payload, len); }

        // 아니라면 (미래) CSP가 앞에 있을 수도 있으니 CSP 경로 시도
        return handle_csp_then_dispatch(payload, len);
    }

    // payload가 Beacon보다 짧으면 CSP 헤더가 있을 가능성 → CSP 경로
    return handle_csp_then_dispatch(payload, len);
}

int TelemetryHandler::handle_beacon_direct(const uint8_t* payload, size_t len) {
    if (len < sizeof(Beacon)) {
        std::fprintf(stderr, "[tm] beacon payload too short: %zu (need %zu)\n", len, sizeof(Beacon));
        return -20;
    }
    Beacon b{};
    std::memcpy(&b, payload, sizeof(Beacon));

    BeaconHandler bh;
    bh.bec = &b;
    bh.parseBeaconData();  // 콘솔/파일 저장

    // 남은 바이트가 있으면 (확장 필드?) 덤프
    if (len > sizeof(Beacon)) {
        std::printf("[tm] beacon extra bytes: %zu\n", len - sizeof(Beacon));
    }
    return 0;
}

int TelemetryHandler::handle_csp_then_dispatch(const uint8_t* payload, size_t len) {
    // TODO: CSP 헤더 스펙 추가되면 여기서 파싱하고,
    //       이어지는 application payload의 CCMessage_ID로 Beacon/HK 분기
    std::fprintf(stderr, "[tm] TODO: CSP header parse not implemented (len=%zu)\n", len);
    return -30;
}

int TelemetryHandler::handle_hk_direct(const uint8_t* /*payload*/, size_t /*len*/) {
    // TODO: HK 구조체 정의 후 구현
    std::fprintf(stderr, "[tm] TODO: HK parse not implemented\n");
    return -40;
}

int TelemetryHandler::SaveRaw(const uint8_t* buf, size_t len) {
    if (!buf || len == 0) return -1;

    struct stat st{};
    if (stat("./data", &st) != 0)
        if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return -1; }
    if (stat("./data/raw", &st) != 0)
        if (mkdir("./data/raw", 0755) != 0) { perror("mkdir ./data/raw"); return -1; }

    time_t now = time(nullptr);
    struct tm *lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/raw/Telemetry--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);

    FILE *fp = std::fopen(path, "w");
    if (!fp) { perror(path); return -1; }

    std::fprintf(fp, "Raw telemetry dump (%zu bytes)\n", len);
    for (size_t i = 0; i < len; ++i) {
        std::fprintf(fp, "%02X%s", buf[i], ((i+1)%16==0) ? "\n" : " ");
    }
    std::fprintf(fp, "\n");
    std::fclose(fp);

    std::printf("[tm] raw saved to %s\n", path);
    return 0;
}
