#ifndef _DCS_Q5SIGNAL_H_
#define _DCS_Q5SIGNAL_H_

#include <utl/utl.h>
#include <ifc/ifc.h>

#define Q5SIGNAL_TIMEOUT    60*14

class SGS_Q5Signal {
private:
    SGS_RS232* serial = NULL;
    int conn_port = 0;
public:
    SGS_CONSOLE* console = NULL;
    const int timeout = Q5SIGNAL_TIMEOUT;
    pthread_t runthread;
    bool runstatus = false;
    char buf[64];

    SGS_Q5Signal();
    ~SGS_Q5Signal();

    int gpio_set_on(int port);
    int gpio_clear_off(int port);
    int gpio_buf_clean(int port);

    int amp_on();
    int amp_off();

    void amp_task();
};

#endif