#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

pthread_t LinkTrhead;


void * UDPQueueHandler(void * param)
{
    if(!param)
        return NULL;
    SGS_UDPHandler * udp = (SGS_UDPHandler *)param;

    while(udp->TRx_mode)
    {
        if(udp->downlink_mode)
        {
            pthread_join(LinkTrhead, NULL);
            udp->uplink_mode = true;
            while(udp->state != UDP_BUSY_UL)
                continue;
        }
        else
            continue;
    }
    return NULL;
}

SGS_UDPHandler::SGS_UDPHandler(uint16_t id, int mtu)
{
    this->_id = id;
    this->mtu = CSP_PKT_LEN_MAX;
    memset(this->port, UDP_PORT_NOUSE, sizeof(this->port));
    this->sock = csp_socket(0);
    csp_bind(sock, 1);
    this->port[1] = UDP_PORT_CSP_ALLOC;
    csp_bind(sock, 12);
    this->port[12] = UDP_PORT_CSP_SVR;
    csp_bind(sock, 31);
    this->port[31] = UDP_PORT_CSP_SVR;
    csp_listen(sock, 10);
    this->ulpkt = NULL;
    this->dlpkt = NULL;
}

SGS_UDPHandler::~SGS_UDPHandler()
{
    if(this->dlpkt)
        csp_buffer_free(this->dlpkt);
    if(this->ulpkt)
        csp_buffer_free(this->ulpkt);
    this->dlpkt = NULL;
    this->ulpkt = NULL;
}

void SGS_UDPHandler::dlpkt_handle_hook_set(int port, udp_dlpkt_handle_hook_func_t f)
{
    this->DLpkt_Handle[port] = f;
}

int SGS_UDPHandler::default_dlpkt_handler_csp(void * data, uint16_t len)
{
    this->callback(SGS_CONMOD_WRN, "Unexpected Packet Received. Port: %d Length: %d", csp_conn_dport(this->conn), len);
    char buf[this->mtu];
    memcpy(buf, data, len);
    while (!csp_send(this->conn, this->dlpkt, this->default_timeout))
        continue;
    return len;
}

int SGS_UDPHandler::dltask_terminate()
{
    if(this->dlpkt)
        csp_buffer_free(this->dlpkt);
    if(this->ulpkt)
        csp_buffer_free(this->ulpkt);
    this->dlpkt = NULL;
    this->ulpkt = NULL;
    if(this->conn)
        csp_close(conn);
    this->conn = NULL;
}

//CSP Node Scan
int SGS_UDPHandler::task_scan_csp()
{
    this->Signaltest = true;
    while(this->Signaltest)
    {
        if(csp_ping(19, this->default_timeout, 1, 0))
            continue;
    }
    return NULL;
}

//Beacon
//Ping Reply Handler
//Default Task
int SGS_UDPHandler::task_receive_csp() 
{
	while (this->state == UDP_BUSY_DL)
    {
		if ((this->conn = csp_accept(this->sock, this->default_timeout)) == NULL)
            continue;
		while ((dlpkt = csp_read(this->conn, this->default_timeout)) != NULL)
            this->DLpkt_Handle[csp_conn_dport(this->conn)];

    }
    return NULL;
}

//Ping
//Command
//Direct Shell

int SGS_UDPHandler::task_request_csp(uint8_t node, uint8_t port, void * req, uint16_t reqlen, void * rep, uint16_t * replen)
{
    uint32_t time = csp_get_ms();

    while(this->state != UDP_BUSY_UL)
        continue;
    this->conn = csp_connect(CSP_PRIO_HIGH, node, port, this->default_timeout, 0);
    if(!this->conn)
    {
        this->callback(SGS_CONMOD_ERR, "Failed to create connection.");
        return 0;
    }

    char pkt[mtu * 16];
    
    for(int i = 0 ; i < reqlen; i++)
        sprintf(pkt, "%x\t", ((char *)ulpkt)[i]);
    sprintf(pkt, "\n");
    this->callback(SGS_CONMOD_DBG, pkt);
    memset(pkt, 0, sizeof(pkt));
    this->ulpkt = (csp_packet_t *)csp_buffer_get(this->mtu);

    this->ulpkt->length = reqlen;
    memcpy(this->ulpkt->data, req, reqlen);
    
    while (this->uplink_mode && this->conn != NULL)
    {
        if(csp_send(this->conn, this->ulpkt, this->default_timeout))
        {
            csp_buffer_free(this->ulpkt);
            this->ulpkt = NULL;
            break;
        }
        else
            continue;
    }
    if(this->conn == NULL)
    {
        this->callback(SGS_CONMOD_ERR, "Connection Buffer Busy. Skip this command.");
        if(this->dlpkt != NULL)
        {
            csp_buffer_free(this->dlpkt);
            this->dlpkt = NULL;
        }
    }
    if((this->dlpkt = csp_read(this->conn, rx_delay_ms(this->ulpkt->length, gstrx->csp_node))) != NULL)
    {
        *replen = this->dlpkt->length;
        if(rep == NULL)
            rep = malloc(this->dlpkt->length);
        memcpy(rep, this->dlpkt->data, this->dlpkt->length);
        
        for(int i = 0 ; i < reqlen; i++)
            sprintf(pkt, "%x\t", ((char *)ulpkt)[i]);
        sprintf(pkt, "\n");
        this->callback(SGS_CONMOD_DBG, pkt);
    }
    else
        this->callback(SGS_CONMOD_ERR, "Failed to receive reply.");
    
    if(this->ulpkt != NULL)
    {
        csp_buffer_free(this->ulpkt);
        this->ulpkt = NULL;
    }

    if(this->dlpkt!=NULL)
    {
        csp_buffer_free(this->dlpkt);
        this->dlpkt = NULL;
    }
    
    if(this->conn != NULL)
    {
        csp_close(this->conn);
        this->conn = NULL;
    }
    time = csp_get_ms() - time;
    this->callback(SGS_CONMOD_DBG, "Communication Time : %u", time);

    return NULL;
}
