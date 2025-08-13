#include <stdio.h>
#include <iostream>
#include <inttypes.h>

#include <csp/csp.h>
#include <csp/csp_endian.h>
#include <csp/csp_rtable.h>
#include <csp/arch/csp_thread.h>
#include <csp/drivers/usart.h>
#include <gs/param/rparam.h>
#include <gs/param/table.h>
#include <gs/param/types.h>
#define GS_PARAM_INTERNAL_USE 1
#include <gs/param/internal/types.h>
#include <csp/csp_debug.h>

#include <csp/drivers/can_socketcan.h>
#include <csp/interfaces/csp_if_zmqhub.h>

#include <utl/utl.h>
#include <ifc/ifc.h>

// void callback_csp_debug(char * filename)
// {
// 	char cwd[4096];
// 	char today[64];

// 	getcwd(cwd, 4096);

// 	struct tm * t;
// 	time_t base = time(NULL);
// 	t = localtime(&base);
// 	sprintf(today, "%d_%d_%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

// 	strcat(cwd, "/log/csp_debug_");
// 	strcat(cwd, today);
// 	strcat(cwd, ".log");
// 	memcpy(filename, cwd, 4096);
// }

SGS_CSP::SGS_CSP(uint8_t _addr)
{
    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_IFC);
    this->console->set_level(SGS_DEBUG);

    this->addr = _addr;

    this->console->info("Initialising CSP");
    
    csp_conf_get_defaults(&this->conf);
    this->conf.address = this->addr;
    int error = csp_init(&this->conf);
    if(error != CSP_ERR_NONE)
    {
        this->console->error("CSP Initialize Failed. EC %d", error);
    }

    csp_route_start_task(500, 0);
}

SGS_CSP::~SGS_CSP()
{

}
int SGS_CSP::add_ifc_kiss(const char* device, uint32_t baudrate)
{
    csp_usart_conf_t conf = {
        .device = device,
        .baudrate = baudrate, /* supported on all platforms */
        .databits = 8,
        .stopbits = 1,
        .paritysetting = 0,
        .checkparity = 0};
    int ec = csp_usart_open_and_add_kiss_interface(&conf, CSP_IF_KISS_DEFAULT_NAME,  &this->ifc);
    if (ec != CSP_ERR_NONE) {
        this->console->error("Cannot Add KISS Interface.");
    }
    return ec;
}
#ifdef CSP_HAVE_LIBSOCKETCAN
int SGS_CSP::add_ifc_can(const char* device)
{
    int ec;
#if (CSP_HAVE_LIBSOCKETCAN)
    ec = csp_can_socketcan_open_and_add_interface(device, CSP_IF_CAN_DEFAULT_NAME, 0, false, &this->ifc);
    if (ec != CSP_ERR_NONE) {
        this->console->error("Cannot Add CAN Interface.");
    }
#else
    this->console->error("Only libsocketcan Interface is supperted.")
#endif
    return ec;
}
#endif
#ifdef CSP_HAVE_ZMQHUB
int SGS_CSP::add_ifc_zmq(const char* device)
{
    int ec = csp_zmqhub_init(csp_get_address(), device, 0, &this->ifc);
    if (ec != CSP_ERR_NONE) {
        this->console->error("Cannot Add ZMQ Interface.");
    }
    return ec;
}
#endif
// int SGS_CSP::set_debug_level(csp_debug_level_t debug_level)
// {
//     /* enable/disable debug levels */
//     for (csp_debug_level_t i = 0; i <= CSP_LOCK; ++i) {
//         csp_debug_set_level(i, (i <= debug_level) ? true : false);
//     }
// }

int SGS_CSP::set_route_table(uint8_t dest, uint8_t via)
{
    return csp_rtable_set(dest, 5, this->ifc, via);
}

uint16_t SGS_CSP::get_mcksum()
{
    return csp_hton16(GS_RPARAM_MAGIC_CHECKSUM);
}