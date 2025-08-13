#pragma once
#ifndef _SGS_UDP_H_
#define _SGS_UDP_H_

#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <inttypes.h>          
#endif

#ifndef _SGS_GSTRX_H_
#include <sgs/backend/gstrx.h>
#endif

#ifndef _SGS_IO_SHELL_H_
#include <sgs/baseline/io_shell.h>
#endif

// Telecommunication Length
#define CSP_PKT_LEN_MAX              256

// Telecommunication Default Settings
#define MIM_DEFAULT_TIMEOUT         1000
#define MIM_DEFAULT_DATALEN 		1024

typedef struct{
	uint16_t target;
	uint16_t filestatus;
	uint32_t filenum;
	uint32_t offset;
	uint32_t step;
}__attribute__((packed)) dlreqdata;

typedef struct{
	int32_t filenum;
	uint32_t file[];
}__attribute__((packed)) filelist;



void * UDPQueueHandler(void * param);

typedef int (*udp_dlpkt_handle_hook_func_t)(void * data, uint16_t len);


typedef enum
{
	UDP_PORT_NOUSE = 0,
	UDP_PORT_CSP_ALLOC = 1,
	UDP_PORT_CSP_SVR = 2,
	UDP_PORT_CSP_CLI = 3
} udp_port_settings_t;

typedef enum
{
    UDP_NOT_BUSY = 0,
    UDP_BUSY_DL = 1,
    UDP_BUSY_UL = 2,
    UDP_BUSY_SCAN = 3,
    UDP_READY_DL = 4,
    UDP_READY_UL = 5,
    UDP_READY_SCAN = 6
} udp_busy_state_t;

typedef struct {

} udp_queue_handle_t;

class SGS_UDPHandler {
private:
    uint16_t _id;
	uint8_t port[32];
	uint8_t ulqueue[64];
    SGS_CONSOLE* console;

public:
    int mtu;
    uint8_t state = UDP_NOT_BUSY;
    csp_conn_t * conn;
	uint32_t default_timeout;
	csp_socket_t * sock;
    csp_packet_t * ulpkt;
	csp_packet_t * dlpkt;

    SGS_GSTRX * gstrx;

    SGS_Console_f callback = printlog;

    bool TRx_mode;
    bool Signaltest;
    bool downlink_mode;
    bool uplink_mode;
    
	SGS_UDPHandler(uint16_t id, int mtu);
	~SGS_UDPHandler();

    udp_dlpkt_handle_hook_func_t DLpkt_Handle[32];

    void dlpkt_handle_hook_set(int index, udp_dlpkt_handle_hook_func_t f);
    int default_dlpkt_handler_csp(void * data, uint16_t len);
    int dltask_terminate();

    int task_scan_csp();
    int task_receive_csp();
    int task_request_csp(uint8_t node, uint8_t port, void * data, uint16_t len, void * reply, uint16_t * replen);
};

#endif