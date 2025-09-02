#ifndef _SGS_PING_H_
#define _SGS_PING_H_
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <sgs/backend/udp.h>
#include <sgs/baseline/_flowcontrol.h>
#endif

class SGS_PING;

typedef enum {
    PINGTYPE_PING_ERR_TIMEOUT = -2,
    PINGTYPE_PING_ERR_QUEUE_ALLOC = -1,
    PINGTYPE_PING_DEFAULT = 0,
    PINGTYPE_PING_PINGCSP = 1,
    PINGTYPE_PING_PING_PONG = 2,
    PINGTYPE_PING_SIGNAL = 3
}sgs_ping_type_t;

typedef struct {
    int channel;
    int type;
    SGS_PING * ping;
    void * reqdata;
    uint16_t reqlen;
}sgs_ping_recall_t;

typedef struct {
    int type;
    int transmit_time;
    void * repdata;
    uint16_t replen;
}sgs_ping_callback_t;

typedef void(*sgsping_reply_callback_t)(sgs_ping_callback_t * data);

class SGS_PING
    : virtual public SGS_Flowcontrol {
private:
    sgsping_reply_callback_t callback = NULL;
protected:
    void callback_err(int type);
    void init_recall_info(sgs_ping_recall_t * recall, int allocidx, int type, void * reqdata, uint16_t reqlen);
public:
    SGS_PING();
    SGS_PING(SGS_UDPHandler * udp, uint8_t node, uint8_t port);
    ~SGS_PING();

    void register_ping_callback(sgsping_reply_callback_t f);

    int ping_csp(int size);
    int ping_pong(int type, void * reqdata, uint16_t reqlen);
    int signal(int type, void * reqdata, uint16_t reqlen);
};

#endif