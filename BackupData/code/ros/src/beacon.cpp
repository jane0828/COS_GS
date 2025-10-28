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

// int BeaconHandler::Save2DB()  { std::printf("[beacon] Save2DB: (not implemented)\n"); return 0; }
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
    (void)Save2DB();
}

static std::string mysql_escape(MYSQL* sql, const std::string& s){
    std::string out; out.resize(s.size()*2+1);
    unsigned long n = mysql_real_escape_string(sql, &out[0], s.c_str(), (unsigned long)s.size());
    out.resize(n);
    return out;
}

// UTC -> 'YYYY-MM-DD HH:MM:SS'
static std::string to_mysql_datetime_utc(time_t t){
    struct tm g{}; gmtime_r(&t,&g);
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
                  g.tm_year+1900, g.tm_mon+1, g.tm_mday, g.tm_hour, g.tm_min, g.tm_sec);
    return std::string(buf);
}

// === get-or-create helpers ===
static long long ensure_app_id(MYSQL* sql, const std::string& label){
    std::string el = mysql_escape(sql, label);
    std::string q = "SELECT app_id FROM applications WHERE code='"+el+"' OR label='"+el+"' LIMIT 1";
    if (mysql_query(sql, q.c_str())==0){
        if (MYSQL_RES* r = mysql_store_result(sql)){
            if (MYSQL_ROW row = mysql_fetch_row(r)){
                long long id = row[0] ? std::strtoll(row[0],nullptr,10) : 0;
                mysql_free_result(r); return id;
            }
            mysql_free_result(r);
        }
    }
    q = "INSERT INTO applications (code,label,sort_order) VALUES ('"+el+"','"+el+"',0)";
    if (mysql_query(sql, q.c_str())!=0){ std::fprintf(stderr,"ensure_app_id insert: %s\n", mysql_error(sql)); return -1; }
    return (long long)mysql_insert_id(sql);
}

static long long ensure_part_id(MYSQL* sql, long long app_id, const std::string& label){
    std::string el = mysql_escape(sql, label);
    char sel[512];
    std::snprintf(sel,sizeof(sel),
        "SELECT part_id FROM parts WHERE app_id=%lld AND (code='%s' OR label='%s') LIMIT 1",
        app_id, el.c_str(), el.c_str());
    if (mysql_query(sql, sel)==0){
        if (MYSQL_RES* r = mysql_store_result(sql)){
            if (MYSQL_ROW row = mysql_fetch_row(r)){
                long long id = row[0] ? std::strtoll(row[0],nullptr,10) : 0;
                mysql_free_result(r); return id;
            }
            mysql_free_result(r);
        }
    }
    char ins[512];
    std::snprintf(ins,sizeof(ins),
        "INSERT INTO parts (app_id,code,label,sort_order) VALUES (%lld,'%s','%s',0)",
        app_id, el.c_str(), el.c_str());
    if (mysql_query(sql, ins)!=0){ std::fprintf(stderr,"ensure_part_id insert: %s\n", mysql_error(sql)); return -1; }
    return (long long)mysql_insert_id(sql);
}

static long long ensure_telemetry_id(MYSQL* sql, long long part_id,
                                     const std::string& name, const std::string& dtype){
    std::string en = mysql_escape(sql, name);
    char sel[512];
    std::snprintf(sel,sizeof(sel),
        "SELECT telemetry_id FROM telemetry_def WHERE part_id=%lld AND name='%s' LIMIT 1",
        part_id, en.c_str());
    if (mysql_query(sql, sel)==0){
        if (MYSQL_RES* r = mysql_store_result(sql)){
            if (MYSQL_ROW row = mysql_fetch_row(r)){
                long long id = row[0] ? std::strtoll(row[0],nullptr,10) : 0;
                mysql_free_result(r); return id;
            }
            mysql_free_result(r);
        }
    }
    std::string ed = mysql_escape(sql, dtype);
    char ins[1024];
    std::snprintf(ins,sizeof(ins),
        "INSERT INTO telemetry_def (part_id,name,label,dtype,is_active,sort_order) "
        "VALUES (%lld,'%s','%s','%s',TRUE,0)",
        part_id, en.c_str(), en.c_str(), ed.c_str());
    if (mysql_query(sql, ins)!=0){ std::fprintf(stderr,"ensure_telemetry_id insert: %s\n", mysql_error(sql)); return -1; }
    return (long long)mysql_insert_id(sql);
}

