#include <dcs/dcs.h>

static void* amp_safety(void* param)
{
    int ec;
    if(!param)
        return NULL;
    SGS_Q5Signal* q5signal = (SGS_Q5Signal*) param;

    if(q5signal->runstatus)
        return NULL;
    q5signal->runstatus = true;
    ec = q5signal->amp_on();
    if(ec != SGS_ERROR_NOT_CONNECTED)
    {
        q5signal->console->error("Failed to Turn on. Cannot Connect to Amplifier.");
        q5signal->runstatus = false;
        return NULL;
    }
    time_t start = std::time(nullptr);
    
    while(q5signal->runstatus) {
        time_t now = std::time(nullptr);

        if(now - start >= q5signal->timeout)
            break;
    }
    ec = q5signal->amp_off();
    if(ec != SGS_ERROR_NOT_CONNECTED)
    {
        q5signal->console->error("Failed to Turn off. Cannot Connect to Amplifier.");
        return NULL;
    }
    return NULL;
}

SGS_Q5Signal::SGS_Q5Signal()
{
    this->serial = new SGS_RS232("/dev/GPIO8C", 9600);
    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_DCS);
    memset(this->buf, 0, sizeof(this->buf));
}

SGS_Q5Signal::~SGS_Q5Signal()
{
    this->serial->Close();
}

int SGS_Q5Signal::gpio_set_on(int port)
{
    memset(this->buf, 0, sizeof(this->buf));
    sprintf(buf, "gpio set %d", port);
    buf[10] = 13;
    if(this->serial->Write(this->buf, strlen(this->buf)) > 0)
    {
        return SGS_OK;
    }
    else
    {
        return SGS_ERROR_NOT_CONNECTED;
    }
}

int SGS_Q5Signal::gpio_clear_off(int port)
{
    memset(this->buf, 0, sizeof(this->buf));
    sprintf(buf, "gpio clear %d", port);
    buf[13] = 13;
    if(this->serial->Write(this->buf, strlen(this->buf)) > 0)
    {
        return SGS_OK;
    }
    else
    {
        return SGS_ERROR_NOT_CONNECTED;
    }
}

int SGS_Q5Signal::gpio_buf_clean(int port)
{
    memset(this->buf, 0, sizeof(this->buf));
    buf[0] = 0;
    buf[1] = 13;
    if(this->serial->Write(this->buf, 2) > 0)
    {
        return SGS_OK;
    }
    else
    {
        return SGS_ERROR_NOT_CONNECTED;
    }
}

int SGS_Q5Signal::amp_on()
{
    return gpio_set_on(this->conn_port);
}

int SGS_Q5Signal::amp_off()
{
    return gpio_clear_off(this->conn_port);
}

void SGS_Q5Signal::amp_task()
{
    int ret = pthread_create(&this->runthread, NULL, amp_safety, this);
    if (ret != 0) {
        this->console->error("AMP Thread is already exist.");
    }
    return;
}