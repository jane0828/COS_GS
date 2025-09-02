#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

static void * task_ping_pong(void * param)
{
    sgs_ping_recall_t * recall = new sgs_ping_recall_t;
    memcpy(recall, param, sizeof(sgs_ping_recall_t));
    recall->ping->HANDLE[recall->channel] = HANDLETYPE_DATA_RECEIVED;
    pthread_mutex_lock(&recall->ping->MUTEX[recall->channel]);
    
    if(recall->ping->HANDLE[recall->channel] == HANDLETYPE_FREE)
        return NULL;
    void * repdata;
    uint16_t replen = 0;
    uint8_t dstport = 0;
    if(recall->type = PINGTYPE_PING_SIGNAL)
    {
        repdata = NULL;
    }
    else
    {
        repdata = malloc(recall->ping->udp->mtu);
        memset(repdata, 0, sizeof(repdata));
        replen = recall->ping->udp->mtu;
    }
    if(recall->type == PINGTYPE_PING_PINGCSP)
        dstport = CSP_PING;
    else
        dstport = recall->ping->_port;
    int transmit_time = recall->ping->udp->task_request_csp(recall->ping->_node, dstport, recall->reqdata, recall->reqlen, repdata, &replen);
    if(recall->ping->ping_callback)
    {
        sgs_ping_callback_t * callbackdata = new sgs_ping_callback_t;
        if(transmit_time)
            callbackdata->type = recall->type;
        else
            callbackdata->type = PINGTYPE_PING_ERR_TIMEOUT;
        callbackdata->transmit_time = transmit_time;
        callbackdata->repdata = repdata;
        callbackdata->replen = replen;
        recall->ping->ping_callback(callbackdata);
    }
    recall->ping->HANDLE[recall->channel] = HANDLETYPE_FREE_REQUIRED;
    return NULL;
}

SGS_PING::SGS_PING(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
    :SGS_Flowcontrol(udp, node, port)
{

}

SGS_PING::~SGS_PING()
{

}

void SGS_PING::callback_err(int type)
{
    sgs_ping_callback_t * callbackdata = new sgs_ping_callback_t;
    callbackdata->type = type;
    callbackdata->transmit_time = 0;
    callbackdata->repdata = NULL;
    callbackdata->replen = 0;
    this->callback(callbackdata);
}

void SGS_PING::init_recall_info(sgs_ping_recall_t * recall, int allocidx, int type, void * reqdata, uint16_t reqlen)
{
    if(!recall)
        recall = new sgs_ping_recall_t;
    recall->channel = allocidx;
    recall->type = type;
    recall->ping = this;
    recall->reqdata = (void *)reqdata;
    recall->reqlen = reqlen;
}

void SGS_PING::register_ping_callback(sgsping_reply_callback_t f)
{
    this->callback = f;
}

int SGS_PING::ping_csp(int size)
{
    char pingdata[size];
    memset(pingdata, 0, sizeof(pingdata));
    for(int i = 0; i < size; i++)
        pingdata[i] = i;
    sgs_ping_recall_t * recall = NULL;
    int alloc = this->alloc_task_idx();
    if(alloc < 0)
    {
        this->callback_err(PINGTYPE_PING_ERR_QUEUE_ALLOC);
        return PINGTYPE_PING_ERR_QUEUE_ALLOC;
    }
    this->init_recall_info(recall, alloc, PINGTYPE_PING_PINGCSP, pingdata, size);
    pthread_create(&this->THREAD[alloc], NULL, task_ping_pong, recall);
    while(this->HANDLE[alloc] < HANDLETYPE_DATA_RECEIVED)
        continue;
    return 0;
}

int SGS_PING::ping_pong(int type, void * reqdata, uint16_t reqlen)
{
    char repdata[this->_udp->mtu];
    uint16_t replen;
    sgs_ping_recall_t * recall = NULL;
    int alloc = this->alloc_task_idx();
    if(alloc < 0)
    {
        this->callback_err(PINGTYPE_PING_ERR_QUEUE_ALLOC);
        return PINGTYPE_PING_ERR_QUEUE_ALLOC;
    }
    this->init_recall_info(recall, alloc, type, reqdata, reqlen);
    pthread_create(&this->THREAD[alloc], NULL, task_ping_pong, recall);
    while(this->HANDLE[alloc] < HANDLETYPE_DATA_RECEIVED)
        continue;
    return 0;
}

int SGS_PING::signal(int type, void * reqdata, uint16_t reqlen)
{
    char repdata[this->_udp->mtu];
    uint16_t replen;
    sgs_ping_recall_t * recall = new sgs_ping_recall_t;
    int alloc = this->alloc_task_idx();
    if(alloc < 0)
    {
        this->callback_err(PINGTYPE_PING_ERR_QUEUE_ALLOC);
        return PINGTYPE_PING_ERR_QUEUE_ALLOC;
    }
    this->init_recall_info(recall, alloc, type, reqdata, reqlen);
    pthread_create(&this->THREAD[alloc], NULL, task_ping_pong, recall);
    while(this->HANDLE[alloc] < HANDLETYPE_DATA_RECEIVED)
        continue;
    return 0;
}