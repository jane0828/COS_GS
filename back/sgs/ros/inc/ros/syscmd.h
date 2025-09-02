#pragma once
#ifndef _SGS_SYSCMD_H_
#define _SGS_SYSCMD_H_
#define CFE_SB_CMD_HDR_SIZE 8
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#endif

typedef enum{
    TYPE_SYSCMD         = 1,
    TYPE_SYSCMD_REDIR   = 2,
    TYPE_SET_REDIR_PATH = 3,
    TYPE_DS_CLEANUP     = 4,
    TYPE_HK_REQUEST     = 5,
    TYPE_RESERVED       = 6,
    TYPE_KILL           = 99,
} cmd_type_t;

typedef struct{
    uint8_t redir_out;
    char cmd[199];
}__attribute__ ((packed)) syscmd_t;

typedef struct{
    char cmd[100];
    char redir_path[100];
}__attribute__ ((packed)) syscmd_redir_t;

typedef struct{
    char redir_path[200];
}__attribute__ ((packed)) syscmd_set_redir_path_t;

typedef struct{
    uint8_t opt;
}__attribute__ ((packed)) syscmd_ds_cleanup_t;

typedef struct{
    uint8_t type;
    union {
        syscmd_t        syscmd;
        syscmd_redir_t  syscmd_redir;
        syscmd_set_redir_path_t set_redir;
        syscmd_ds_cleanup_t    ds_cleanup;
    } required;
}__attribute__ ((packed)) cmd_packet_t;

typedef struct{
    uint8_t type;
    uint8_t result;
    int retcode;
}__attribute__ ((packed)) reply_packet_t;

typedef void (*sgssyscmd_reply_callback_t)(reply_packet_t * rep, uint16_t replen);

// class SGS_sysCMD {
// private:
//     SGS_UDPHandler * _udp;
//     uint8_t _node;
//     uint8_t _port;

//     sgssyscmd_reply_callback_t reply_callback = NULL;
// public:
//     SGS_sysCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port);
//     ~SGS_sysCMD();

//     void register_reply_callback(sgssyscmd_reply_callback_t f);
//     int req_syscmd(syscmd_t * syscmd);
//     int req_syscmd_redir(syscmd_redir_t * syscmdredir);
//     int req_syscmd_setredirpath(syscmd_set_redir_path_t * redirpath);
//     int req_syscmd_dscleanup(syscmd_ds_cleanup_t * cleanup);
// };



// FOR COSMIC & BEE-1000 GS



/****************************************** For BOTH COSMIC & BEE-1000 ******************************************/
/**********************************/
/*                                */
/*   UT(R)X (COSMIC & BEE-1000)   */
/*        GomSpace AX100          */
/*                                */
/**********************************/

// Define MSGID

#define ADCS_HK_TLM_MID         0x0820
#define ADCS_REF_TLM_WHL_MID    0x0821
#define ADCS_REF_TLM_MTQ_MID    0x0822
#define ADCS_LOOP_WAKEUP_MID    0x0823
#define ADCS_OUT_TLM_MID        0x0824



// Define Function Codes

#define UTRX_APP_NOOP_CC                      0  //set
#define UTRX_APP_RESET_COUNTERS_CC            1  //set
#define UTRX_APP_RESET_APP_CMD_COUNTERS_CC    2  //set
#define UTRX_APP_RESET_DEVICE_CMD_COUNTERS_CC 3  //set
#define UTRX_APP_PROCESS_CC                   4 //set
#define UTRX_APP_DISPLAY_PARAM_CC             5 //set
#define UTRX_GNDWDT_CLEAR_CC                  6 //set
#define UTRX_REBOOT_CC                        7 //set
#define UTRX_RXCONF_SET_BAUD_CC               8 //set
#define UTRX_TXCONF_SET_BAUD_CC               9 //set
#define UTRX_SET_DEFAULT_BAUD_CC              10 //set
#define UTRX_RPARAM_SAVE_1_CC                 11 //set
#define UTRX_RPARAM_SAVE_5_CC                 12 //set
#define UTRX_RPARAM_SAVE_ALL_CC               13 //set
#define UTRX_RXCONF_SET_FREQ_CC               14 //set
#define UTRX_TXCONF_SET_FREQ_CC               15 //set
#define UTRX_CHECK_STATE_PING_CC              32 //set


