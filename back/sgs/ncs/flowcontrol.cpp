#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

static void * flowcontrol_queuemanage(void * param)
{
    SGS_Flowcontrol * flow = (SGS_Flowcontrol *)param;
    while(flow->on)
    {
        if(flow->HANDLE[flow->qidx] == HANDLETYPE_DATA_RECEIVED)
            pthread_mutex_unlock(&flow->MUTEX[flow->qidx]);
        else if(flow->HANDLE[flow->qidx] < HANDLETYPE_FREE)
        {
            pthread_join(flow->THREAD[flow->qidx], NULL);
            flow->HANDLE[flow->qidx] = HANDLETYPE_FREE;
            if(flow->qidx >= 16)
                flow->qidx = 0;
            else
                flow->qidx++;
            
        }
        else
            continue;
    }
    return NULL;
}

SGS_Flowcontrol::SGS_Flowcontrol()
{

}

SGS_Flowcontrol::SGS_Flowcontrol(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
{
    // if(!udp)
    //     return NULL;
    this->_udp = udp;
    this->_node = node;
    this->_port = port;
    for(int i = 0; i < sizeof(this->MUTEX) / sizeof(pthread_mutex_t); i++)
    memset(this->THREAD, 0, sizeof(this->THREAD));
    memset(this->HANDLE, HANDLETYPE_FREE, sizeof(this->HANDLE));
    for(int i = 0; i < sizeof(this->MUTEX) / sizeof(pthread_mutex_t); i++)
    {
        pthread_mutex_init(&this->MUTEX[i], NULL);
        pthread_mutex_lock(&this->MUTEX[i]);
    }
    pthread_create(&this->queue, NULL, flowcontrol_queuemanage, this);
}

SGS_Flowcontrol::~SGS_Flowcontrol()
{
    this->on = false;
    memset(this->HANDLE, HANDLETYPE_FREE, sizeof(this->HANDLE));
    pthread_join(this->queue, NULL);
    for(int i = 0; i < sizeof(this->THREAD) / sizeof(pthread_t); i++)
        pthread_join(this->THREAD[i], NULL);
    for(int i = 0; i < sizeof(this->MUTEX) / sizeof(pthread_mutex_t); i++)
        pthread_mutex_destroy(&this->MUTEX[i]);
}

int SGS_Flowcontrol::alloc_task_idx()
{
    int seq = this->qidx;
    int idx = seq;
    for(seq; seq < sizeof(this->HANDLE); seq++)
    {
        idx = seq % (sizeof(this->HANDLE) / sizeof(int));
        if(this->HANDLE[idx] == HANDLETYPE_FREE)
        {
            this->HANDLE[idx] = HANDLETYPE_READY;
            return idx;
        }
        break;
    }
    return -1;
}

void SGS_Flowcontrol::dst_task_idx(int idx)
{
    this->HANDLE[idx] = HANDLETYPE_FREE;
    pthread_join(this->THREAD[idx], NULL);
}
