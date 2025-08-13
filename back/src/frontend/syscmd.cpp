#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif


void * Direct_Shell(void * data)
{
    while(!State.RotatorReadReady)
        continue;
    bool dlstate = true;


    //This funcion must be on p_thread[4]
    if((dlstate))
    {
        State.downlink_mode = false;
    }
        
    while(!State.uplink_mode)
        continue;

    cmd_packet_t * ResultCMD = (cmd_packet_t *)data;
    reply_packet_t * ResultReply = new reply_packet_t;
    csp_conn_t * txconn;
    csp_packet_t * packet = (csp_packet_t *)csp_buffer_get(sizeof(cmd_packet_t));
    csp_packet_t * confirm = (csp_packet_t *)csp_buffer_get(sizeof(reply_packet_t));

    uint32_t start, time = 0;
    start = csp_get_ms();

    memcpy(packet->data, ResultCMD, sizeof(cmd_packet_t));
    packet->length = sizeof(cmd_packet_t);

    if(State.Debugmode)
    {
        printf("Direct Shell Data : ");
        for(int i = 0; i < sizeof(cmd_packet_t); i++)
            printf("%u\t", ((uint8_t *)packet->data)[i]);
        printf("\n");
    }


    while(State.uplink_mode)
    {
        if ((txconn = csp_connect(CSP_PRIO_HIGH, setup->obc_node, 27, MIM_DEFAULT_TIMEOUT, 0)) == NULL)
            continue;
        else
            break;
    }
    while (State.uplink_mode && txconn != NULL)
    {
        if(csp_send(txconn, packet, udp->default_timeout *2))
        {
            packet = NULL;
            break;
        }
        else
            continue;
    }
    if(txconn == NULL)
    {
        console->AddLog(SGS_CONMOD_ERR, "Connection Buffer Busy. Skip this command.");
        if(confirm!=NULL)
        {
            csp_buffer_free(confirm);
            confirm = NULL;
        }
    }
    if((confirm = csp_read(txconn, 2 *rx_delay_ms(sizeof(MIM_LEN_PACKET), setup->ax100_node))) != NULL && State.uplink_mode)
    {
        if(State.uplink_mode)
        {
            memcpy(ResultReply, confirm->data, confirm->length);
            console->AddLog(SGS_CONMOD_OK, " Received Shell Reply. Type : %" PRIu8 ", Result : %" PRIu8 ", Retcode : %d", ResultReply->type, ResultReply->result, ResultReply->retcode);
            if(confirm!=NULL)
            {
                csp_buffer_free(confirm);
                confirm = NULL;
            }
        }
    }
    else
        console->AddLog(SGS_CONMOD_ERR, "Cannot Receive Shell Reply.");
    
    if(txconn != NULL)
    {
        csp_close(txconn);
        txconn = NULL;
    }
    
    if(packet!=NULL)
    {
        csp_buffer_free(packet);
        packet = NULL;
    }
    time = csp_get_ms() - start;
    console->AddLog(SGS_CONMOD_DBG, "Communication Time : %u", time);

    State.uplink_mode = false;
    if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
    return NULL;
}



SGS_sysCMD::SGS_sysCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
{
    if(!udp)
        return NULL;
    this->_udp = udp;
    this->_node = node;
    this->_port = port;
}

SGS_sysCMD::~SGS_sysCMD()
{

}

void SGS_sysCMD::register_reply_callback(sgssyscmd_reply_callback_t f)
{
    this->reply_callback = f;
}

int SGS_sysCMD::req_syscmd(syscmd_t * syscmd)
{

}

int SGS_sysCMD::req_syscmd_redir(syscmd_redir_t * syscmdredir)
{

}

int SGS_sysCMD::req_syscmd_setredirpath(syscmd_set_redir_path_t * redirpath)
{

}

int SGS_sysCMD::req_syscmd_dscleanup(syscmd_ds_cleanup_t * cleanup)
{

}