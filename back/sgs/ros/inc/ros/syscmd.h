#pragma once
#ifndef _SGS_SYSCMD_H_
#define _SGS_SYSCMD_H_

#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#endif

typedef enum{
    TYPE_SYSCMD         = 1,
    TYPE_SYSCMD_REDIR   = 2,
    TYPE_SET_REDIR_PATH = 3,
    TYPE_DS_CLEANUP     = 4,
    TYPE_HK_REQUEST     = 5,
    TYPE_RESERVED       = 6,
    TYPE_KILL           = 99,
} cmd_type_t;

typedef struct{
    uint8_t redir_out;
    char cmd[199];
}__attribute__ ((packed)) syscmd_t;

typedef struct{
    char cmd[100];
    char redir_path[100];
}__attribute__ ((packed)) syscmd_redir_t;

typedef struct{
    char redir_path[200];
}__attribute__ ((packed)) syscmd_set_redir_path_t;

typedef struct{
    uint8_t opt;
}__attribute__ ((packed)) syscmd_ds_cleanup_t;

typedef struct{
    uint8_t type;
    union {
        syscmd_t        syscmd;
        syscmd_redir_t  syscmd_redir;
        syscmd_set_redir_path_t set_redir;
        syscmd_ds_cleanup_t    ds_cleanup;
    } required;
}__attribute__ ((packed)) cmd_packet_t;

typedef struct{
    uint8_t type;
    uint8_t result;
    int retcode;
}__attribute__ ((packed)) reply_packet_t;

typedef void (*sgssyscmd_reply_callback_t)(reply_packet_t * rep, uint16_t replen);

class SGS_sysCMD {
private:
    SGS_UDPHandler * _udp;
    uint8_t _node;
    uint8_t _port;

    sgssyscmd_reply_callback_t reply_callback = NULL;
public:
    SGS_sysCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port);
    ~SGS_sysCMD();

    void register_reply_callback(sgssyscmd_reply_callback_t f);
    int req_syscmd(syscmd_t * syscmd);
    int req_syscmd_redir(syscmd_redir_t * syscmdredir);
    int req_syscmd_setredirpath(syscmd_set_redir_path_t * redirpath);
    int req_syscmd_dscleanup(syscmd_ds_cleanup_t * cleanup);
};


#endif