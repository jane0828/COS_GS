#ifndef _DCS_KTA223_H_
#define _DCS_KTA223_H_

#include <utl/utl.h>
#include <ifc/ifc.h>

class SGS_KTA223 : public SGS_RS232 {
private:
    SGS_CONSOLE* console = NULL;
    SGS_WebSocketClient* ws = NULL;
    
    char buf[64];
public:
    SGS_KTA223(const char* _devname, uint32_t baud);
    ~SGS_KTA223();
    int register_handle(SGS_WebSocketClient* _ws);
    int switch_to_rx();
    int switch_to_rx_json(const Json::Value& data);
    int switch_to_tx();
    int switch_to_tx_json(const Json::Value& data);
};

#endif