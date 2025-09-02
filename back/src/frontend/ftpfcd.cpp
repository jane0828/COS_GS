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

#include <sgs/include.h>
#include <sgs/backend/gstrx.h>
#include <sgs/backend/udp.h>
#include <sgs/backend/ftp.h>
#include <sgs/global.h>

void printftp(const char * input, ...)
{
	va_list args;
	va_start(args, input);
    memset(State.FTPWindowBuffer, 0, sizeof(State.FTPWindowBuffer));
    vsprintf(State.FTPWindowBuffer, input, args);
    console.AddLog("[FTP]##%s", State.FTPWindowBuffer);
    csp_log_protocol("FTP: %s (info)", State.FTPWindowBuffer);
	va_end(args);
}

class FTP_FCD_V1
{
private:
    uint32_t ftp_chunk_size = 185;
    uint32_t ftp_backend = 3;
    const char * packet_missing = "-";
    const char * packet_ok = "+";

public:
    char name[30];
    char local[256];
    char remote[256];

    FTP_FCD_V1();
    ~FTP_FCD_V1();
};

FTP_FCD_V1::FTP_FCD_V1()
{
    memset(this->name, 0, sizeof(this->name));
    memset(this->local, 0, sizeof(this->local));
    memset(this->remote, 0, sizeof(this->remote));
}

FTP_FCD_V1::~FTP_FCD_V1()
{

}

void ftp_callback(const gs_ftp_info_t * info)
{
    switch(info->type)
    {
        case GS_FTP_INFO_DL_FILE : {
            console.AddLog("[OK]##Received FTP Download Spec.");
            printftp("Received FTP Download Spec. Filesize : %u, CRC : %u.", info->u.file.size, info->u.file.crc);
            break;
        }
        case GS_FTP_INFO_UL_FILE : {
            console.AddLog("[OK]##Received FTP Upload Spec.");
            printftp("Received FTP Upload File Spec. Filesize : %u, CRC : %u.", info->u.file.size, info->u.file.crc);
            break;
        }
        case GS_FTP_INFO_CRC : {
            printftp("FTP Transaction Completed.");
            console.AddLog("Calculate FTP CRC. Remote : %u, Local : %u.", info->u.crc.remote, info->u.crc.local);
            break;
        }
        case GS_FTP_INFO_DL_COMPLETED : {
            console.AddLog("[OK]##Received FTP Status Request.");
            if(info->u.completed.completed_chunks != 0)
            {
                printftp("Restart FTP Downlink from chunk : %u, to total : %u", info->u.completed.completed_chunks, info->u.completed.total_chunks);
            }
            else
            {
                printftp("Start FTP Downlink. Total Chunk Size : %u", info->u.completed.total_chunks);
            }
            break;
        }
        case GS_FTP_INFO_UL_COMPLETED : {
            console.AddLog("[OK]##Received FTP Status Reply.");
            if(info->u.completed.completed_chunks != 0)
            {
                printftp("Restart FTP Uplink from chunk : %u, to total : %u", info->u.completed.completed_chunks, info->u.completed.total_chunks);
            }
            else
            {
                printftp("Start FTP Uplink. Total Chunk Size : %u", info->u.completed.total_chunks);
            }
            break;
        }
        case GS_FTP_INFO_DL_PROGRESS : {
            printftp("Download : %u/%u", info->u.progress.current_chunk, info->u.progress.total_chunks);
            break;
        }
        case GS_FTP_INFO_UL_PROGRESS : {
            printftp("Upload : %u/%u", info->u.progress.current_chunk, info->u.progress.total_chunks);
            break;
        }
        default : {
            console.AddLog("[ERROR]##Invalid Call to FTP Callback Function.");
        }
    }
}



