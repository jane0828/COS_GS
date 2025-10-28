#include <atomic>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <endian.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <ctime>
#include <fcntl.h>
#include <vector>
#include <unordered_map>
#include <algorithm> 
#include <string>

#include "../inc/ros/beacon.h"
#include "../inc/ros/sendrecv.h"
#include "../inc/ros/telemetry.h"
#include "../inc/ros/bitstream.h"
#include "../inc/ros/image_extract.h"

// #define BEACON_PORT 2001
// #define BEACON_IP   "0.0.0.0"

// #define REMOTE_IP   "192.168.4.25"
#define REMOTE_IP   "127.0.0.1"
#define REMOTE_PORT 2001

static Bitstream g_bitstream;

static void write_image_report(const ImageAssemblyReport& rpt,
                               size_t expected_payload_len_per_pkt) {
    // ./data/report 디렉터리 보장
    struct stat st{};
    if (stat("./data", &st) != 0) { if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return; } }
    if (stat("./data/report", &st) != 0) { if (mkdir("./data/report", 0755) != 0) { perror("mkdir ./data/report"); return; } }

    // 타임스탬프 파일명
    time_t now = time(nullptr);
    struct tm* lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/report/image-report--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);

    FILE* fp = std::fopen(path, "w");
    if (!fp) { perror(path); return; }

    // [SESSION]
    std::fprintf(fp, "[SESSION]\n");
    std::fprintf(fp, "file_id        : %s\n", rpt.file_id.empty() ? "(unknown)" : rpt.file_id.c_str());
    std::fprintf(fp, "mode           : %s\n", rpt.expedite_present ? "EXPEDITE_PRESENT" : "STREAM");
    std::fprintf(fp, "recv_time      : %04d-%02d-%02d %02d:%02d:%02d\n\n",
                 lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                 lt->tm_hour, lt->tm_min, lt->tm_sec);
    std::fprintf(fp, "raw_source     : %s\n",
             rpt.source_raw_path.empty() ? "(unknown)" : rpt.source_raw_path.c_str());
    std::fprintf(fp, "output_image   : %s\n\n",
             rpt.output_image_path.empty() ? "(unknown)" : rpt.output_image_path.c_str());

    // [RECEIVED]
    std::fprintf(fp, "[RECEIVED]\n");
    std::fprintf(fp, "packets_ok     : %zu\n", rpt.packets_ok);
    std::fprintf(fp, "total_bytes    : %zu\n", rpt.total_image_bytes);

    // seq -> received bytes 매핑
    std::unordered_map<uint16_t, size_t> len_by_seq;
    for (size_t i = 0; i < rpt.seq_seen.size() && i < rpt.rx_bytes.size(); ++i) {
        len_by_seq[rpt.seq_seen[i]] = rpt.rx_bytes[i];
    }

    if (!rpt.seq_seen.empty()) {
        std::vector<uint16_t> s = rpt.seq_seen;
        std::sort(s.begin(), s.end());
        s.erase(std::unique(s.begin(), s.end()), s.end());
        uint16_t start = s.front();
        uint16_t end   = s.back();

        size_t ordinal = 1;
        for (uint16_t seq = start; seq <= end; ++seq, ++ordinal) {
            size_t got = 0;
            auto it = len_by_seq.find(seq);
            if (it != len_by_seq.end()) got = it->second;
            std::fprintf(fp, "packet #%zu - Received : %zu\n",
                         ordinal, got);
        }
    }
    std::fprintf(fp, "\n");

    // [SEQUENCE]
    std::fprintf(fp, "[SEQUENCE]\n");
    std::fprintf(fp, "flags_present  : FIRST=%s, CONT=%s, LAST=%s\n",
                 rpt.has_first ? "Yes" : "No",
                 rpt.has_cont  ? "Yes" : "No",
                 rpt.has_last  ? "Yes" : "No");
    std::fprintf(fp, "continuity     : %s", rpt.continuity_ok ? "OK (no gap/dup)" : "NG (gap)");
    if (!rpt.missing_seq.empty()) {
        std::fprintf(fp, " — missing seq: ");
        for (size_t i = 0; i < rpt.missing_seq.size(); ++i) {
            std::fprintf(fp, (i + 1 == rpt.missing_seq.size()) ? "%u" : "%u,", rpt.missing_seq[i]);
        }
    }
    std::fprintf(fp, "\n\n");

    // [JPEG]
    std::fprintf(fp, "[JPEG]\n");
    std::fprintf(fp, "SOI/EOI        : %s (FFD8 ... FFD9)\n",
                 (rpt.soi_ok && rpt.eoi_ok) ? "OK" : "NG");
    std::fprintf(fp, "\n");

    // [REREQUEST]
    std::fprintf(fp, "[REREQUEST]\n");
    if (rpt.missing_seq.empty()) {
        std::fprintf(fp, "missing_packets: (none)\n");
        std::fprintf(fp, "byte_ranges    : (none)\n");
    } else {
        std::fprintf(fp, "missing_packets: ");
        for (size_t i = 0; i < rpt.missing_seq.size(); ++i) {
            uint16_t m = rpt.missing_seq[i];
            std::fprintf(fp, (i + 1 == rpt.missing_seq.size()) ? "%u (0x%04X)" : "%u (0x%04X),", m, m);
        }
        std::fprintf(fp, "\n");
        if (rpt.first_seq_valid && rpt.expected_payload_len_per_pkt > 0) {
            std::fprintf(fp, "byte_ranges    : ");
            bool first_printed = false;
            for (uint16_t m : rpt.missing_seq) {
                // 14-bit 롤오버 대비한 거리 계산
                uint16_t dist = (uint16_t)((m - rpt.first_seq) & 0x3FFF);
                size_t start  = (size_t)dist * rpt.expected_payload_len_per_pkt;
                size_t len    = rpt.expected_payload_len_per_pkt;
                if (first_printed) std::fprintf(fp, ", ");
                std::fprintf(fp, "[%zu - %zu]", start, start + len - 1);
                first_printed = true;
            }
            std::fprintf(fp, "\n");
        } else {
            std::fprintf(fp, "byte_ranges    : (need FIRST and fixed payload size)\n");
        }
    }
    std::fprintf(fp, "\n");

    std::fclose(fp);
    std::printf("[report] saved to %s\n", path);
}


