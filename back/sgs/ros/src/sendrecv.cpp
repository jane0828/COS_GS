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

// static ssize_t recvall(int fd, void* buf, size_t len) {
//     uint8_t* p = static_cast<uint8_t*>(buf);
//     size_t got = 0;
//     while (got < len) {
//         ssize_t n = ::recv(fd, p + got, len - got, 0);
//         if (n <= 0) return n; // 0=FIN, <0=에러(EAGAIN 포함)
//         got += (size_t)n;
//     }
//     return (ssize_t)got;
// }

// int SendRecv::start_recv_tcp(std::atomic<bool>& run) {
//     int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
//     if (sfd < 0) { perror("socket(TCP)"); return -1; }

//     int opt = 1;
//     if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
//         perror("setsockopt(SO_REUSEADDR)"); ::close(sfd); return -1;
//     }

//     // 논블로킹 accept를 위해 O_NONBLOCK 설정
//     int flags = fcntl(sfd, F_GETFL, 0);
//     if (flags >= 0) fcntl(sfd, F_SETFL, flags | O_NONBLOCK);

//     sockaddr_in addr{};
//     addr.sin_family      = AF_INET;
//     addr.sin_port        = htons(BEACON_PORT);      // TCP 2001
//     addr.sin_addr.s_addr = inet_addr(BEACON_IP);    // "0.0.0.0"
//     if (::bind(sfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
//         perror("bind(TCP)"); ::close(sfd); return -1;
//     }
//     if (::listen(sfd, 8) < 0) {
//         perror("listen"); ::close(sfd); return -1;
//     }

//     std::printf("[recv] Listening TCP %s:%d\n", BEACON_IP, BEACON_PORT);
//     TelemetryHandler tmh;

//     while (run.load(std::memory_order_relaxed)) {
//         sockaddr_in cli{}; socklen_t clen = sizeof(cli);
//         int cfd = ::accept(sfd, (sockaddr*)&cli, &clen);
//         if (cfd < 0) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) { usleep(10000); continue; }
//             if (errno == EINTR) continue;
//             perror("accept");
//             continue;
//         }

//         // 클라이언트 소켓에 수신 타임아웃(500ms) 설정 → 중간에 run 플래그 체크 가능
//         timeval tv{ .tv_sec = 0, .tv_usec = 500000 };
//         setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

//         std::printf("[recv][tcp] connection from %s:%d\n",
//                     inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

//         for (;;) {
//             // 1) 길이(2바이트, Big-Endian)
//             uint16_t be_len = 0;
//             ssize_t n = recvall(cfd, &be_len, sizeof(be_len));
//             if (n <= 0) break; // 타임아웃/FIN/에러 → 연결 종료

//             uint16_t len = ntohs(be_len);
//             if (len == 0 || len > 2048) {
//                 std::fprintf(stderr, "[recv][tcp] bad frame length=%u\n", len);
//                 break;
//             }

//             // 2) payload 수신
//             uint8_t buf[2048];
//             n = recvall(cfd, buf, len);
//             if (n <= 0) break;

//             std::printf("[recv][tcp] from %s:%d size=%u\n",
//                         inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), len);
//             save_raw_txt_always(buf, len);

//             // Telemetry → 우선 파서에 위임
//             int rc = tmh.parse_and_dispatch(buf, len);
//             if (rc == 0) continue;

//             // Beacon 고정길이 프레임 수신 처리(기존 로직 유지)
//             if (len == sizeof(Beacon)) {
//                 Beacon b{};
//                 std::memcpy(&b, buf, sizeof(Beacon));
//                 BeaconHandler bh; bh.bec = &b;
//                 bh.parseBeaconData();
//                 continue;
//             }

//             std::fprintf(stderr, "[recv][tcp][warn] unknown frame (len=%u)\n", len);
//         }

//         ::close(cfd);
//         std::printf("[recv][tcp] connection closed\n");
//     }

//     ::close(sfd);
//     std::puts("[recv][tcp] loop stopped");
//     return 0;
// }


int SendRecv::start_recv(std::atomic<bool>& run) {
    if (init_socket(BEACON_IP, BEACON_PORT) < 0) return -1;
    std::printf("[recv] Listening UDP %s:%d\n", BEACON_IP, BEACON_PORT);

    uint8_t buf[2048];
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
        // Raw data save
        save_raw_txt_always(buf, (size_t)n);
        
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

