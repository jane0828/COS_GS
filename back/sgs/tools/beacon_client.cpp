#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include "../ros/inc/ros/beacon.h"  // Beacon struct(148B)

// 편의 매크로
static inline uint16_t H16(uint16_t v){ return htons(v); }
static inline uint32_t H32(uint32_t v){ return htonl(v); }
static inline int16_t  H16S(int16_t v){ return (int16_t)htons((uint16_t)v); }

int main() {
    // 목적지 127.0.0.1:2001
    int s = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) { perror("socket"); return 1; }
    sockaddr_in dst{}; dst.sin_family = AF_INET;
    dst.sin_port = htons(2001);
    dst.sin_addr.s_addr = inet_addr("127.0.0.1");

    Beacon b{};
    // ===== Header =====
    b.CCMessage_ID = H16(0xBEAC);    // 임의
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

    // 송신
    ssize_t n = ::sendto(s, &b, sizeof(b), 0, (sockaddr*)&dst, sizeof(dst));
    if (n < 0) { perror("sendto"); ::close(s); return 1; }
    std::printf("Sent %zd bytes to %s:%d\n", n, "127.0.0.1", 2001);
    ::close(s);
    return 0;
}
