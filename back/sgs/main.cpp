// // #include <stdio.h>
// // #include <utl/utl.h>
// // #include <ifc/ifc.h>
// // #include <dcs/dcs.h>
// // // #include <gts/gts.h>
// // // #include <ncs/ncs.h>
// // #include <ros/ros.h>



// // int main(int argc, char* argv[])
// // {
// //     MySQLDatabase mysqldb("localhost", "miman", "qwe123", "HYVRID");
// //     mysqldb.connect();

// //     SGS_WebSocketClient wsclient("gs634", "client634");
    
// //     wsclient.RegisterHandler("private", [&](const Json::Value& data) {
// //         // data == { "msgid": ..., "cc": ..., "parameters": [ {name,type,value}, ... ] }
// //         int msgid = data.get("msgid", 0).asInt();
// //         int cc    = data.get("cc", 0).asInt();
// //         int type  = data.get("type", 0).asInt();
// //         const Json::Value params = data["parameters"]; // 배열 없을 수도 있음

// //         auto tc = MakeTCData(data);


// //         int pcount = (params.isArray() ? (int)params.size() : 0);

// //         // printf("[WS] received private: msgid=%d cc=%d params=%d\n", msgid, cc, pcount);

// //         printf("[WS] received private: msgid=%d cc=%d params=%d, tc_len=%zu\n", msgid, cc, params.isArray() ? (int)params.size() : 0, tc.size());

// //         for (int i = 0; i < pcount; ++i) {
// //             const auto& p = params[i];
// //             std::string name = p.get("name","").asString();
// //             std::string type = p.get("type","").asString();
// //             std::string val  = p.get("value","").asString(); // 숫자면 .asInt() 등으로 캐스팅
// //             printf("  - %s (%s) = %s\n", name.c_str(), type.c_str(), val.c_str());
// //         }

// //     // TODO: 여기서 실제 송신 버퍼 구성 → 장치로 전송
// //     // 1) msgid/cc/parameters를 바이너리로 패킹
// //     // 2) CSP/UDP 전송 루틴 호출 (예시: task_request_csp / uhftrx API 등)
// //     //
// //     // uint8_t buf[256]; int len = build_packet(msgid, cc, params, buf);
// //     // uint16_t replen = 0;
// //     // udpHandler->task_request_csp(node, port, buf, len, nullptr, &replen);
// //     });
// //     wsclient.Connect("ws://localhost:4443");

// //     // SGS_CSP cspclient(8);
// //     // SGS_GS100 uhftrx(&cspclient, "/dev/GS102", 500000);
// //     // uhftrx.register_handle(&wsclient);
// //     // cspclient.set_route_table(20, CSP_NO_VIA_ADDRESS);
// //     // cspclient.set_route_table(7, 20);
// //     // cspclient.set_route_table(28, 20);

// //     // // GS232B rotator;
// //     // SGS_KTA223* switchbox = new SGS_KTA223("/dev/KTA223", 115200);
// //     // // SGS_Q5Signal amp;

// //     // std::vector<SGS_TLE *> TleSource;
// //     // TleSource.push_back(new SGS_TLE("active"));
// //     // TleSource.push_back(new SGS_TLE("weather"));

// //     while(1);

// //     return 0;
// // }

// // // #include <iostream>
// // // #include <arpa/inet.h>
// // // #include <netinet/in.h>
// // // #include <sys/socket.h>
// // // #include <unistd.h>
// // // #include <cstdio>
// // // #include <cstring>
// // // #include <string>

// // // int main() {
// // //     std::string ip; int port;
// // //     printf("IP: "); fflush(stdout);
// // //     if (!(std::cin >> ip)) return 1;
// // //     printf("Port: "); fflush(stdout);
// // //     if (!(std::cin >> port)) return 1;

// // //     int fd = socket(AF_INET, SOCK_STREAM, 0);
// // //     if (fd < 0) { perror("socket"); return 1; }

// // //     sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_port = htons((uint16_t)port);
// // //     if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1) { perror("inet_pton"); close(fd); return 1; }

// // //     if (connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0) { perror("connect"); close(fd); return 1; }

// // //     unsigned char buf[2] = {0x01, 0x01};
// // //     ssize_t n = send(fd, buf, sizeof(buf), 0);
// // //     if (n != (ssize_t)sizeof(buf)) { perror("send"); close(fd); return 1; }

// // //     printf("sent %zd bytes\n", n);
// // //     close(fd);
// // //     return 0;
// // // }




// #include <stdio.h>
// #include <thread>
// #include <chrono>
// #include <json/json.h>

// #include <utl/utl.h>
// #include <ifc/ifc.h>
// #include <dcs/dcs.h>
// // #include <gts/gts.h>
// // #include <ncs/ncs.h>
// #include <ros/ros.h>

