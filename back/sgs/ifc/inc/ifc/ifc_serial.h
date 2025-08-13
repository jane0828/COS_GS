#ifndef _IFC_SERIAL_H_
#define _IFC_SERIAL_H_

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <utl/utl.h>

class SGS_RS232 {
private:
    int fd;
    struct termios tio;

public:
    SGS_RS232(const char *devname, uint32_t baud);
    ~SGS_RS232();

    void SetBaudRate(uint32_t baud);
    ssize_t Write(const void *data, size_t size);
    ssize_t Read(void *buffer, size_t size);
    void Close();
};

#endif