#define UTRX_RXCONF_GET_BAUD_CC           16 //get
#define UTRX_RXCONF_GET_GUARD_CC          17 //get
#define UTRX_TXCONF_GET_BAUD_CC           18 //get
#define UTRX_TLM_GET_TEMP_BRD_CC          19 //get
#define UTRX_TLM_GET_LAST_RSSI_CC         20 //get
#define UTRX_TLM_GET_LAST_RFERR_CC        21 //get
#define UTRX_TLM_GET_ACTIVE_CONF_CC       22 //get
#define UTRX_TLM_GET_BOOT_COUNT_CC        23 //get
#define UTRX_TLM_GET_BOOT_CAUSE_CC        24 //get
#define UTRX_TLM_GET_LAST_CONTACT_CC      25 //get
#define UTRX_TLM_GET_TOT_TX_BYTES_CC      26 //get
#define UTRX_TLM_GET_TOT_RX_BYTES_CC      27 //get
#define UTRX_GET_STATUS_CONFIGURATION_CC  28 //get
#define UTRX_RXCONF_GET_FREQ_CC           29 //get
#define UTRX_TXCONF_GET_FREQ_CC           30 //get

#define UTRX_SEND_HK_CC                   31//hk data



// Define CMD Structure

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
} UTRX_NoArgsCmd_t;


typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint32_t                  Arg;
} UTRX_u32Cmd_t;



typedef UTRX_NoArgsCmd_t    UTRX_NoopCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_ResetCountersCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_ResetAppCmdCountersCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_ResetDeviceCmdCountersCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_ProcessCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_DisplayParamCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_GndWdtClearCmd_t;
typedef UTRX_NoArgsCmd_t    UTRX_RebootCmd_t;

typedef UTRX_u32Cmd_t       UTRX_SetTxFreqCmd_t;
typedef UTRX_u32Cmd_t       UTRX_SetTxBaudCmd_t;
typedef UTRX_u32Cmd_t       UTRX_SetRxFreqCmd_t;
typedef UTRX_u32Cmd_t       UTRX_SetRxBaudCmd_t;






/****************************************** For COSMIC ******************************************/

/**********************************/
/*                                */
/*         STRX (COSMIC)          */
/*        GomSpace AX2150         */
/*                                */
/**********************************/

// Define MSGID

// Define Function Codes
#define STRX_APP_NOOP_CC                      0  //set
#define STRX_APP_RESET_COUNTERS_CC            1  //set
#define STRX_APP_RESET_APP_CMD_COUNTERS_CC    2  //set
#define STRX_APP_RESET_DEVICE_CMD_COUNTERS_CC 3  //set
// #define STRX_APP_PROCESS_CC                   4 //set
#define STRX_APP_DISPLAY_PARAM_CC             5 //set
#define STRX_GNDWDT_CLEAR_CC                  6 //set
#define STRX_REBOOT_CC                        7 //set
#define STRX_RXCONF_SET_BAUD_CC               8 //set
#define STRX_TXCONF_SET_BAUD_CC               9 //set
#define STRX_SET_DEFAULT_BAUD_CC              10 //set
#define STRX_RPARAM_SAVE_1_CC                 11 //set
#define STRX_RPARAM_SAVE_5_CC                 12 //set
#define STRX_RPARAM_SAVE_ALL_CC               13 //set
#define STRX_RXCONF_SET_FREQ_CC               14 //set
#define STRX_TXCONF_SET_FREQ_CC               15 //set
#define STRX_CHECK_STATE_PING_CC              32 //set


