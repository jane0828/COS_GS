#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include <sys/stat.h>
#include <ctime>

#include "../inc/ros/telemetry.h"

static constexpr uint32_t RX_PREAMBLE  = 0xA1B2C3D4u; 
static constexpr uint32_t RX_POSTAMBLE = ~RX_PREAMBLE; 


// ===== CRC32 (IEEE 802.3) =====
uint32_t TelemetryHandler::crc32_ieee(const uint8_t* data, size_t len) {
    const uint32_t POLY = 0x04C11DB7u;
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 1u) ? ((crc >> 1) ^ POLY) : (crc >> 1);
        }
    }
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

    TelemetryHeader hdr{};
    std::memcpy(&hdr, buf, sizeof(hdr));

    if (hdr.preamble != RX_PREAMBLE) {
        std::fprintf(stderr,
            "[tm] bad preamble: got=0x%08X expect=0x%08X\n",
            hdr.preamble, RX_PREAMBLE);
        return -2;
    }
    if (hdr.msg_type != TM_TYPE_TELEMETRY) {
        std::fprintf(stderr, "[tm] unsupported msg_type: 0x%08X\n", hdr.msg_type);
    }

    // total_len = 프레임 전체 길이(헤더36 + payload + CRC4 + POST4)
    const uint32_t N = hdr.total_len;
    if (N < (TM_FIXED_HEADER + 8)) {
        std::fprintf(stderr, "[tm] invalid total_len: %u (< 44)\n", N);
        return -3;
    }
    const size_t need = (size_t)N;
    if (n < need) {
        std::fprintf(stderr, "[tm] incomplete: have=%zu need=%zu (N=%u)\n", n, need, N);
        return -3;
    }

    // payload 길이 및 포인터
    const size_t payload_len = (size_t)N - (TM_FIXED_HEADER + 8);
    const uint8_t* payload = buf + TM_FIXED_HEADER;
    const uint8_t* p_crc   = payload + payload_len;   // ← FIX
    const uint8_t* p_post  = p_crc + 4;

    // CRC32 확인: 헤더+payload 전체(N-8) 구간
    uint32_t rx_crc = 0;
    std::memcpy(&rx_crc, p_crc, 4);
    if (crc_check_) {
        const size_t crc_region_len = TM_FIXED_HEADER + payload_len; // = N - 8
        const uint8_t* crc_region   = buf;                           // 헤더부터
        uint32_t calc = crc32_ieee(crc_region, crc_region_len);      // ← FIX
        if (rx_crc != calc) {
            std::fprintf(stderr, "[tm] CRC mismatch: rx=0x%08X calc=0x%08X (region=%zu)\n",
                         rx_crc, calc, crc_region_len);
            // 경고만
        }
    }

    uint32_t post = 0;
    std::memcpy(&post, p_post, 4);
    if (post != RX_POSTAMBLE) {  // ← FIX
        std::fprintf(stderr, "[tm] bad postamble: 0x%08X (expect 0x%08X)\n", post, RX_POSTAMBLE);
        // 경고만
    }

    (void)SaveRaw(buf, n);

    std::printf("[tm] %s (payload=%zu)\n", header_summary(hdr).c_str(), payload_len);
    return dispatch_payload(payload, payload_len);   // ← FIX
}



int TelemetryHandler::dispatch_payload(const uint8_t* payload, size_t len) {
    if (!payload) return -10;

    // csp_id(4) + Beacon(143) + inner_crc(4) = 151
    const size_t need = 4 + sizeof(Beacon) + 4;
    if (len < 4+4){
        std::fprintf(stderr, "[tm] payload too short: %zu (need %zu)\n", len, need);
        return -10;
    }

    // csp_id (로그용)
    uint32_t csp_id = 0;
    std::memcpy(&csp_id, payload, 4);

    // Beacon 시작/길이
    const uint8_t* app   = payload + 4;
    const size_t   app_n = sizeof(Beacon);

    uint16_t msgid = 0;
    std::memcpy(&msgid, app, 2);
    msgid = ntohs(msgid);  

    if (msgid == (uint16_t)BEACON_CC_ID) { // 매크로 이름은 그대로 사용
        return handle_beacon_direct(app, app_n);
    }
    if (msgid == (uint16_t)REPORT_CC_ID) {
        return handle_report_direct(app, sizeof(Report));
    }
    
    std::fprintf(stderr, "[tm] unknown app msgid=0x%04X (csp=0x%08X)\n", msgid, csp_id);
    return -30;
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

int TelemetryHandler::handle_report_direct(const uint8_t* payload, size_t len) {
    if (len < sizeof(Report)) {
        std::fprintf(stderr, "[tm] report payload too short: %zu (need %zu)\n", len, sizeof(Report));
        return -20;
    }
    Report r{};
    std::memcpy(&r, payload, sizeof(Report));

    ReportHandler rh;
    rh.rep = &r;
    rh.parseReportData();

    if (len > sizeof(Report)) {
        std::printf("[tm] report extra bytes: %zu\n", len - sizeof(Report));
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
