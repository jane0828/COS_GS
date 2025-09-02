#include <iostream>
#include <memory>
#include <unistd.h>

#include <csp/csp.h>
#include <csp/drivers/usart.h>
#include <csp/csp_endian.h>
#include <csp/csp_rtable.h>
#include <gs/param/rparam.h>
#include <gs/param/table.h>
#include <gs/param/types.h>
#define GS_PARAM_INTERNAL_USE 1
#include <gs/param/internal/types.h>



void get_table()
{
    csp_conf_t conf;
    csp_conf_get_defaults(&conf);
    conf.address = 8;
    gs_error_t error;
    int ec = csp_init(&conf);
    printf("CSP Init State : %d\n", ec);
    csp_route_start_task(500, 0);

    csp_iface_t* ifc = NULL;
    csp_usart_conf_t kiss_conf = {
        .device = "/dev/GS102",
        .baudrate = 500000, /* supported on all platforms */
        .databits = 8,
        .stopbits = 1,
        .paritysetting = 0,
        .checkparity = 0};
    ec = csp_usart_open_and_add_kiss_interface(&kiss_conf, CSP_IF_KISS_DEFAULT_NAME,  &ifc);
    printf("Kiss Interface Addition: %d\n", ec);

    csp_rtable_set(20, 5, ifc, CSP_NO_VIA_ADDRESS);

    gs_param_table_id_t tbl_id = 1;
    uint16_t cksum = csp_hton16(GS_RPARAM_MAGIC_CHECKSUM);
    gs_param_table_instance_t tinst;
    memset(&tinst, 0, sizeof(gs_param_table_instance_t));
    char fname[64];
    snprintf(fname, sizeof(fname), "./paraminfo.bin");
    error = gs_rparam_download_table_spec(&tinst, fname, 20, tbl_id, 1000, &cksum);

    printf("Error State : %d\n", error);
    printf("cksum: %hx\n", cksum);

    error = gs_rparam_get_full_table(&tinst, 20, tbl_id, csp_hton16(GS_RPARAM_MAGIC_CHECKSUM), 1000);
    printf("Error State : %d\n", error);

    
    FILE* fp = fopen("./tblspec.txt", "w");
    error = gs_param_list_to_stream(&tinst, true, 0, fp);
    fflush(fp);
    printf("Error State : %d\n", error);

    

}



int main(int, char**)
{

    get_table();


    return 0;
}
