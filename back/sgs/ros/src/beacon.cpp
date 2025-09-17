#include <ctime>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "../inc/ros/beacon.h"
#include <endian.h>

// ===== 엔디안 도우미 (단 한 번 정의) =====
static inline uint16_t LE16(uint16_t v) { return le16toh(v); }
static inline uint32_t LE32(uint32_t v) { return le32toh(v); }
static inline uint16_t BE16(uint16_t v) { return be16toh(v); }
static inline uint32_t BE32(uint32_t v) { return be32toh(v); }

// ===== 출력 유틸 (단 한 번 정의) =====
static void print_u8_array(FILE* out, const char* name, const uint8_t* a, size_t n) {
    std::fprintf(out, "%s=[", name);
    for (size_t i=0;i<n;i++) std::fprintf(out, "%u%s", (unsigned)a[i], (i+1<n)?", ":"");
    std::fprintf(out, "]\n");
}
static void print_u16_array(FILE* out, const char* name, const uint16_t* a, size_t n) {
    std::fprintf(out, "%s=[", name);
    for (size_t i=0;i<n;i++) std::fprintf(out, "%u%s", (unsigned)LE16(a[i]), (i+1<n)?", ":"");
    std::fprintf(out, "]\n");
}
static void print_i16_array(FILE* out, const char* name, const int16_t* a, size_t n) {
    std::fprintf(out, "%s=[", name);
    for (size_t i=0;i<n;i++) std::fprintf(out, "%d%s", (int16_t)LE16((uint16_t)a[i]), (i+1<n)?", ":"");
    std::fprintf(out, "]\n");
}

static void dump_beacon(FILE* out, const Beacon& b, bool include_binary) {
    // ===== Header =====
    std::fprintf(out, "CCID=0x%04X Seq=0x%04X Len=%u\n",
                 BE16(b.CCMessage_ID), BE16(b.CCSequence), BE16(b.CCLength)+7);
    std::fprintf(out, "Tcode=%02X %02X %02X %02X %02X %02X\n",
                 b.CCTime_code[0], b.CCTime_code[1], b.CCTime_code[2],
                 b.CCTime_code[3], b.CCTime_code[4], b.CCTime_code[5]);

    // ===== FSW.SRL =====
    // std::puts("[SRL]===============================");
    std::fprintf(out, " cnt=%u, err=%u\n", b.SRL_CommandCounter, b.SRL_CommandErrorCounter);
    std::fprintf(out, " I2C0=%u/%u,  I2C1=%u/%u,  I2C2=%u/%u\n",
                 b.SRL_I2C0_Status, LE16(b.SRL_I2C0_TxCount),
                 b.SRL_I2C1_Status, LE16(b.SRL_I2C1_TxCount),
                 b.SRL_I2C2_Status, LE16(b.SRL_I2C2_TxCount));
    std::fprintf(out, " UART=%u/%u\n",  b.SRL_UART_Status,  LE16(b.SRL_UART_TxCount));
    std::fprintf(out, " RS422=%u/%u\n", b.SRL_RS422_Status, LE16(b.SRL_RS422_TxCount));

    // ===== FSW.RPT =====
    // std::puts("[RPT]===============================");
    std::fprintf(out, " cnt=%u/%u,  q=%u/%u boot=%u\n",
                 b.RPT_CmdCounter, b.RPT_ErrCounter,
                 b.RPT_ReportQueueCount, b.RPT_CriticalQueueCount,
                 LE16(b.RPT_BootCount));
    std::fprintf(out, " time=%u.%u\n", LE32(b.RPT_SpacecraftTimeSec), LE32(b.RPT_SpacecraftTimeSubsec));
    std::fprintf(out, " seq=%u,  reset=%u\n", LE32(b.RPT_Sequence), b.RPT_ResetCause);

    // ===== COMS =====
    // std::puts("[SANT]===============================");
    std::fprintf(out, " status=%u\n", b.SANT_status);

    // std::puts("[AX2150]============================");
    std::fprintf(out, " rssi=%d\n", (int16_t)LE16((uint16_t)b.STRX_last_rssi));
    std::fprintf(out, " bootcnt=%u\n", LE16(b.STRX_bootcount));
    std::fprintf(out, " cause=0x%08X\n", LE32(b.STRX_bootcause));
    std::fprintf(out, " rx=%u\n", b.STRX_rx_mode);
    std::fprintf(out, " wdt_cnt=%u\n", LE16(b.STRX_gnd_wdt_cnt));
    std::fprintf(out, " wdt_left=%u\n", LE32(b.STRX_gnd_wdt_left));

    // std::puts("[UANT]===============================");
    std::fprintf(out, " deploy=0x%04X\n", LE16(b.UANT_report_deploy));

    // std::puts("[AX100]==============================");
    std::fprintf(out, " active=%u\n", b.UTX_active_conf);
    std::fprintf(out, " boot=%u\n", LE16(b.UTX_boot_count));
    std::fprintf(out, " cause=0x%08X\n", LE32(b.UTX_boot_cause));

    // ===== EPS =====
    // std::puts("[P31U]===============================");
    std::fprintf(out, " vbatt=%u mV\n", LE16(b.EPS_vbatt));
    print_u8_array(out,  " output",  b.EPS_output, 8);
    print_u16_array(out, " curout",  b.EPS_curout, 6);
    print_u16_array(out, " curin",   b.EPS_curin,  3);
    std::fprintf(out, " cursys=%u\n", LE16(b.EPS_cursys));
    std::fprintf(out, " boot_cnt=%u\n", LE16(b.EPS_counter_boot));
    std::fprintf(out, " wdt_left=%u\n", LE32(b.EPS_wdt_gnd_time_left));
    std::fprintf(out, " bootcause=%u\n", b.EPS_bootcause);
    std::fprintf(out, " battmode=%u\n", b.EPS_battmode);
    // std::fprintf(out, " heater_mode=%u\n", b.EPS_batt_heater_mode);

    // std::puts("[SOLAR]==============================");
    std::fprintf(out, " deploy=%u\n", b.EPS_solar_panel_deploy);
    // std::fprintf(out, " BP4_Temperature=[%d, %d]\n",
    //     (int16_t)LE16((uint16_t)b.EPS_BP4_Temperature[0]),
    //     (int16_t)LE16((uint16_t)b.EPS_BP4_Temperature[1]));

    // ===== ADCS =====
    // std::puts("[ADCS]===============================");
    std::fprintf(out, " power=%u\n", b.ADCS_power_state);
    std::fprintf(out, " ctrl=%u\n",  b.ADCS_control_mode);
    std::fprintf(out, " gyro=[%.6f, %.6f, %.6f]\n",
                 b.ADCS_gyro0_cal_rate_x, b.ADCS_gyro0_cal_rate_y, b.ADCS_gyro0_cal_rate_z);

    // ===== PAY UZURO =====
    // std::puts("[CAM]===============================");
    std::fprintf(out, " mode=%u\n", b.PAY_cam_mode);
    print_u8_array(out, " mem", b.PAY_cam_memory_status, 6);
    print_u8_array(out, " img", b.PAY_cam_image_number,  6);

    // std::puts("[DEORBIT]===========================");
    std::fprintf(out, " adc=[%u, %u]\n",
                 LE16(b.PAY_deorbit_adc[0]), LE16(b.PAY_deorbit_adc[1]));

    if (include_binary) {
        std::fprintf(out, "\nBinary (%zu B):\n", sizeof(Beacon));
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&b);
        for (size_t i = 0; i < sizeof(Beacon); ++i)
            std::fprintf(out, "%02X%s", p[i], ((i+1)%16==0) ? "\n" : " ");
        std::fprintf(out, "\n");
    }
}

