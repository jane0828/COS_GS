#ifndef _ROS_FSWCMD_H_
#define _ROS_FSWCMD_H_

#include <utl/utl.h>
#include "v670.h"

typedef struct {
    uint32_t idx;
    uint32_t size;
    std::string name;
}sgs_fswcmd_headerconfig_t;

class SGS_FSWCMD 
{
private:
    SGS_CONSOLE* console = NULL;

    void AssembleHead();
    void AssembleData();
    void ExportPacket();

public:
    bool Scheduled = false;
    bool Checksum = true;

    
    SGS_FSWCMD(uint8_t node, uint8_t reqport, uint8_t repport);
    ~SGS_FSWCMD();

    void RegisterPreHead(uint32_t idx, uint32_t size, std::string name);
    void RegisterPostHead(uint32_t idx, uint32_t size, std::string name);

    void RegisterCFEVersion(std::string version);

    void JsonCallback(const Json::Value& jsonData);


};
#endif