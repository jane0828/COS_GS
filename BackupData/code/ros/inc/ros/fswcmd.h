// #ifndef _ROS_FSWCMD_H_
// #define _ROS_FSWCMD_H_

// #include <utl/utl.h>
// #include "v670.h"

// typedef struct {
//     uint32_t idx;
//     uint32_t size;
//     std::string name;
// }sgs_fswcmd_headerconfig_t;

// class SGS_FSWCMD 
// {
// private:
//     SGS_CONSOLE* console = NULL;

//     void AssembleHead();
//     void AssembleData();
//     void ExportPacket();

// public:
//     bool Scheduled = false;
//     bool Checksum = true;

    
//     SGS_FSWCMD(uint8_t node, uint8_t reqport, uint8_t repport);
//     ~SGS_FSWCMD();

//     void RegisterPreHead(uint32_t idx, uint32_t size, std::string name);
//     void RegisterPostHead(uint32_t idx, uint32_t size, std::string name);

//     void RegisterCFEVersion(std::string version);

//     void JsonCallback(const Json::Value& jsonData);


// };
// #endif


#ifndef _ROS_FSW_H_
#define _ROS_FSW_H_

#include <cstdint>
#include <string>
#include <vector>
#include <json/json.h>   // ← MakeTCData가 Json::Value를 받으니 헤더에서 필요
#include <utl/utl.h>
#include "v670.h"

typedef struct {
    uint32_t idx;
    uint32_t size;
    std::string name;
} sgs_fswcmd_headerconfig_t;

class SGS_FSWCMD
{
private:
    SGS_CONSOLE* console = nullptr;

    void AssembleHead();
    void AssembleData();
    void ExportPacket();

    // ---- 내부 헬퍼들(구현은 .cpp에) ----
    static bool packOneByType(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload);
    static bool packFromParamList(const Json::Value& params, std::vector<uint8_t>& payload);
    static std::vector<uint8_t> buildCmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload);

public:
    bool Scheduled = false;
    bool Checksum = true;

    SGS_FSWCMD(uint8_t node, uint8_t reqport, uint8_t repport);
    ~SGS_FSWCMD();

    void RegisterPreHead(uint32_t idx, uint32_t size, std::string name);
    void RegisterPostHead(uint32_t idx, uint32_t size, std::string name);
    void RegisterCFEVersion(std::string version);

    // ==== 여기 추가: 패킷 생성/전송 API ====
    // parameters: [{name,type,value}, ...] 을 읽어 payload 생성 → 헤더+체크섬 붙여 완성된 TC 반환
    static std::vector<uint8_t> MakeTCData(int msgid, int cc, const Json::Value& parameters);

    // 구버전 호환(외부에서 type 정수 전달하더라도 무시하고 위 제네릭 경로 사용)
    static std::vector<uint8_t> MakeTCData(int msgid, int cc, int type, const Json::Value& parameters);

    // 전송기(구현은 .cpp — 헤더에 네트워크 헤더 포함 안 함)
    static bool SendTCP(const std::string& ip, int port, const std::vector<uint8_t>& buf);
    static bool SendUDP(const std::string& ip, int port, const std::vector<uint8_t>& buf);

    // JSON 한 벌로 → 빌드+전송까지 (message 래퍼/ Proto:"udp" 지원). 성공 시 보낸 바이트 수, 실패 시 -1
    static int BuildAndSendFromJson(const Json::Value& jsonData);

    // 네가 원래 쓰던 콜백 — 여기서 위 API들을 호출
    void JsonCallback(const Json::Value& jsonData);
};

void MakeTCData(const Json::Value& root);

#endif
