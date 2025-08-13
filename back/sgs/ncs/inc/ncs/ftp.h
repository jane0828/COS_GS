#pragma once
#ifndef _SGS_FTP_H_
#define _SGS_FTP_H_
#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#endif

#define MIM_DLTYPE_HK				1
#define MIM_DLTYPE_AOD				2
#define MIM_DLTYPE_LOG				3
#define MIM_DLTYPE_SNSR             4
#define MIM_DLTYPE_GPS              5
#define MIM_DLTYPE_CTRLO            6


#define MIM_DLSTAT_NEW				1
#define MIM_DLSTAT_OLD				2
#define MIM_DLSTAT_ING				3

typedef struct {
    FILE       * fp;
    FILE       * fp_map;
    csp_conn_t * conn;
    uint32_t   timeout;
    char       file_name[GS_FTP_PATH_LENGTH];
    uint32_t   file_size;
    uint32_t   chunks;
    int        chunk_size;
    uint32_t   checksum;
    ftp_status_element_t last_status[GS_FTP_STATUS_CHUNKS];
    uint32_t   last_entries;
    gs_ftp_info_callback_t info_callback;
    void       * info_data;
} gs_ftp_state_t;

typedef struct {
    gs_ftp_backend_type_t backend;
    const char * path;
    uint32_t addr;
    uint32_t size;
} gs_ftp_url_t;

typedef struct{
    FILE * fd;
    char flistbuf[128];
    char fpathbuf[64];
    char fdispbuf[2048];
}__attribute__((packed)) gsftp_listup_t;

typedef struct{
    char from[64];
    char to[64];
}__attribute__((packed)) gsftp_move_t;

typedef struct{
    char from[64];
    char to[64];
}__attribute__((packed)) gsftp_copy_t;

typedef struct{
    char path[64];
}__attribute__((packed)) gsftp_remove_t;

typedef struct{
    char path[64];
}__attribute__((packed)) gsftp_mkdir_t;

typedef struct{
    char path[64];
}__attribute__((packed)) gsftp_rmdir_t;

void printftp(const char * input, ...);

int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t * listent, void * data);
void ftp_callback(const gs_ftp_info_t * info);
void * ftp_uplink_onorbit(void * param);
void * ftp_uplink_force(void * param);
void * ftp_downlink_onorbit(void * param);
void * ftp_downlink_force(void * param);
void * ftp_list_onorbit(void * );
void * ftp_move_onorbit(void * );
void * ftp_remove_onorbit(void * );
void * ftp_copy_onorbit(void * );
void * ftp_mkdir_onorbit(void * );
void * ftp_rmdir_onorbit(void * );

#endif