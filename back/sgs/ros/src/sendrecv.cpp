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

#include "../inc/ros/beacon.h"
#include "../inc/ros/sendrecv.h"
#include "../inc/ros/telemetry.h"

#define BEACON_PORT 2001
#define BEACON_IP   "0.0.0.0"

static void save_raw_txt_always(const uint8_t* buf, size_t len) {
    if (!buf || len == 0) return;
    struct stat st{};
    if (stat("./data", &st) != 0) {
        if (mkdir("./data", 0755) != 0) { perror("mkdir ./data"); return; }
    }
    if (stat("./data/raw", &st) != 0) {
        if (mkdir("./data/raw", 0755) != 0) { perror("mkdir ./data/raw"); return; }
    }
    time_t now = time(nullptr);
    struct tm* lt = localtime(&now);
    char path[256];
    std::snprintf(path, sizeof(path),
                  "./data/raw/raw--%04d-%02d-%02d-%02d-%02d-%02d.txt",
                  lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                  lt->tm_hour, lt->tm_min, lt->tm_sec);
    FILE* fp = std::fopen(path, "w");
    if (!fp) { perror(path); return; }
    std::fprintf(fp, "Raw dump (%zu bytes)\n", len);
    for (size_t i = 0; i < len; ++i) {
        std::fprintf(fp, "%02X%s", buf[i], ((i + 1) % 16 == 0) ? "\n" : " ");
    }
    std::fprintf(fp, "\n");
    std::fclose(fp);
    std::printf("[recv][raw] saved to %s\n", path);
}


SendRecv::SendRecv() : sock_fd(0) {}
SendRecv::~SendRecv() { if (sock_fd > 0) ::close(sock_fd); }

int SendRecv::init_socket(const char* ip, int port) {
    sock_fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) { perror("socket"); return -1; }

    int opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR)"); ::close(sock_fd); return -1;
    }
    timeval tv{ .tv_sec = 0, .tv_usec = 500000 }; // 500ms timeout
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt(SO_RCVTIMEO)"); ::close(sock_fd); return -1;
    }

    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (::bind(sock_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); ::close(sock_fd); return -1;
    }
    return sock_fd;
}

int SendRecv::start_recv(std::atomic<bool>& run) {
    if (init_socket(BEACON_IP, BEACON_PORT) < 0) return -1;
    std::printf("[recv] Listening UDP %s:%d\n", BEACON_IP, BEACON_PORT);

    uint8_t buf[2048];
    // Beacon  beacon{};
    // BeaconHandler handler;
    // handler.bec = &beacon;
    socklen_t client_len = sizeof(client_addr);
    TelemetryHandler tmh;

    while (run.load(std::memory_order_relaxed)) {
        const ssize_t n = ::recvfrom(sock_fd, buf, sizeof(buf), 0,
                                     (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) continue;
            perror("recvfrom");
            continue;
        }
        if (n == 0) continue;
        
        std::printf("[recv] from %s:%d size=%zd\n",
                    inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port), n);

        save_raw_txt_always(buf, (size_t)n);
        
        if ((size_t)n >= sizeof(CSPHeader) + 2) {
            const auto* tf = reinterpret_cast<const TelemetryFrame*>(buf);
            const size_t need = sizeof(CSPHeader) + 2 + tf->payload_len;
            if ((size_t)n >= need && tf->payload_len <= TM_MAX_PAYLOAD) {
                // payload 첫 2바이트(CCMessage_ID)로 타입 판별
                uint16_t cc = 0; std::memcpy(&cc, tf->payload, 2);
                // 엔디안 정책에 맞게 필요시 ntohs(cc) 사용
                cc = le16toh(cc);
                if (cc == 0xBEAC) {
                    Beacon b{};
                    if (tf->payload_len >= sizeof(Beacon)) {
                        std::memcpy(&b, tf->payload, sizeof(Beacon));
                        BeaconHandler bh; bh.bec = &b;
                        bh.parseBeaconData();
                        continue;
                    }
                }
                // TODO: HK 등 다른 CCMessage_ID 분기
            }
        }
        // Telemetry parsing first
        int rc = tmh.parse_and_dispatch(buf, (size_t)n);
        if (rc == 0) continue;

        if ((size_t)n == sizeof(Beacon)) {
            Beacon b{};
            std::memcpy(&b, buf, sizeof(Beacon));
            BeaconHandler bh; 
            bh.bec = &b;
            bh.parseBeaconData();
            continue;
        }

        std::fprintf(stderr, "[recv][warn] unknown packet (len=%zd)\n", n);
    }
    std::puts("[recv] loop stopped");
    return 0;
}

