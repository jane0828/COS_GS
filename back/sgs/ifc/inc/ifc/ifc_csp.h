#ifndef _IFC_CSP_H_
#define _IFC_CSP_H_

#include <stdio.h>
#include <iostream>
#include <inttypes.h>

#include <csp/csp.h>
#include <csp/csp_autoconfig.h>
#include <utl/utl.h>

class SGS_CSP
{
private:
    uint8_t addr;
    // csp_debug_level_t debug_level;
    csp_iface_t* ifc = NULL;
    csp_conf_t conf;
    SGS_CONSOLE* console;
public:
    SGS_CSP(uint8_t _addr);
    ~SGS_CSP();

    int add_ifc_kiss(const char* device, uint32_t baudrate);
#ifdef CSP_HAVE_LIBSOCKETCAN
    int add_ifc_can(const char* device);
#endif
#ifdef CSP_HAVE_ZMQHUB
    int add_ifc_zmq(const char* device);
#endif

    // int set_debug_level(csp_debug_level_t debug_level);
    int set_route_table(uint8_t dest, uint8_t via);
    uint16_t get_mcksum();
};

#endif