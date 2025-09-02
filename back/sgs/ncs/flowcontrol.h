#pragma once
#ifndef _SGS_FLOWCONTROL_H_
#define _SGS_FLOWCONTROL_H_
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#endif

#ifndef _SGS_UDP_H_
#include <sgs/backend/udp.h>
#endif

typedef enum {
    HANDLETYPE_FREE_REQUIRED = -1,
    HANDLETYPE_FREE = 0,
    HANDLETYPE_READY = 1,
    HANDLETYPE_DATA_RECEIVED = 2,
    HANDLETYPE_ON_LOOP = 3
    
}sgsflowcontrol_handleypte_t;
class SGS_Flowcontrol {
private:
    
protected:
    uint8_t _node = 0;
    uint8_t _port = 0;
public:
    bool on;
    int qidx = 0;
    pthread_t queue;
    bool HANDLE[16];
    pthread_t THREAD[16];
    pthread_mutex_t MUTEX[16];
    SGS_UDPHandler * _udp = NULL;

    SGS_Flowcontrol();
    SGS_Flowcontrol(SGS_UDPHandler * udp, uint8_t node, uint8_t port);
    ~SGS_Flowcontrol();

    int alloc_task_idx();
    void dst_task_idx(int idx);
};
#endif