static int insert_value(MYSQL* sql, long long telemetry_id,
                        const char* value_text, const char* value_num,
                        const std::string& ts_mysql){
    char q[1024];
    std::snprintf(q,sizeof(q),
        "INSERT INTO telemetry_value (telemetry_id,value_text,value_num,created_at) "
        "VALUES (%lld,%s,%s,'%s')",
        telemetry_id,
        value_text ? value_text : "NULL",
        value_num  ? value_num  : "NULL",
        ts_mysql.c_str());
    if (mysql_query(sql, q)!=0){ std::fprintf(stderr,"insert_value: %s\n", mysql_error(sql)); return -1; }
    return 0;
}

// 값 묶음 정의
struct KV {
    std::string name;   // telemetry_def.name
    std::string dtype;  // "int" | "float" | "text"
    bool is_num;
    std::string sval;   // 텍스트일 때
    double dval;        // 숫자일 때
};

// 앱/파트 단위로 upsert + 값 insert
static int upsert_group(MYSQL* sql,
                        const std::string& appLabel,
                        const std::string& partLabel,
                        const std::vector<KV>& kvs,
                        const std::string& ts_mysql)
{
    long long app_id  = ensure_app_id(sql,  appLabel);  if (app_id<0)  return -1;
    long long part_id = ensure_part_id(sql, app_id, partLabel); if (part_id<0) return -1;

    for (const auto& kv : kvs){
        long long tel_id = ensure_telemetry_id(sql, part_id, kv.name, kv.dtype);
        if (tel_id<0) return -1;

        if (kv.is_num){
            char numbuf[64]; std::snprintf(numbuf,sizeof(numbuf), "%.9g", kv.dval);
            if (insert_value(sql, tel_id, nullptr, numbuf, ts_mysql)<0) return -1;
        }else{
            std::string es = mysql_escape(sql, kv.sval);
            std::string tv = "'"+es+"'";
            if (insert_value(sql, tel_id, tv.c_str(), nullptr, ts_mysql)<0) return -1;
        }
    }
    return 0;
}

static void push_idx_u8(std::vector<KV>& kv, const char* base, const uint8_t* a, int n){
    for (int i=0;i<n;i++){
        char nm[64]; std::snprintf(nm,sizeof(nm), "%s_%02d", base, i);
        kv.push_back({nm, "int", true, "", (double)a[i]});
    }
}
static void push_idx_u16(std::vector<KV>& kv, const char* base, const uint16_t* a, int n){
    for (int i=0;i<n;i++){
        char nm[64]; std::snprintf(nm,sizeof(nm), "%s_%02d", base, i);
        kv.push_back({nm, "int", true, "", (double)LE16(a[i])});
    }
}