// ===== 클래스 구현 =====
BeaconHandler::BeaconHandler() {
    std::memset(BeaconFileName, 0, sizeof(BeaconFileName));
    std::snprintf(BeaconFileName, sizeof(BeaconFileName),
                  "beacon_log_%ld.dat", (long)time(nullptr));
}
BeaconHandler::~BeaconHandler() {
    if (sql) mysql_close(sql);
}

int BeaconHandler::Save2File() {
    if (!bec) { std::fprintf(stderr, "Save2File: bec=null\n"); return -1; }

    struct stat st{};
    if (stat("./data", &st) != 0)
        if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return -1; }
    if (stat("./data/beacon", &st) != 0)
        if (mkdir("./data/beacon", 0755) != 0) { perror("mkdir ./data/beacon"); return -1; }

    time_t now = time(nullptr);
    struct tm *lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/beacon/Beacon--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);

    FILE *fp = std::fopen(path, "w");
    if (!fp) { perror(path); return -1; }

    dump_beacon(fp, *bec, /*include_binary=*/true);
    std::fclose(fp);
    std::printf("[beacon] Saved to %s\n", path);
    return 0;
}

int BeaconHandler::Save2DB()  { std::printf("[beacon] Save2DB: (not implemented)\n"); return 0; }
int BeaconHandler::File2DB()  { std::printf("[beacon] File2DB: (not implemented)\n"); return 0; }

std::string beacon_summary(const Beacon& b) {
    char tcode[32];
    std::snprintf(tcode, sizeof(tcode), "%02X %02X %02X %02X %02X %02X",
                  b.CCTime_code[0], b.CCTime_code[1], b.CCTime_code[2],
                  b.CCTime_code[3], b.CCTime_code[4], b.CCTime_code[5]);

    char line[256];
    std::snprintf(line, sizeof(line),
        "CCID=0x%04X Seq=0x%04X Len=%u T=[%s] Boot=%u SCET=%u.%u SANT=%u RSSI=%d EPSV=%u ADCS=%u PAY=%u",
        LE16(b.CCMessage_ID), LE16(b.CCSequence), LE16(b.CCLength), tcode,
        LE16(b.RPT_BootCount), LE32(b.RPT_SpacecraftTimeSec), LE32(b.RPT_SpacecraftTimeSubsec),
        b.SANT_status, (int16_t)LE16((uint16_t)b.STRX_last_rssi),
        LE16(b.EPS_vbatt), b.ADCS_control_mode, b.PAY_cam_mode
    );
    return std::string(line);
}

void BeaconHandler::parseBeaconData() {
    if (!bec) { std::fprintf(stderr, "parseBeaconData: bec=null\n"); return; }
    // dump_beacon(stdout, *bec, /*include_binary=*/false);  // 콘솔엔 텍스트만
    (void)Save2File();
    // (void)Save2DB();
}
