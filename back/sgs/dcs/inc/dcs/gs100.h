#ifndef _DCS_GS100_H_
#define _DCS_GS100_H_
#include <gs/csp/csp.h>
#include <csp/csp_endian.h>
#include <gs/param/rparam.h>
#include <gs/param/table.h>
#include <gs/param/types.h>
#define GS_PARAM_INTERNAL_USE 1
#include <gs/param/internal/types.h>
#include <inttypes.h>
#include <utl/utl.h>
#include <ifc/ifc.h>

#define GS100_DEFAULT_TIMEOUT   1000
#define GS100_DEFAULT_NODE      20
#define GS100_PORT_REBOOT       4
#define GS100_PORT_RPARAM       7
#define GS100_PORT_WATCHDOG     9

typedef enum {
    GS100_PARAM_TBL_CONF,
    GS100_PARAM_TBL_RX,
    GS100_PARAM_TBL_NOT_USED,
    GS100_PARAM_TBL_RF,
    GS100_PARAM_TBL_INFO,
    GS100_PARAM_TBL_TX,
}gs100_param_tbl_n;

class SGS_GS100
{
private:
    uint8_t node = GS100_DEFAULT_NODE;
    uint32_t timeout = GS100_DEFAULT_TIMEOUT;
    uint16_t cksum;

    gs_param_table_instance_t tinst[5];

    SGS_CSP* csp_handler = NULL;
    SGS_CONSOLE* console = NULL;
    SGS_WebSocketClient* ws = NULL;

    char fname[6][64];
    char tablebuf[16384]; // for text parsing we need buffer.

    void update_gs100_table_reply(uint8_t table_id);
    void update_gs100_param_reply(uint8_t table_id, std::string name, const char* cstrbin, uint16_t cstrbinlen);
public:
    SGS_GS100(SGS_CSP* _csp_handler, const char* _device, uint32_t _baudrate);
    ~SGS_GS100();
    int register_handle(SGS_WebSocketClient* ws);

    int set_csp_settings(uint8_t _node, uint8_t _timeout);
    int get_param_table(uint8_t table_id);

    int get_param(uint8_t table_id, const char* name, void** value, uint16_t& value_size);
    int set_param(uint8_t table_id, const char* name, void* value, uint16_t& value_size);

    int set_watchdog();
    int save_table(uint8_t table_id);
    int reboot();

    int get_param_table_json(const Json::Value& data);
    int get_param_json(const Json::Value& data);
    int set_param_json(const Json::Value& data);
    int set_watchdog_json(const Json::Value& data);
    int save_table_json(const Json::Value& data);
    int reboot_json(const Json::Value& data);
};

#endif