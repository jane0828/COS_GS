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

#include "../inc/ros/beacon.h"
#include "../inc/ros/sendrecv.h"
#include "../inc/ros/telemetry.h"

// #define BEACON_PORT 2001
// #define BEACON_IP   "0.0.0.0"

#define REMOTE_IP   "172.31.21.12"
#define REMOTE_PORT 2001


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

int SendRecv::start_recv_tcp(std::atomic<bool>& run) {
    TelemetryHandler tmh;

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
            save_raw_txt_always(acc.data(), acc.size());

            // 1) 텔레메트리 파싱
            int prc = tmh.parse_and_dispatch(acc.data(), acc.size());
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
