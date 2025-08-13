#ifndef _DCS_GS232B_H_
#define _DCS_GS232B_H_
#include <pthread.h>
#include <utl/utl.h>
#include <ifc/ifc.h>
#include <inttypes.h>

class GS232B : public SGS_RS232
{
public:
    bool HANDLE[16];
    pthread_t THREAD[16];
    SGS_CONSOLE* console = NULL;
    char ControlBuf[32];
    char ReadBuf[32];
    
    bool RotatorReady = false;
    bool RotatorReading = false;
    bool RotatorEngage = false;
    uint16_t Rotator_baud;

    int az_actual;
    int el_actual;
    int az_target;
    int el_target;

    int tolrnc_az = 3;
    int tolrnc_el = 3;

    int offset_az = 0;
    int offset_el = 0;
    int offset_tm = 3;

    int byte = 0;
    int read_count = 0;

    SGS_WebSocketClient* ws = NULL;

    GS232B(const char* _devname, uint32_t _baud);
    ~GS232B();
    int register_handle(SGS_WebSocketClient* _ws);
    int set_angle(int azimuth, int elevation);
    int set_angle_json(const Json::Value& data);
    int get_angle();
    int get_angle_json(const Json::Value& data);
};

#endif