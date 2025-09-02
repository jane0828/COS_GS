#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_RS232::SGS_RS232(char * devname, uint32_t baud)
{
    this->fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
    if(this->fd < 0)
    {
        //
    }
    tcflush(this->fd, TCIOFLUSH);
    this->tio.c_cflag = baud;



}

SGS_RS232::~SGS_RS232()
{

}

void SGS_RS232::SetBaudRate(uint32_t baud)
{
    // this->
}