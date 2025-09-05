#pragma once
#ifndef _SGS_TELEMETRY_H_
#define _SGS_TELEMETRY_H_

#include <cstdint>
#include <cstddef>
#include <string>

#include "beacon.h"   // Beacon 구조체 사용


#pragma pack(push, 1)
struct CSPHeader {
    uint32_t header;   // 임의 값(예: flags)
    uint8_t  src;      // source node
    uint8_t  dst;      // destination node
    uint16_t dport;    // dest port
};
#pragma pack(pop)

static constexpr size_t TM_MAX_PAYLOAD = 2048;


struct TelemetryFrame {
    CSPHeader csp;
    uint16_t  payload_len;            // payload 길이(바이트)
    uint8_t   payload[TM_MAX_PAYLOAD];// 실제 전송 시에는 payload_len 만큼만 사용
};
// === 고정값(스펙) ===
static constexpr uint32_t TM_PREAMBLE  = 0xA1B2C34D;
static constexpr uint32_t TM_POSTAMBLE = ~TM_PREAMBLE;     // 표에 "~0xA1B2C34D"
static constexpr uint32_t TM_TYPE_TELEMETRY = 0x00000000;       // 표의 Message type 0x00
static constexpr size_t   TM_FIXED_HEADER = 36;            // 0..35 바이트 (payload 시작 offset)

// header는 packed로 정의
#pragma pack(push, 1)
struct TelemetryHeader {
    uint32_t preamble;        // 0:  4  (0xA1B2C34D)
    uint32_t total_len;       // 4:  4  (1..16384)  == N (payload 길이, CRC/포스트앰블 제외)
    uint32_t msg_type;        // 8:  4  (0x00)
    uint64_t irigb_time_tag;  // 12: 8
    float    sat_id;          // 20: 4  (문서대로 float 유지)
    float    slant;           // 24: 4
    float    altitude;        // 28: 4
    float    elevation;       // 32: 4
}; // 36 bytes
#pragma pack(pop)

// 프레임 전체 레이아웃:
// [ TelemetryHeader(36) | payload(N) | crc32(4) | postamble(4) ]  => 총 길이 36 + N + 8

class TelemetryHandler {
public:
    // 파싱 + 디스패치 (성공: 0, 실패: 음수)
    // - buf: 수신 버퍼 전체
    // - n  : 수신 길이
    int parse_and_dispatch(const uint8_t* buf, size_t n);
    int dispatch_payload(const uint8_t* payload, size_t len);
    int handle_beacon_direct(const uint8_t* payload, size_t len);
    int handle_csp_then_dispatch(const uint8_t* payload, size_t len); // TODO
    int handle_hk_direct(const uint8_t* payload, size_t len);         // TODO

    // (옵션) CRC32 체크 on/off
    void set_crc_check(bool on) { crc_check_ = on; }
    
    static uint32_t crc32_ieee(const uint8_t* data, size_t len);
    static std::string header_summary(const TelemetryHeader& h);

    int SaveRaw(const uint8_t* buf, size_t len); 
private:
    bool crc_check_ = true;
};

#endif // _SGS_TELEMETRY_H_
