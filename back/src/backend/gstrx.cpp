#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

#ifdef CSP_USE_PROMISC
extern csp_queue_handle_t csp_promisc_queue;
#endif
csp_iface_t csp_if_kiss;
csp_kiss_handle_t csp_kiss_driver;

void callback_usart_rx(uint8_t * buf, int len, void * pxTaskWoken) {
	csp_kiss_rx(&csp_if_kiss, buf, len, pxTaskWoken);
}

void callback_csp_debug(char * filename)
{
	char cwd[4096];
	char today[64];

	getcwd(cwd, 4096);

	struct tm * t;
	time_t base = time(NULL);
	t = localtime(&base);
	sprintf(today, "%d_%d_%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

	strcat(cwd, "/log/csp_debug_");
	strcat(cwd, today);
	strcat(cwd, ".log");
	memcpy(filename, cwd, 4096);
}

SGS_GSTRX::SGS_GSTRX(const char * devname, uint32_t baud, uint8_t csp_node)
{
	csp_buffer_init(2048, 2048);
	csp_init(csp_node);
	csp_route_set(CSP_DEFAULT_ROUTE, &csp_if_kiss, CSP_NODE_MAC);
	csp_kiss_init(&csp_if_kiss, &csp_kiss_driver, usart_putc, usart_insert, "KISS");

	this->usart.device = devname;
	this->usart.baudrate = baud;
	usart_init(&this->usart);
	usart_set_callback(callback_usart_rx);

	csp_rdp_set_opt(12, 10000, 7500, 1, 2000, 20);
	csp_route_start_task(0, 0);

	csp_debug_hook_set(callback_csp_debug);
}

SGS_GSTRX::~SGS_GSTRX()
{

}