// REPLACE
int BeaconHandler::Save2DB(){
    if (!bec){ std::fprintf(stderr,"Save2DB: bec=null\n"); return -1; }

    // 1) MySQL 연결 (lazy)
    if (!sql){
        const char* host = std::getenv("MYSQL_HOST"); if (!host) host="127.0.0.1";
        const char* user = std::getenv("MYSQL_USER"); if (!user) user="aclgs";
        const char* pass = std::getenv("MYSQL_PASS"); if (!pass) pass="qwe123";
        const char* dbnm = std::getenv("MYSQL_DB");   if (!dbnm) dbnm="cosmic";
        unsigned port = 3306; if (const char* pp = std::getenv("MYSQL_PORT")) port = std::strtoul(pp,nullptr,10);

        sql = mysql_init(nullptr);
        if (!sql){ std::fprintf(stderr,"mysql_init failed\n"); return -1; }
        if (!mysql_real_connect(sql, host, user, pass, dbnm, port, nullptr, 0)){
            std::fprintf(stderr,"mysql_real_connect: %s\n", mysql_error(sql));
            mysql_close(sql); sql=nullptr; return -1;
        }
    }

    const Beacon& b = *bec;
    const std::string ts = to_mysql_datetime_utc(time(nullptr)); // created_at

    // ========= FSW / SRL (스샷과 동일한 이름) =========
    {
        std::vector<KV> kv{
            {"CommandCounter",       "int", true, "", (double)b.SRL_CommandCounter},
            {"CommandErrorCounter",  "int", true, "", (double)b.SRL_CommandErrorCounter},
            {"I2C0 Status",          "int", true, "", (double)b.SRL_I2C0_Status},
            {"I2C0 TxCount",         "int", true, "", (double)LE16(b.SRL_I2C0_TxCount)},
            {"I2C1 Status",          "int", true, "", (double)b.SRL_I2C1_Status},
            {"I2C1 TxCount",         "int", true, "", (double)LE16(b.SRL_I2C1_TxCount)},
            {"I2C2 Status",          "int", true, "", (double)b.SRL_I2C2_Status},
            {"I2C2 TxCount",         "int", true, "", (double)LE16(b.SRL_I2C2_TxCount)},
            {"RS422 Status",         "int", true, "", (double)b.SRL_RS422_Status},
            {"RS422 TxCount",        "int", true, "", (double)LE16(b.SRL_RS422_TxCount)},
            {"UART Status",          "int", true, "", (double)b.SRL_UART_Status},
            {"UART TxCount",         "int", true, "", (double)LE16(b.SRL_UART_TxCount)}
        };
        if (upsert_group(sql, "FSW", "SRL", kv, ts)<0) return -1;
    }

    // ========= FSW / RPT =========
    {
        std::vector<KV> kv{
            {"Boot Count",             "int", true, "", (double)LE16(b.RPT_BootCount)},
            {"Report Queue Count",     "int", true, "", (double)b.RPT_ReportQueueCount},
            {"Critical Queue Count",   "int", true, "", (double)b.RPT_CriticalQueueCount},
            {"Sequence",               "int", true, "", (double)LE32(b.RPT_Sequence)},
            {"Spacecraft Time (sec)",  "int", true, "", (double)LE32(b.RPT_SpacecraftTimeSec)},
            {"Spacecraft Time (sub)",  "int", true, "", (double)LE32(b.RPT_SpacecraftTimeSubsec)},
            {"Reset Cause",            "int", true, "", (double)b.RPT_ResetCause},
            {"Cmd Counter",            "int", true, "", (double)b.RPT_CmdCounter},
            {"Err Counter",            "int", true, "", (double)b.RPT_ErrCounter}
        };
        if (upsert_group(sql, "FSW", "RPT", kv, ts)<0) return -1;
    }
    // ========= COMS / SANT =========
    {
        std::vector<KV> kv{
            {"status", "int", true, "", (double)b.SANT_status}
        };
        if (upsert_group(sql, "COMS", "SANT-AM2150O", kv, ts) < 0) return -1;
    }

    // ========= COMS / STRX (AX2150) =========
    {
        std::vector<KV> kv{
            {"last_rssi",   "int", true, "", (double)(int16_t)LE16((uint16_t)b.STRX_last_rssi)},
            {"bootcount",   "int", true, "", (double)LE16(b.STRX_bootcount)},
            {"bootcause",   "int", true, "", (double)LE32(b.STRX_bootcause)},
            {"rx_mode",     "int", true, "", (double)b.STRX_rx_mode},
            {"gnd_wdt_cnt", "int", true, "", (double)LE16(b.STRX_gnd_wdt_cnt)},
            {"gnd_wdt_left","int", true, "", (double)LE32(b.STRX_gnd_wdt_left)}
        };
        if (upsert_group(sql, "COMS", "STRX-AX2150", kv, ts) < 0) return -1;
    }

    // ========= COMS / UANT (ISIS) =========
    {
        std::vector<KV> kv{
            {"report_deploy", "int", true, "", (double)LE16(b.UANT_report_deploy)}
        };
        if (upsert_group(sql, "COMS", "UANT-ISIS", kv, ts) < 0) return -1;
    }

    // ========= COMS / UTX (AX100) =========
    {
        std::vector<KV> kv{
            {"active_conf", "int", true, "", (double)b.UTX_active_conf},
            {"boot_count",  "int", true, "", (double)LE16(b.UTX_boot_count)},
            {"boot_cause",  "int", true, "", (double)LE32(b.UTX_boot_cause)}
        };
        if (upsert_group(sql, "COMS", "UTX-AX100", kv, ts) < 0) return -1;
    }

    // ========= EPS / PCDU-P31U =========
    {
        std::vector<KV> kv;
        kv.push_back({"vbatt",        "int",   true, "", (double)LE16(b.EPS_vbatt)});
        push_idx_u8 (kv, "output",    b.EPS_output, 8);      // output_00..07
        push_idx_u16(kv, "curout",    b.EPS_curout, 6);      // curout_00..05 (mA 가정)
        push_idx_u16(kv, "curin",     b.EPS_curin,  3);      // curin_00..02
        kv.push_back({"cursys",       "int",   true, "", (double)LE16(b.EPS_cursys)});
        kv.push_back({"counter_boot", "int",   true, "", (double)LE16(b.EPS_counter_boot)});
        kv.push_back({"wdt_gnd_left", "int",   true, "", (double)LE32(b.EPS_wdt_gnd_time_left)});
        kv.push_back({"bootcause",    "int",   true, "", (double)b.EPS_bootcause});
        kv.push_back({"battmode",     "int",   true, "", (double)b.EPS_battmode});
        if (upsert_group(sql, "EPS", "PCDU-P31U", kv, ts) < 0) return -1;
    }

    // ========= EPS / SOLAR =========
    {
        std::vector<KV> kv{
            {"deploy", "int", true, "", (double)b.EPS_solar_panel_deploy}
        };
        if (upsert_group(sql, "EPS", "SOLAR", kv, ts) < 0) return -1;
    }

    // ========= ADCS / ADCS-MOD =========
    {
        std::vector<KV> kv{
            {"power",  "int",   true,  "", (double)b.ADCS_power_state},
            {"ctrl",   "int",   true,  "", (double)b.ADCS_control_mode},
            {"gyro_x", "float", true,  "", (double)b.ADCS_gyro0_cal_rate_x},
            {"gyro_y", "float", true,  "", (double)b.ADCS_gyro0_cal_rate_y},
            {"gyro_z", "float", true,  "", (double)b.ADCS_gyro0_cal_rate_z}
        };
        if (upsert_group(sql, "ADCS", "ADCS-MOD", kv, ts) < 0) return -1;
    }

    // ========= PAY / CAM =========
    {
        std::vector<KV> kv;
        kv.push_back({"mode", "int", true, "", (double)b.PAY_cam_mode});
        push_idx_u8(kv, "mem", b.PAY_cam_memory_status, 6);  // mem_00..05
        push_idx_u8(kv, "img", b.PAY_cam_image_number,  6);  // img_00..05
        if (upsert_group(sql, "PAY UZURO", "PAY_CAM", kv, ts) < 0) return -1;
    }

    // ========= PAY / DEORBIT =========
    {
        std::vector<KV> kv{
            {"adc_00", "int", true, "", (double)LE16(b.PAY_deorbit_adc[0])},
            {"adc_01", "int", true, "", (double)LE16(b.PAY_deorbit_adc[1])}
        };
        if (upsert_group(sql, "PAY UZURO", "PAY_DEORBIT", kv, ts) < 0) return -1;
    }

    // 필요 시 동일 패턴으로 COMS/EPS/ADCS/PAY 추가 가능

    std::printf("[beacon] Save2DB: inserted (created_at=%s UTC)\n", ts.c_str());
    return 0;
}

