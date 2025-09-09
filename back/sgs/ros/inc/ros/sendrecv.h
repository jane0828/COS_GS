#pragma once
#include <atomic>
#include <netinet/in.h>

class SendRecv {
public:
    SendRecv();
    ~SendRecv();

    int init_socket(const char* ip, int port); // UDP 바인드
    int start_recv(std::atomic<bool>& run);
    // int start_recv_tcp(std::atomic<bool>& run);

private:
    int         sock_fd;
    sockaddr_in server_addr{};
    sockaddr_in client_addr{};
};