#define STRX_RXCONF_GET_BAUD_CC           16 //get
#define STRX_RXCONF_GET_GUARD_CC          17 //get
#define STRX_TXCONF_GET_BAUD_CC           18 //get
#define STRX_TLM_GET_TEMP_BRD_CC          19 //get
#define STRX_TLM_GET_LAST_RSSI_CC         20 //get
#define STRX_TLM_GET_LAST_RFERR_CC        21 //get
// #define STRX_TLM_GET_ACTIVE_CONF_CC       22 //get
#define STRX_TLM_GET_BOOT_COUNT_CC        23 //get
#define STRX_TLM_GET_BOOT_CAUSE_CC        24 //get
#define STRX_TLM_GET_LAST_CONTACT_CC      25 //get
#define STRX_TLM_GET_TOT_TX_BYTES_CC      26 //get
#define STRX_TLM_GET_TOT_RX_BYTES_CC      27 //get
#define STRX_GET_STATUS_CONFIGURATION_CC  28 //get
#define STRX_RXCONF_GET_FREQ_CC           29 //get
#define STRX_TXCONF_GET_FREQ_CC           30 //get
#define STRX_TLM_RXMODE_CC                32 //get
#define STRX_TLM_GET_GNDWDT_CNT_CC        33 //get
#define STRX_TLM_GET_GNDWDT_LEFT_CC       34 //get

#define STRX_SEND_HK_CC                   31//hk data

// Define CMD Structure




/*******************************************/
/*                                         */
/*              UANT (COSMIC)              */
/*    ISISpace Deployable Antenna System   */
/*                                         */
/*******************************************/

// Define MSGID

// Define Function Codes
#define UANT_APP_NOOP_CC                          1
#define UANT_APP_RESET_COUNTERS_CC                2
#define UANT_APP_RESET_CC                         3
#define UANT_APP_GET_DEPLOYMENT_STATUS_CC         4
#define UANT_APP_ARM_ANTENNA_SYSTEMS_CC           5
#define UANT_APP_DISARM_CC                        6
#define UANT_APP_AUTOMATED_DEPLOYMENT_CC          7
#define UANT_APP_CANCEL_DEPLOYMENT_ACTIVATION_CC  8
#define UANT_APP_DEPLOY_ANT1_CC                   9
#define UANT_APP_DEPLOY_ANT2_CC                   10
#define UANT_APP_DEPLOY_ANT3_CC                   11
#define UANT_APP_DEPLOY_ANT4_CC                   12
#define UANT_APP_DEPLOY_ANT1_OVERRIDE_CC          13
#define UANT_APP_DEPLOY_ANT2_OVERRIDE_CC          14
#define UANT_APP_DEPLOY_ANT3_OVERRIDE_CC          15
#define UANT_APP_DEPLOY_ANT4_OVERRIDE_CC          16
#define UANT_APP_REPORT_ANT_ACTIVATION_CNT_CC     17
#define UANT_APP_REPORT_ANT_ACTIVATION_TIME_CC    18
#define UANT_APP_MEASURE_SYSTEM_TEMPERATURE_CC    19

// Define CMD Structure
typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
} COS_UANT_NoArgsCmd_t;

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t                   Arg;
} COS_UANT_u8Cmd_t;



typedef COS_UANT_NoArgsCmd_t    COS_UANT_NoopCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_ResetCountersCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_ResetCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_GetDeploymentStatusCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_ArmAntennaSystemsCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_DisarmCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_CancelDeploymentActivationCmd_t;
typedef COS_UANT_NoArgsCmd_t    COS_UANT_MeasureSystemTemperatureCmd_t;



typedef COS_UANT_u8Cmd_t        COS_UANT_AutomatedDeploymentCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt1Cmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt2Cmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt3Cmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt4Cmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt1OverrideCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt2OverrideCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt3OverrideCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_DeployAnt4OverrideCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_ReportAntActivationCntCmd_t;
typedef COS_UANT_u8Cmd_t        COS_UANT_ReportAntActivationTimeCmd_t;




/*******************************************/
/*                                         */
/*              SANT (COSMIC)              */
/*        GomSpace NanoCom AM2150-O        */
/*                                         */
/*******************************************/
// Define MSGID

// Define Function Codes
#define SANT_APP_NOOP_CC                0
#define SANT_APP_RESET_COUNTERS_CC      1
#define SANT_APP_SOFT_REBOOT_CC         2
#define SANT_APP_BURN_CC                3
#define SANT_APP_STOP_BURN_CC           4
#define SANT_APP_GET_STATUS_CC          5
#define SANT_APP_GET_BACKUP_STATUS_CC   6
#define SANT_APP_GET_BOARD_STATUS_CC    7
#define SANT_APP_GET_TEMPERATURE_CC     8
#define SANT_APP_GET_SETTINGS_CC        9
#define SANT_APP_SET_SETTINGS_CC        10


