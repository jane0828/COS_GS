// #pragma once
#ifndef _IFC_IP_H_
#define _IFC_IP_H_

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <pthread.h>
#include <utl/utl.h>


typedef enum {
    SGS_IO_IP_MODE_SERVER,
    SGS_IO_IP_MODE_CLIENT
}sgs_io_ip_mode_t;

class SGS_IP
{
private:
    pthread_t THREAD[16];
public:
    int mode;
    int descripter;
    int serv_sock;
    int clnt_sock;
    int port;
    int opt;
    sockaddr_in serv_addr;
    sockaddr_in clnt_addr;

    SGS_IP(int _mode, int _dst, int _port);
    ~SGS_IP();
    
    SGS_CONSOLE* console;

    int InitializeServer();
    int InitializeClient();

};

#endif