static std::string save_raw_txt_always(const uint8_t* buf, size_t len) {
    std::string out_path;
    if (!buf || len == 0) return {};
    struct stat st{};
    if (stat("./data", &st) != 0) {
        if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return {}; }
    }
    if (stat("./data/raw", &st) != 0) {
        if (mkdir("./data/raw", 0755) != 0) { perror("mkdir ./data/raw"); return {}; }
    }
    time_t now = time(nullptr);
    struct tm* lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/raw/raw--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);
    FILE* fp = std::fopen(path, "w");
    if (!fp) { perror(path); return {}; }
    std::fprintf(fp, "Raw dump (%zu bytes)\n", len);
    for (size_t i = 0; i < len; ++i) {
        std::fprintf(fp, "%02X%s", buf[i], ((i + 1) % 16 == 0) ? "\n" : " ");
    }
    std::fprintf(fp, "\n");
    std::fclose(fp);

    out_path = path;
    std::printf("[recv][raw] saved to %s\n", path);

    return out_path;
}


SendRecv::SendRecv() : sock_fd(0) {}
SendRecv::~SendRecv() { if (sock_fd > 0) ::close(sock_fd); }

int SendRecv::start_recv_tcp(std::atomic<bool>& run) {
    TelemetryHandler tmh;
    {
        Bitstream::Options opt;
        opt.image_debug_mode = true;
        opt.base_dir = "./data/image";
        opt.prefix   = "image-";
        opt.ext      = ".jpg";     // 파일명 확장자만 .jpg, 내용은 raw bitstream
        g_bitstream.set_options(opt);
    }

    while (run.load(std::memory_order_relaxed)) {
        int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sfd < 0) { perror("socket(TCP)"); sleep(1); continue; }

        // 커널 Keepalive 켜두면 장시간 유휴 시 끊김 탐지에 유리
        int ka = 1;
        setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &ka, sizeof(ka));

        // (선택) 연결 타임아웃 구현: 논블로킹 connect + select
        int flags = fcntl(sfd, F_GETFL, 0);
        if (flags >= 0) fcntl(sfd, F_SETFL, flags | O_NONBLOCK);

        sockaddr_in raddr{};
        raddr.sin_family      = AF_INET;
        raddr.sin_port        = htons(REMOTE_PORT);
        raddr.sin_addr.s_addr = inet_addr(REMOTE_IP);

        int rc = ::connect(sfd, (sockaddr*)&raddr, sizeof(raddr));
        if (rc < 0 && errno == EINPROGRESS) {
            fd_set wfds; FD_ZERO(&wfds); FD_SET(sfd, &wfds);
            timeval tv{ .tv_sec = 5, .tv_usec = 0 }; // 5초 연결 타임아웃
            rc = select(sfd + 1, nullptr, &wfds, nullptr, &tv);
            if (rc > 0) {
                int err=0; socklen_t len=sizeof(err);
                if (getsockopt(sfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0 || err != 0) rc = -1;
                else rc = 0;
            } else rc = -1;
        } else if (rc == 0) {
            // 즉시 연결됨
        }

        if (rc != 0) {
            std::fprintf(stderr, "[recv][tcp] connect %s:%d failed: %s\n",
                         REMOTE_IP, REMOTE_PORT, std::strerror(errno));
            ::close(sfd);
            usleep(500*1000); // 0.5초 후 재시도
            continue;
        }

        // 블로킹으로 되돌리고 수신 타임아웃 설정(수신 정지/지연 대비)
        if (flags >= 0) fcntl(sfd, F_SETFL, flags & ~O_NONBLOCK);
        timeval rtv{ .tv_sec = 2, .tv_usec = 0 };
        setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &rtv, sizeof(rtv));

        std::printf("[recv][tcp] connected to %s:%d\n", REMOTE_IP, REMOTE_PORT);

        // 스트리밍 수신 루프
        std::vector<uint8_t> acc; acc.reserve(8192);
        for (;;) {
            uint8_t buf[4096];
            ssize_t n = ::recv(sfd, buf, sizeof(buf), 0);
            if (n > 0) {
                acc.insert(acc.end(), buf, buf + n);
                // 여기서 조각 단위로 프레이밍/파싱을 해도 되고,
                // 상대가 주기적으로 끊지 않는다면 일정 크기마다 처리하도록 바꿔도 됩니다.
                continue;
            }
            if (n == 0) {
                // peer closed
                std::puts("[recv][tcp] peer closed");
                break;
            }
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                // 타임아웃: 누적 있으면 한 번 처리
                if (!acc.empty()) break;
                else continue;
            }
            perror("recv");
            break;
        }

        if (!acc.empty()) {
            std::printf("[recv][tcp] total bytes=%zu\n", acc.size());
            std::string raw_path = save_raw_txt_always(acc.data(), acc.size());

            // 1) 텔레메트리 파싱
            int prc = tmh.parse_stream(acc.data(), acc.size());
            // int prc = tmh.parse_and_dispatch(acc.data(), acc.size());
            if (prc != 0) {
                // 2) Beacon 길이 동일 시 Beacon 파싱 시도
                if (acc.size() == sizeof(Beacon)) {
                    Beacon b{};
                    std::memcpy(&b, acc.data(), sizeof(Beacon));
                    BeaconHandler bh; bh.bec = &b;
                    bh.parseBeaconData();
                } else {
                    std::fprintf(stderr, "[recv][tcp][warn] unknown frame (len=%zu)\n", acc.size());
                }
            }
            ImageAssemblyConfig cfg;
            cfg.image_apid =  0x078;   // 예시 값 → 실제 값으로!
            cfg.sec_hdr_len = 10; // 예시 → 장비 포맷에 맞춰 조정

            ImageAssemblyReport rpt;
            bool ok = assemble_image_from_acc(acc, cfg, g_bitstream, &rpt,raw_path);     

            // write_image_report(rpt, /*expected_payload_len_per_pkt=*/128);

        }

        ::close(sfd);
        std::puts("[recv][tcp] disconnected, retrying soon...");
        usleep(500*1000); // 0.5초 후 자동 재연결
    }

    std::puts("[recv][tcp] loop stopped");
    return 0;
}

int SendRecv::start_recv(std::atomic<bool>& run) {
    std::fprintf(stderr, "[recv][udp] disabled in this build\n");
    while (run.load(std::memory_order_relaxed)) { usleep(100000); }
    return 0;
}
