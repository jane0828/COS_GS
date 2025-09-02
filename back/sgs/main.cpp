#include <stdio.h>
#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>
#include <gts/gts.h>
// #include <ncs/ncs.h>
// #include <ros/ros.h>

int main(int argc, char* argv[])
{
    MySQLDatabase mysqldb("localhost", "miman", "qwe123", "HYVRID");
    mysqldb.connect();

    SGS_WebSocketClient wsclient("gs634", "client634");
    
    wsclient.RegisterHandler("private", [&](const Json::Value& data) {
        // data == { "msgid": ..., "cc": ..., "parameters": [ {name,type,value}, ... ] }
        int msgid = data.get("msgid", 0).asInt();
        int cc    = data.get("cc", 0).asInt();

        const Json::Value params = data["parameters"]; // 배열 없을 수도 있음
        int pcount = (params.isArray() ? (int)params.size() : 0);

        printf("[WS] received private: msgid=%d cc=%d params=%d\n", msgid, cc, pcount);
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

    SGS_CSP cspclient(8);
    SGS_GS100 uhftrx(&cspclient, "/dev/GS102", 500000);
    uhftrx.register_handle(&wsclient);
    cspclient.set_route_table(20, CSP_NO_VIA_ADDRESS);
    cspclient.set_route_table(7, 20);
    cspclient.set_route_table(28, 20);

    // GS232B rotator;
    SGS_KTA223* switchbox = new SGS_KTA223("/dev/KTA223", 115200);
    // SGS_Q5Signal amp;

    std::vector<SGS_TLE *> TleSource;
    TleSource.push_back(new SGS_TLE("active"));
    TleSource.push_back(new SGS_TLE("weather"));

    while(1);

    return 0;
}