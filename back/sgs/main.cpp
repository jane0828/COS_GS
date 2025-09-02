#include <stdio.h>
#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>
// #include <gts/gts.h>
// #include <ncs/ncs.h>
#include <ros/ros.h>



int main(int argc, char* argv[])
{
    MySQLDatabase mysqldb("localhost", "miman", "qwe123", "HYVRID");
    mysqldb.connect();

    SGS_WebSocketClient wsclient("gs634", "client634");
    
    wsclient.RegisterHandler("private", [&](const Json::Value& data) {
        // data == { "msgid": ..., "cc": ..., "parameters": [ {name,type,value}, ... ] }
        int msgid = data.get("msgid", 0).asInt();
        int cc    = data.get("cc", 0).asInt();
        int type  = data.get("type", 0).asInt();
        const Json::Value params = data["parameters"]; // 배열 없을 수도 있음

        auto tc = MakeTCData(msgid, cc, params);


        int pcount = (params.isArray() ? (int)params.size() : 0);

        // printf("[WS] received private: msgid=%d cc=%d params=%d\n", msgid, cc, pcount);

        printf("[WS] received private: msgid=%d cc=%d params=%d, tc_len=%zu\n", msgid, cc, params.isArray() ? (int)params.size() : 0, tc.size());

        for (int i = 0; i < pcount; ++i) {
            const auto& p = params[i];
            std::string name = p.get("name","").asString();
            std::string type = p.get("type","").asString();
            std::string val  = p.get("value","").asString(); // 숫자면 .asInt() 등으로 캐스팅
            printf("  - %s (%s) = %s\n", name.c_str(), type.c_str(), val.c_str());
        }

    // TODO: 여기서 실제 송신 버퍼 구성 → 장치로 전송
    // 1) msgid/cc/parameters를 바이너리로 패킹
    // 2) CSP/UDP 전송 루틴 호출 (예시: task_request_csp / uhftrx API 등)
    //
    // uint8_t buf[256]; int len = build_packet(msgid, cc, params, buf);
    // uint16_t replen = 0;
    // udpHandler->task_request_csp(node, port, buf, len, nullptr, &replen);
    });
    wsclient.Connect("ws://localhost:4443");

    // SGS_CSP cspclient(8);
    // SGS_GS100 uhftrx(&cspclient, "/dev/GS102", 500000);
    // uhftrx.register_handle(&wsclient);
    // cspclient.set_route_table(20, CSP_NO_VIA_ADDRESS);
    // cspclient.set_route_table(7, 20);
    // cspclient.set_route_table(28, 20);

    // // GS232B rotator;
    // SGS_KTA223* switchbox = new SGS_KTA223("/dev/KTA223", 115200);
    // // SGS_Q5Signal amp;

    // std::vector<SGS_TLE *> TleSource;
    // TleSource.push_back(new SGS_TLE("active"));
    // TleSource.push_back(new SGS_TLE("weather"));

    while(1);

    return 0;
}

// #include <iostream>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <cstdio>
// #include <cstring>
// #include <string>

// int main() {
//     std::string ip; int port;
//     printf("IP: "); fflush(stdout);
//     if (!(std::cin >> ip)) return 1;
//     printf("Port: "); fflush(stdout);
//     if (!(std::cin >> port)) return 1;

//     int fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (fd < 0) { perror("socket"); return 1; }

//     sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_port = htons((uint16_t)port);
//     if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1) { perror("inet_pton"); close(fd); return 1; }

//     if (connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0) { perror("connect"); close(fd); return 1; }

//     unsigned char buf[2] = {0x01, 0x01};
//     ssize_t n = send(fd, buf, sizeof(buf), 0);
//     if (n != (ssize_t)sizeof(buf)) { perror("send"); close(fd); return 1; }

//     printf("sent %zd bytes\n", n);
//     close(fd);
//     return 0;
// }
