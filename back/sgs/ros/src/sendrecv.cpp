#include <ros/ros.h>

// SGS_SendRecv::SGS_SendRecv(uint8_t node, uint8_t reqport, uint8_t repport)
// {
//     this->node = node;
//     this->reqport = reqport;
//     this->repport = repport;
// }

// SGS_SendRecv::SGS_SendRecv(uint8_t node, uint8_t port)
// {
//     this->node = node;
//     this->reqport = port;
//     this->repport = port;
// }

// SGS_SendRecv::~SGS_SendRecv()
// {

// }

// void SGS_SendRecv::delay_ms(uint32_t delay)
// {
//     usleep(1000 * SGS_DEFAULT_PREDELAY);
//     return;
// }

// int SGS_SendRecv::sendto(void* data, uint16_t datalen)
// {
//     return csp_sendto(CSP_PRIO_NORM, this->node, this->reqport, this->repport, this->opts, (void *)data, this->timeout);
// }

// int SGS_SendRecv::recvfrom(void* buf, uint16_t buflen)
// {
//     csp_packet_t* packet;
//     if(!buf)
//         return SGS_ERROR_NULL;
//     packet = csp_recvfrom(CSP_PRIO_NORM, this->timeout);
//     if(buflen > packet->length)
//         return SGS_ERROR_RANGE;
//     memset(buf, 0, buflen);
//     memcpy(buf, packet->data, packet->length);
//     csp_buffer_free(packet);
// }

// int SGS_SendRecv::sgs_ping(uint32_t size)
// {
//     uint8_t* data = (uint8_t*)malloc(size);
//     for(uint32_t i = 0; i < size; i++)
//         data[i] = i % 256;

//     switch_to_tx();
//     usleep(1000 * SGS_DEFAULT_PREDELAY);
//     if()
//     {
//         return SGS_ERROR_BUSY;
//     }
// }

// int SGS_SendRecv::set_timeout(uint32_t timeout)
// {
//     this->timeout = timeout;
// }

// int SGS_SendRecv::set_switch_predelay(uint32_t predelay)
// {
//     this->predelay = predelay;
// }

// int SGS_SendRecv::set_swtich_postdelay(uint32_t postdelay)
// {
//     this->postdelay = postdelay;
// }

// int SGS_SendRecv::set_switch_bitweight(uint32_t bitweight)
// {
//     this->bitweight = bitweight;
// }

// int SGS_SendRecv::set_interval(uint32_t interval)
// {
//     this->interval = interval;
// }

// void SGS_SendRecv::RegisterCSPOption(uint32_t csp_opts)
// {
//     this->opts = csp_opts;
// }

// void SGS_SendRecv::JsonCallback(const Json::Value& jsonData)
// {

// }