// // --- MakeTCData 선언(프로토타입) ---
// void MakeTCData(const Json::Value& root);

// int main(int argc, char* argv[])
// {
//     MySQLDatabase mysqldb("localhost", "miman", "qwe123", "HYVRID");
//     mysqldb.connect();

//     SGS_WebSocketClient wsclient("gs634", "client634");

//     wsclient.RegisterHandler("private", [&](const Json::Value& data) {
//         // data 또는 data.message 모두 지원 (MakeTCData도 래퍼 지원)
//         const Json::Value& m = data.isMember("message") ? data["message"] : data;

//         int msgid = m.get("msgid", 0).asInt();
//         int cc    = m.get("cc", 0).asInt();
//         // int type  = m.get("type", 0).asInt();  // 지금 안 쓰면 주석/제거
//         const Json::Value params = m["parameters"]; // 배열 없을 수도 있음

//         // 패킷 생성 + 헥사 프린트 (전송 미구현)
//         MakeTCData(data);

//         int pcount = params.isArray() ? (int)params.size() : 0;
//         std::printf("[WS] received private: msgid=%d cc=%d params=%d\n", msgid, cc, pcount);

//         for (int i = 0; i < pcount; ++i) {
//             const auto& p = params[i];
//             std::string name      = p.get("name","").asString();
//             std::string paramType = p.get("type","").asString();   // ← 변수명 변경(충돌 회피)
//             std::string val       = p.get("value","").asString();
//             std::printf("  - %s (%s) = %s\n", name.c_str(), paramType.c_str(), val.c_str());
//         }
//     });

//     wsclient.Connect("ws://localhost:4443");

//     // 과도한 busy loop 방지
//     while (true) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(100));
//     }
//     return 0;
// }


// ===== main.cpp =====
// WebSocket으로 JSON 받으면 MakeTCData 호출만 해도
// 내부CMD 생성 → Encapsulate → TCP 전송까지 수행

#include <atomic>
#include <stdio.h>
#include <thread>
#include <csignal>
#include <chrono>
#include <json/json.h>

#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>
// #include <gts/gts.h>
// #include <ncs/ncs.h>
#include "./ros/inc/ros/sendrecv.h"

// MakeTCData 외부 함수 선언
void MakeTCData(const Json::Value& root);

static std::atomic<bool> g_run(true);
static void on_signal(int) {
    g_run.store(false, std::memory_order_relaxed);
}

static void parse_args(int argc, char* argv[], std::string& ip, int& port) {
    // 기본값 설정
    ip = "192.168.4.25";
    port = 2001;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-i" || arg == "--ip") && i + 1 < argc) {
            ip = argv[++i];
        } else if ((arg == "-p" || arg == "--port") && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "-h" || arg == "--help") {
            printf("Usage: %s [-i <ip>] [-p <port>]\n", argv[0]);
            printf("Default: -i 192.168.4.25 -p 2001\n");
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    std::signal(SIGINT,  on_signal);
    std::signal(SIGTERM, on_signal);

    std::string ip;
    int port = 0;
    parse_args(argc, argv, ip, port);

    printf("[CONFIG] Target IP: %s\n", ip.c_str());
    printf("[CONFIG] Target Port: %d\n", port);

    SendRecv srv;
    srv.set_target(ip, port);
    std::thread t_tcp([&]{ srv.start_recv_tcp(g_run); });


    MySQLDatabase mysqldb("localhost", "aclgs", "qwe123", "cosmic");
    mysqldb.connect();

    SGS_WebSocketClient wsclient("gs634", "client634");

    wsclient.RegisterHandler("private", [&](const Json::Value& data) {
        const Json::Value& m = data.isMember("message") ? data["message"] : data;

        int msgid = m.get("msgid", 0).asInt();
        int cc    = m.get("cc", 0).asInt();
        const Json::Value params = m["parameters"];

        // 패킷 생성 + Encapsulate + TCP 전송(내부에서 수행)
        MakeTCData(data);

        int pcount = params.isArray() ? (int)params.size() : 0;
        std::printf("[WS] received private: msgid=%d cc=%d params=%d\n", msgid, cc, pcount);

        for (int i = 0; i < pcount; ++i) {
            const auto& p = params[i];
            std::string name      = p.get("name","").asString();
            std::string paramType = p.get("type","").asString();
            std::string val       = p.get("value","").asString();
            std::printf("  - %s (%s) = %s\n", name.c_str(), paramType.c_str(), val.c_str());
        }
    });

    wsclient.Connect("ws://localhost:4443");

    while (g_run.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (t_tcp.joinable()) t_tcp.join();
    return 0;

}