// Define CMD Structure

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
} SANT_NoArgsCmd_t;

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
    uint8_t                   Arg;
} SANT_u8Cmd_t;

typedef struct {
    uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
    uint16_t                 min_deploy;
    uint8_t                  backup;
    uint8_t                  max_burn_duration;
} SANT_SetSettingsCmd_t;


typedef SANT_NoArgsCmd_t        SANT_NoopCmd_t;
typedef SANT_NoArgsCmd_t        SANT_ResetCountersCmd_t;
typedef SANT_NoArgsCmd_t        SANT_SoftRebootCmd_t;
typedef SANT_NoArgsCmd_t        SANT_StopBurnCmd_t;
typedef SANT_NoArgsCmd_t        SANT_GetStatusCmd_t;
typedef SANT_NoArgsCmd_t        SANT_GetBackupStatusCmd_t;
typedef SANT_NoArgsCmd_t        SANT_GetTemperatureCmd_t;
typedef SANT_NoArgsCmd_t        SANT_GetSettingsCmd_t;

typedef SANT_u8Cmd_t            SANT_BurnCmd_t;









// /*******************************************/
// /*                                         */
// /*               EPS (COSMIC)              */
// /*        ?????????????????????????        */
// /*                                         */
// /*******************************************/
// // Define MSGID

// // Define Function Codes

// /*
// ** EPS App command codes
// */
// #define EPS_NOOP_CC                 0
// #define EPS_RESET_COUNTERS_CC       1
// #define EPS_GET_COUNTERS_CC         2
// #define EPS_GET_APPDATA_CC          3

// /*
// ** P31u device & channel control
// */
// #define EPS_P31U_SET_OUT_SINGLE_CC  10
// #define EPS_P31U_SET_OUTPUTS_CC     11
// #define EPS_P31U_RESET_WDT_CC       20
// #define EPS_P31U_RESET_COUNTERS_CC  21
// #define EPS_P31U_HARD_RESET_CC      22

// /*
// ** P31u housekeeping requests
// */
// #define EPS_P31U_GETHK_ALL_CC       30
// #define EPS_P31U_GETHK_OUT_CC       31
// #define EPS_P31U_GETHK_VI_CC        32
// #define EPS_P31U_GETHK_WDT_CC       33
// #define EPS_P31U_GETHK_BASIC_CC     34
// #define EPS_P31U_GETHK_OLD_CC       35
// #define EPS_P31U_GETHK_CC           36

// /*
// ** P31u config commands
// */
// #define EPS_P31U_SET_PV_VOLT_CC     40
// #define EPS_P31U_SET_PV_AUTO_CC     41
// #define EPS_P31U_SET_HEATER_CC      42

// #define EPS_P31U_GET_CONFIG_CC      50
// #define EPS_P31U_SET_CONFIG_CC      51
// #define EPS_P31U_CONFIG_CC          52
// #define EPS_P31U_GET_CONFIG2_CC     53
// #define EPS_P31U_SET_CONFIG2_CC     54
// #define EPS_P31U_CONFIG2_CC         55
// #define EPS_P31U_SET_CONFIG3_CC     56

// /*
// ** Generic transaction (plumbing)
// */
// #define EPS_P31U_TRANSACTION_CC     99


// // Define CMD Structure
// typedef struct {
//     uint8_t  channel;
//     uint8_t  value;
//     uint16_t delay;
// } EPS_P31U_SetOutputSingle_Payload_t;

// typedef struct {
//     uint8_t  mask;
// } EPS_P31U_SetOutputs_Payload_t;

// typedef struct {
//     uint8_t  id;
// } EPS_P31U_GetHk_Payload_t;
 
// typedef struct {
//     int16_t  voltage[3];
// } EPS_P31U_SetPvVolt_Payload_t;

// typedef struct {
//     uint8_t  mode;
// } EPS_P31U_SetPvAuto_Payload_t;

// typedef struct {
//     uint8_t  cmd;
//     uint8_t  heater;
//     uint8_t  mode;
// } EPS_P31U_SetHeater_Payload_t;

