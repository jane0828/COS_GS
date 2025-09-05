// tools/telemetry_client.cpp
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <ctime>
#include <arpa/inet.h>
#include <unistd.h>

#include <endian.h>
#include "../ros/inc/ros/telemetry.h"
#include "../ros/inc/ros/beacon.h"

#define SERVER_IP   "127.0.0.1"
#define SERVER_PORT 2001


static inline uint16_t H16(uint16_t v){ return htole16(v); }
static inline uint32_t H32(uint32_t v){ return htole32(v); }
static inline int16_t  H16S(int16_t v){ return (int16_t)htole16((uint16_t)v); }

static uint32_t crc32_ieee_local(const uint8_t* data, size_t len) {
    static uint32_t table[256]; static bool init=false;
    if (!init) {
        for (uint32_t i=0;i<256;++i) {
            uint32_t c=i; for (int j=0;j<8;++j)
                c = (c&1)? (0xEDB88320u ^ (c>>1)) : (c>>1);
            table[i]=c;
        }
        init=true;
    }
    uint32_t crc=0xFFFFFFFFu;
    for (size_t i=0;i<len;++i)
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

int main() {
    // --- UDP 소켓 ---
    int sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    sockaddr_in servaddr{};
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // --- payload: Beacon 예시 ---
     Beacon b{};
    // ===== Header =====
    b.CCMessage_ID = H16(BEACON_CC_ID);    // 임의
    b.CCSequence   = H16(1);         // 임의
    b.CCLength     = H16(sizeof(Beacon)); // 148 고정
    // 예: 2025-08-31 15:24:40 -> [25 08 31 15 24 40]
    b.CCTime_code[0]=0x25; b.CCTime_code[1]=0x08; b.CCTime_code[2]=0x31;
    b.CCTime_code[3]=0x15; b.CCTime_code[4]=0x24; b.CCTime_code[5]=0x40;

    // ===== FSW.SRL =====  (로그: cnt=0, err=0, I2C0=7/0, I2C1=7/1687, I2C2=7/181, UART=7/0, RS422=7/1267)
    b.SRL_CommandCounter      = 0;
    b.SRL_CommandErrorCounter = 0;
    b.SRL_I2C0_Status = 7;   b.SRL_I2C0_TxCount = H16(0);
    b.SRL_I2C1_Status = 7;   b.SRL_I2C1_TxCount = H16(1687);
    b.SRL_I2C2_Status = 7;   b.SRL_I2C2_TxCount = H16(181);
    b.SRL_UART_Status = 7;   b.SRL_UART_TxCount = H16(0);
    b.SRL_RS422_Status= 7;   b.SRL_RS422_TxCount= H16(1267);

    // ===== FSW.RPT =====  (cnt=0/0, q=10/0, boot=3, time=1051553.804174644, seq=2527, reset=33)
    b.RPT_CmdCounter        = 0;
    b.RPT_ErrCounter        = 0;
    b.RPT_ReportQueueCount  = 10;
    b.RPT_CriticalQueueCount= 0;
    b.RPT_BootCount         = H16(3);
    b.RPT_SpacecraftTimeSec    = H32(1051553);
    b.RPT_SpacecraftTimeSubsec = H32(804174644); // 소수부를 ns처럼 804,174,644로 사용
    b.RPT_Sequence          = H32(2527);
    b.RPT_ResetCause        = 33;

    // ===== COMS =====
    // [SANT] status=1
    b.SANT_status = 1;
    // [AX2150] rssi=-116, bootcnt=85, cause=0x00000002, rx=5, wdt_cnt=1, wdt_left=57397
    b.STRX_last_rssi   = H16S(-116);
    b.STRX_bootcount   = H16(85);
    b.STRX_bootcause   = H32(0x00000002);
    b.STRX_rx_mode     = 5;
    b.STRX_gnd_wdt_cnt = H16(1);
    b.STRX_gnd_wdt_left= H32(57397);
    // [UANT] deploy=0x8888
    b.UANT_report_deploy = H16(0x8888);
    // [AX100] active=2, boot=81, cause=0x00000001
    b.UTX_active_conf = 2;
    b.UTX_boot_count  = H16(81);
    b.UTX_boot_cause  = H32(0x00000001);

    // ===== EPS (P31U) =====
    // vbatt=14471, output=[0,0,0,0,1,0,0,0], curout=[1,0,0,2,458,1], curin=[465,0,282]
    // cursys=389, boot_cnt=94, wdt_left=0, bootcause=8, battmode=3
    b.EPS_vbatt = H16(14471);
    uint8_t out8[8] = {0,0,0,0,1,0,0,0};
    std::memcpy(b.EPS_output, out8, 8);
    uint16_t curout6[6] = {1,0,0,2,458,1};
    for (int i=0;i<6;i++) b.EPS_curout[i] = H16(curout6[i]);
    uint16_t curin3[3] = {465,0,282};
    for (int i=0;i<3;i++) b.EPS_curin[i] = H16(curin3[i]);
    b.EPS_cursys        = H16(389);
    b.EPS_counter_boot  = H16(94);
    b.EPS_wdt_gnd_time_left = H32(0);
    b.EPS_bootcause     = 8;
    b.EPS_battmode      = 3;
    b.EPS_batt_heater_mode = 0; // 미지정 → 0
    // [SOLAR] deploy=1
    b.EPS_solar_panel_deploy = 1;
    // 온도 2개는 로그에 없어서 0
    b.EPS_BP4_Temperature[0] = H16(0);
    b.EPS_BP4_Temperature[1] = H16(0);

    // ===== ADCS =====  power=4, ctrl=0, gyro=[0.02, 0.0, 0.0125]
    b.ADCS_power_state  = 4;
    b.ADCS_control_mode = 0;
    b.ADCS_gyro0_cal_rate_x = 0.020000f;
    b.ADCS_gyro0_cal_rate_y = 0.000000f;
    b.ADCS_gyro0_cal_rate_z = 0.012500f;

    // ===== CAM ===== mode=0, mem=[2,2,0,0,0,0], img=[2,2,0,0,0,0]
    b.PAY_cam_mode = 0;
    uint8_t mem6[6] = {2,2,0,0,0,0};
    uint8_t img6[6] = {2,2,0,0,0,0};
    std::memcpy(b.PAY_cam_memory_status, mem6, 6);
    std::memcpy(b.PAY_cam_image_number,  img6, 6);

    // ===== DEORBIT ===== adc=[2,1]
    b.PAY_deorbit_adc[0] = H16(2);
    b.PAY_deorbit_adc[1] = H16(1);

    const uint32_t N = sizeof(Beacon);

    // --- TelemetryHeader 채우기 ---
    TelemetryHeader hdr{};
    hdr.preamble       = TM_PREAMBLE;       // 0xA1B2C34D
    hdr.total_len      = N;                 // payload 길이
    hdr.msg_type       = TM_TYPE_TELEMETRY; // 0x00
    hdr.irigb_time_tag = static_cast<uint64_t>(time(nullptr));
    hdr.sat_id         = 1.0f;
    hdr.slant          = 0.0f;
    hdr.altitude       = 500.0f;
    hdr.elevation      = 30.0f;

    // --- 전체 프레임 버퍼 구성: [Hdr(36) | payload(N) | crc(4) | post(4)] ---
    const size_t total_len = TM_FIXED_HEADER + N + 8;
    std::vector<uint8_t> buf(total_len);

    // Header
    std::memcpy(buf.data(), &hdr, sizeof(hdr));

    // Payload
    std::memcpy(buf.data() + TM_FIXED_HEADER, &b, N);

    // CRC32 over payload
    const uint32_t crc = crc32_ieee_local(buf.data() + TM_FIXED_HEADER, N);
    std::memcpy(buf.data() + TM_FIXED_HEADER + N, &crc, sizeof(crc));

    // Postamble
    const uint32_t post = TM_POSTAMBLE;
    std::memcpy(buf.data() + TM_FIXED_HEADER + N + 4, &post, sizeof(post));

    // --- 전송 ---
    const ssize_t sent = ::sendto(sockfd, buf.data(), buf.size(), 0,
                                  reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr));
    if (sent < 0) { perror("sendto"); ::close(sockfd); return 1; }

    std::cout << "[client] Sent " << sent << " bytes telemetry frame\n";
    ::close(sockfd);
    return 0;
}