static void * filelisthandler(filelist * list, uint16_t target, uint16_t status, int index = 0)
{
    ftpinfo *nowftplistup = (ftpinfo *)malloc(sizeof(ftpinfo));
    char filetype[128];
    char filestatus[16];
    char filelabel[16];
    char fileconf[8];
    char nowtime[24];
    char filename[64];
    char filepath[256];
    char localfilepath[256];
    char localfiletype[16];
    
    //filetype input
    switch(target) {
        case MIM_DLTYPE_HK : {
            sprintf(filetype, "/mnt/ds/hk");
            sprintf(filelabel, "hktlm");
            sprintf(fileconf, ".hk");
            sprintf(localfiletype, "data/hk");
            break;
        }
        case MIM_DLTYPE_AOD : {
            sprintf(filetype, "/mnt/ds/aod");
            sprintf(filelabel, "aodtlm");
            sprintf(fileconf, ".aod");
            sprintf(localfiletype, "data/aod");
            break;
        }
        case MIM_DLTYPE_LOG : {
            sprintf(filetype, "/mnt/ds/log");
            sprintf(filelabel, "logtlm");
            sprintf(fileconf, ".log");
            sprintf(localfiletype, "data/log");
            break;
        }
        case MIM_DLTYPE_SNSR : {
            sprintf(filetype, "/mnt/ds/snsr");
            sprintf(filelabel, "snsr");
            sprintf(fileconf, ".raw");
            sprintf(localfiletype, "data/snsr");
            break;
        }
        case MIM_DLTYPE_GPS : {
            sprintf(filetype, "/mnt/ds/gps");
            sprintf(filelabel, "raw");
            sprintf(fileconf, ".raw");
            sprintf(localfiletype, "data/gps");
            break;
        }
        case MIM_DLTYPE_CTRLO : {
            sprintf(filetype, "/mnt/ds/ctrlo");
            sprintf(filelabel, "raw");
            sprintf(fileconf, ".raw");
            sprintf(localfiletype, "data/ctrlo");
            break;
        }
    }
    //filestatus input
    switch(status) {
        case MIM_DLSTAT_NEW : {
            sprintf(filestatus, "/");
            break;
        }
        case MIM_DLSTAT_OLD : {
            sprintf(filestatus, "/sent/");
            break;
        }
        case MIM_DLSTAT_ING : {
            sprintf(filestatus, "/input/");
            break;
        }
    }
    time_t secs = time(0);
    struct tm *local = localtime(&secs);
    sprintf(nowtime, "__%04d_%02d_%02d_%02d_%02d_%02d__", local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour, local->tm_min, local->tm_sec);


    sprintf(filename, "%08" PRIu32 , list->file[index]);
    strcpy(filepath, filetype);
    strcat(filepath, filestatus);
    strcat(filepath, filelabel);
    strcat(filepath, filename);
    strcat(filepath, fileconf);

    strcpy(localfilepath, localfiletype);
    strcat(localfilepath, filestatus);
    strcat(localfilepath, nowtime);
    strcat(localfilepath, filelabel);
    strcat(localfilepath, filename);
    strcat(localfilepath, fileconf);
    memcpy(nowftplistup->name, filename, strlen(filename)+1);
    memcpy(nowftplistup->remote_path, filepath, strlen(filepath)+1);
    memcpy(nowftplistup->local_path, localfilepath, strlen(localfilepath)+1);
    //filename input
    return nowftplistup;
}

void * ftp_downlink_force(void * param){
#ifndef CSP_USE_RDP
#define CSP_USE_RDP
#endif
    FTP_FCD_V1 * ftp = (FTP_FCD_V1 *)param;
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
    // gs_ftp_info_callback_t = ftp_callback;
    printftp("Start FTP Downlink.");
    State.downlink_mode = false;
    gs_ftp_settings_t ftp_config;
    ftp_config.mode = GS_FTP_MODE_STANDARD;
    ftp_config.host = setup->obc_node;
    ftp_config.port = FTPFCD_PORT;
    ftp_config.timeout = 5000; //default timeout value
    ftp_config.chunk_size = State.chunk_sz; //default chunk size value
    State.ftp_mode = true;
    int status = (int)gs_ftp_download_force(&ftp_config, ftp->local, ftp->remote, ftp_callback, NULL, State.iteration);
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

void * ftp_uplink_force(void * param){
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
    ftp_config.port = FTPFCD_PORT;
    ftp_config.timeout = 5000; //default timeout value
    ftp_config.chunk_size = State.chunk_sz; //default chunk size value
    State.ftp_mode = true;
    int status = 0;
    status = (int)gs_ftp_upload_force(&ftp_config, FTP->local_path, FTP->remote_path, ftp_callback, NULL, State.iteration);
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