#ifndef _ROS_SENDRECV_H_
#define _ROS_SENDRECV_H_

#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>

#define SGS_DEFAULT_TIMEOUT     1000
#define SGS_DEFAULT_PREDELAY    100
#define SGS_DEFAULT_POSTDELAY   100
#define SGS_DEFAULT_BITWEIGHT   5
#define SGS_DEFAULT_INTERVAL    5000

class SGS_SendRecv {
private:
    SGS_CONSOLE* console = NULL;

    uint32_t timeout    = SGS_DEFAULT_TIMEOUT;
    uint32_t predelay   = SGS_DEFAULT_PREDELAY;
    uint32_t postdelay  = SGS_DEFAULT_POSTDELAY;
    uint32_t bitweight  = SGS_DEFAULT_BITWEIGHT;
    uint32_t interval   = SGS_DEFAULT_INTERVAL;
    uint32_t opts       = CSP_O_NONE | CSP_O_CRC32;

    uint32_t node;
    uint32_t reqport;
    uint32_t repport;

public:
    
    SGS_SendRecv(uint8_t node, uint8_t reqport, uint8_t repport);
    SGS_SendRecv(uint8_t node, uint8_t port);
    ~SGS_SendRecv();

    void delay_ms(uint32_t delay);
    int sendto(void* data, uint16_t datalen);
    int recvfrom(void* buf, uint16_t buflen);
    int switch_to_tx();
    int switch_to_rx();

    int sgs_ping(uint32_t size);
    int set_timeout(uint32_t timeout);
    int set_switch_predelay(uint32_t predelay);
    int set_swtich_postdelay(uint32_t postdelay);
    int set_switch_bitweight(uint32_t bitweight);
    int set_interval(uint32_t interval);

    void RegisterCSPOption(uint32_t csp_opts);

    void JsonCallback(const Json::Value& jsonData);
}

#endif