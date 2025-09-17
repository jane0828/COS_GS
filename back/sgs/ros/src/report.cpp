#include <ctime>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <endian.h>

#include "../inc/ros/report.h"
#include <string>

// ===== 엔디안 도우미 =====
static inline uint16_t LE16(uint16_t v) { return le16toh(v); }
static inline uint32_t LE32(uint32_t v) { return le32toh(v); }
static inline uint16_t BE16(uint16_t v) { return be16toh(v); }
static inline uint32_t BE32(uint32_t v) { return be32toh(v); }

static void dump_report(FILE* out, const Report& r, bool include_binary) {
    // CCSDS Header
    std::fprintf(out, "MsgID=0x%04X Seq=0x%04X Len=%u\n",
                 BE16(r.CCMessage_ID), BE16(r.CCSequence), BE16(r.CCLength));
    std::fprintf(out, "Tcode=%02X %02X %02X %02X %02X %02X Padding=0x%08X\n",
                 r.CCTime_code[0], r.CCTime_code[1], r.CCTime_code[2],
                 r.CCTime_code[3], r.CCTime_code[4], r.CCTime_code[5],
                 LE32(r.CCPadding));

    // Body
    std::fprintf(out, " msg_id=0x%04X cc=%u ret_type=%u\n",
                 LE16(r.msg_id), r.cc, r.ret_type);
    std::fprintf(out, " ret_code=%d ret_val_size=%u\n",
                 (int32_t)LE32(r.ret_code), LE16(r.ret_val_size));

    uint16_t size = LE16(r.ret_val_size);
    if (size == 2) {
        uint16_t val16 = 0;
        std::memcpy(&val16, r.ret_val, 2);
        val16 = le16toh(val16);
        std::fprintf(out, " ret_val=%u (HEX: ", val16);
        for (int i = 0; i < 2; i++) {
            std::fprintf(out, "%02X%s", r.ret_val[i], (i+1<2)?" ":"");
        }
        std::fprintf(out, ")\n");
    } else if (size == 4) {
        uint32_t val32 = 0;
        std::memcpy(&val32, r.ret_val, 4);
        val32 = le32toh(val32);
        std::fprintf(out, " ret_val=%u (HEX: ", val32);
        for (int i = 0; i < 4; i++) {
            std::fprintf(out, "%02X%s", r.ret_val[i], (i+1<4)?" ":"");
        }
        std::fprintf(out, ")\n");
    } else {
        std::fprintf(out, " ret_val=[");
        for (int i = 0; i < size && i < 126; i++) {
            std::fprintf(out, "%u%s", r.ret_val[i], (i+1<size)?", ":"");
        }
        std::fprintf(out, "] (HEX: ");
        for (int i = 0; i < size && i < 126; i++) {
            std::fprintf(out, "%02X%s", r.ret_val[i], (i+1<size)?" ":"");
        }
        std::fprintf(out, ")\n");
    }
                 


    if (include_binary) {
        std::fprintf(out, "\nBinary (%zu B):\n", sizeof(Report));
        const uint8_t* p = reinterpret_cast<const uint8_t*>(&r);
        for (size_t i=0; i<sizeof(Report); ++i)
            std::fprintf(out, "%02X%s", p[i], ((i+1)%16==0) ? "\n" : " ");
        std::fprintf(out, "\n");
    }
}

// ===== 클래스 구현 =====
ReportHandler::ReportHandler() {
    std::memset(ReportFileName, 0, sizeof(ReportFileName));
    std::snprintf(ReportFileName, sizeof(ReportFileName),
                  "report_log_%ld.dat", (long)time(nullptr));
}
ReportHandler::~ReportHandler() {
}

int ReportHandler::Save2File() {
    if (!rep) { std::fprintf(stderr, "Save2File: rep=null\n"); return -1; }

    struct stat st{};
    if (stat("./data", &st) != 0)
        if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return -1; }
    if (stat("./data/report", &st) != 0)
        if (mkdir("./data/report", 0755) != 0) { perror("mkdir ./data/report"); return -1; }

    time_t now = time(nullptr);
    struct tm *lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/report/Report--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);

    FILE *fp = std::fopen(path, "w");
    if (!fp) { perror(path); return -1; }

    dump_report(fp, *rep, /*include_binary=*/true);
    std::fclose(fp);
    std::printf("[report] Saved to %s\n", path);
    return 0;
}

int ReportHandler::Save2DB()  { std::printf("[report] Save2DB: (not implemented)\n"); return 0; }
int ReportHandler::File2DB()  { std::printf("[report] File2DB: (not implemented)\n"); return 0; }

std::string report_summary(const Report& r) {
    char line[256];
    std::snprintf(line, sizeof(line),
        "MsgID=0x%04X Seq=0x%04X Len=%u ret_code=%d size=%u",
        BE16(r.CCMessage_ID), BE16(r.CCSequence), BE16(r.CCLength),
        (int32_t)LE32(r.ret_code), LE16(r.ret_val_size));
    return std::string(line);
}

void ReportHandler::parseReportData() {
    if (!rep) { std::fprintf(stderr, "parseReportData: rep=null\n"); return; }
    (void)Save2File();
    // (void)Save2DB();
}
