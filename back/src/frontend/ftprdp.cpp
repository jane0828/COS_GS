#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

/* CSP */
#include <csp/csp.h>
#include <csp/csp_error.h>
#include <csp/interfaces/csp_if_kiss.h>
#include <csp/drivers/usart.h>
#include <csp_port.h>
#include <csp/arch/csp_queue.h>
#include <csp/arch/csp_semaphore.h>
#include <csp/arch/csp_malloc.h>
#include <csp/arch/csp_time.h>
#include <csp_conn.h>
#include <csp_io.h>
#include <csp/csp_endian.h>
#include <csp/delay.h>

/* Drivers / Util */
#include <gs/util/log.h>
#include <gs/ftp/client.h>
#include <gs/ftp/types.h>
#include <gs/util/crc32.h>
#include <gs/util/string.h>
#include <gs/util/crc32.h>
#include <gs/util/clock.h>
#include <gs/util/vmem.h>

#include <sgs/backend/gstrx.h>
#include <sgs/backend/udp.h>
#include <sgs/backend/ftp.h>
#include <sgs/global.h>

static unsigned int ftp_chunk_size = 180;
static unsigned int ftp_backend = 3; // Use file backend as standard
static const char * const packet_missing = "-";
static const char * const packet_ok = "+";


int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t * listent, void * data)
{
    char pathbuf[256];
    time_t tmtime = time(0);
    struct tm * local = localtime(&tmtime);
    sprintf(pathbuf, "./data/listup/Listup--%04d-%02d-%02d-%02d-", local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour);
    strcat(pathbuf, State.gslistup->fpathbuf);
    State.gslistup->fd = fopen(pathbuf, "wb");

    memset(State.gslistup->flistbuf, 0, sizeof(State.gslistup->flistbuf));
    if(listent->type == GS_FTP_LIST_FILE)
        sprintf(State.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "F", listent->size, listent->path);
    else
        sprintf(State.gslistup->flistbuf, "Type : %s\tSize : %u\tName : %s\n", "D", listent->size, listent->path);
    fprintf(State.gslistup->fd, "%s", State.gslistup->flistbuf);
    strcat(State.gslistup->fdispbuf, State.gslistup->flistbuf);
    fclose(State.gslistup->fd);
    return 0;
}

void * ftp_downlink_onorbit(void * param){
#ifndef CSP_USE_RDP
#define CSP_USE_RDP
#endif
    ftp_avail();
    bool dlstate = State.downlink_mode;
    //This funcion must be on p_thread[4]
    if((dlstate))
        State.downlink_mode = false;
    while(!State.uplink_mode)
        continue;
    while(!State.RotatorReadReady)
        continue;
    
    //host : OBC adress
    //Port : 15
    ftpinfo * FTP = (ftpinfo *) param;
    // gs_ftp_info_callback_t = ftp_callback;
    printftp("Start FTP Downlink.");
    State.downlink_mode = false;
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = 5000; //default timeout value
    ftp_config.chunk_size = State.chunk_sz; //default chunk size value
    State.ftp_mode = true;
    int status = (int)gs_ftp_download(&ftp_config, FTP->local_path, FTP->remote_path, ftp_callback, NULL);
    if (status != 0) {
		console.AddLog("[ERROR]##Fail to complete ftp_download. Retcode : %d", status);\
	}
    else
    {
        console.AddLog("[OK]##End FTP download. Retcode : %d", status);\
    }
    ftp_abort();
    State.ftp_mode = false;
    if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
#ifdef CSP_USE_RDP
#undef CSP_USE_RDP
#endif
	return NULL;
}

void * ftp_uplink_onorbit(void * param){
#ifndef CSP_USE_RDP
#define CSP_USE_RDP
#endif
    ftp_avail();
    bool dlstate = State.downlink_mode;
    //This funcion must be on p_thread[4]
    if((dlstate))
        State.downlink_mode = false;
    while(!State.uplink_mode)
        continue;
    while(!State.RotatorReadReady)
        continue;
    //host : OBC adress
    //Port : 15
    ftpinfo * FTP = (ftpinfo *) param;
    State.downlink_mode = false;
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = 5000; //default timeout value
    ftp_config.chunk_size = State.chunk_sz; //default chunk size value
    State.ftp_mode = true;
    int status = 0;
    status = (int)gs_ftp_upload(&ftp_config, FTP->local_path, FTP->remote_path, ftp_callback, NULL);
    printftp("FTP task DONE.");
    
    if (status != 0) {
		console.AddLog("[ERROR]##Fail to complete ftp_upload. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##End FTP upload. Retcode : %d", status);
    }
    ftp_abort();
    State.ftp_mode = false;
    if(dlstate)
    {
        State.downlink_mode = dlstate;
    }
#ifdef CSP_USE_RDP
#undef CSP_USE_RDP
#endif
	return NULL;
}


void * ftp_list_onorbit(void *){
    char flistbuffer[16384];
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_list(&ftp_config, State.gslistup->fpathbuf, ftp_list_callback, NULL);
    
    if (status != 0) {
		console.AddLog("[ERROR]##Fail to call FTP list. Retcode : %d", status);
	}
    else
    {
        console.AddLog("Call FTP list. Retcode : %d", status);
        printf("%s", flistbuffer);
    }
    return NULL;
}

void * ftp_move_onorbit(void *){
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_move(&ftp_config, State.gsmove->from, State.gsmove->to);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP move. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP move done. Retcode : %d", status);
    }
    return NULL;
}

void * ftp_remove_onorbit(void *){
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_remove(&ftp_config, State.gsremove->path);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP remove. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP remove done. Retcode : %d", status);
    }
    return NULL;
}

void * ftp_copy_onorbit(void *){
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_copy(&ftp_config, State.gscopy->from, State.gscopy->to);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP copy. Retcode : %d", status);
	}
    else
    {
        console.AddLog("FTP copy done. Retcode : %d", status);
    }
    return NULL;
}

void * ftp_mkdir_onorbit(void *){
    uint32_t mode = 0;
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_mkdir(&ftp_config, State.gsmkdir->path, mode);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP make directory. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP make directory done. Retcode : %d", status);
    }
    return NULL;
}

void * ftp_rmdir_onorbit(void *){
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPRDP_PORT;
    ftp_config.timeout = setup->default_timeout + setup->guard_delay; //default timeout value
    ftp_config.chunk_size = 200; //default chunk size value
    int status = (int)gs_ftp_rmdir(&ftp_config, State.gsrmdir->path);

    if (status != 0) {
		console.AddLog("[ERROR]##Fail to FTP remove directory. Retcode : %d", status);
	}
    else
    {
        console.AddLog("[OK]##FTP remove directory done. Retcode : %d", status);
    }
    return NULL;
}