// typedef struct {
//     uint8_t  ppt_mode;
//     uint8_t  battheater_mode;
//     int8_t   battheater_low;
//     int8_t   battheater_high;
//     uint8_t  output_normal_value[8];
//     uint8_t  output_safe_value[8];
//     uint16_t output_initial_on_delay[8];
//     uint16_t output_initial_off_delay[8];
//     uint16_t vboost[3];
// } EPS_P31U_SetConfig_Payload_t;

// typedef struct {
//     uint16_t batt_maxvoltage;
//     uint16_t batt_safevoltage;
//     uint16_t batt_criticalvoltage;
//     uint16_t batt_normalvoltage;
//     uint32_t reserved1[2];
//     uint8_t  reserved2[4];
// } EPS_P31U_SetConfig2_Payload_t;

// typedef struct {
//     uint8_t  version;
//     uint8_t  cmd;
//     uint8_t  length;
//     uint8_t  flags;
//     uint16_t cur_lim[8];
//     uint8_t  cur_ema_gain;
//     uint8_t  cspwdt_channel[2];
//     uint8_t  cspwdt_address[2];
// } EPS_P31U_SetConfig3_Payload_t;

// typedef struct {
//     uint8_t  port;
//     uint8_t  reserved;
//     uint8_t  txSize;
//     uint8_t  rxSize;
//     uint8_t  tx[128];
// } EPS_P31U_Transaction_Payload_t;

// /*************************************************************************/
// /*
// ** Type definition (EPS housekeeping)
// */

// typedef struct {

// } EPS_P31U_HkTlm_Payload_t;

// typedef struct SAMPLE_APP_HkTlm_Payload {
//     uint8_t CommandErrorCounter;
//     uint8_t CommandCounter;
//     uint8_t spare[2];
// } EPS_HkTlm_Payload_t;



// /**
//  * Noarg cmd template.
//  */
// typedef struct {
//     uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
// } EPS_NoArgCmd_t;

// typedef EPS_NoArgCmd_t  EPS_NoopCmd_t;
// typedef EPS_NoArgCmd_t  EPS_ResetCountersCmd_t;
// typedef EPS_NoArgCmd_t  EPS_GetCountersCmd_t;
// typedef EPS_NoArgCmd_t  EPS_GetAppDataCmd_t;

// typedef EPS_NoArgCmd_t  EPS_P31U_ResetCountersCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_ResetWdtCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_HardResetCmd_t;

// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkAllCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkOutCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkViCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkWdtCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkBasicCmd_t;
// typedef EPS_NoArgCmd_t  EPS_P31U_GetHkOldCmd_t;

// typedef struct {
//     uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
//     EPS_P31U_GetHk_Payload_t Payload;
// } EPS_P31U_GetHkCmd_t;

// typedef struct {
//     uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
//     EPS_P31U_SetOutputSingle_Payload_t Payload;
// } EPS_P31U_SetOutputSingleCmd_t;

// typedef struct {
//     uint8_t               CmdHeader[CFE_SB_CMD_HDR_SIZE];
//     EPS_P31U_SetOutputs_Payload_t Payload;
// } EPS_P31U_SetOutputsCmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetPvVolt_Payload_t Payload;
// } EPS_P31U_SetPvVoltCmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetPvAuto_Payload_t Payload;
// } EPS_P31U_SetPvAutoCmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetHeater_Payload_t Payload;
// } EPS_P31U_SetHeaterCmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetConfig_Payload_t Payload;
// } EPS_P31U_SetConfigCmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetConfig2_Payload_t Payload;
// } EPS_P31U_SetConfig2Cmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_SetConfig3_Payload_t Payload;
// } EPS_P31U_SetConfig3Cmd_t;

// typedef struct {
//     CFE_MSG_CommandHeader_t CommandHeader;
//     EPS_P31U_Transaction_Payload_t Payload;
// } EPS_P31U_TransactionCmd_t;


// /*************************************************************************/
// /*
// ** Type definition (EPS housekeeping)
// */
// typedef EPS_NoArgCmd_t  EPS_SendHkCmd_t;


// typedef struct {
//     CFE_MSG_TelemetryHeader_t  TelemetryHeader;
//     EPS_HkTlm_Payload_t Payload;
// } EPS_HkTlm_t;







#endif