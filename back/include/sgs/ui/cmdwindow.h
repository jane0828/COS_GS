#pragma once
#ifndef _SGS_CMDWINDOW_H_
#define _SGS_CMDWINDOW_H_
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <sgs/tc/adcs.h>
#include <sgs/tc/cmd.h>
#include <sgs/tc/eps.h>
#include <sgs/tc/es.h>
#include <sgs/tc/fm.h>
#include <sgs/tc/gps.h>
#include <sgs/tc/mtq.h>
#include <sgs/tc/pay.h>
#include <sgs/tc/rwa.h>
#include <sgs/tc/snsr.h>
#include <sgs/tc/stx.h>
#include <sgs/tc/to.h>
#include <sgs/tc/ts.h>
#include <sgs/tc/utrx.h>
#endif


typedef struct {
    ADCS_SetParamScCmd_t adcs_setparamsccmd;
    ADCS_SetParamTleCmd_t adcs_setparamtlecmd;
    ADCS_SetParamAuxCmd_t adcs_setparamauxcmd;
    ADCS_SetParamTargetCmd_t adcs_setparamtargetcmd;
    ADCS_SetParamGainCmd_t adcs_setparamgaincmd;
    ADCS_NoArgsCmd_t adcs_noargscmd;

    EPS_SetConfigurationParameterCmd_t eps_setconfig;
    EPS_ResetConfigurationParamterCmd_t eps_resetconfig;
    EPS_CorrectTimeCmd_t eps_correcttimecmd;
    EPS_OutputBusGroupOnCmd_t eps_outputbusgrouponcmd;
    EPS_OutputBusGroupOffCmd_t eps_outputbusgroupoffcmd;
    EPS_OutputBusGroupStateCmd_t eps_outputbusgroupstatecmd;
    EPS_OutputBusChannelOnCmd_t eps_outputbuschanneloncmd;
    EPS_OutputBusChannelOffCmd_t eps_outputbuschanneloffcmd;
    EPS_NoArgsCmd_t eps_noargscmd;

    FM_ResetAppCmd_t fm_resetappcmd;
    FM_ModeTransferCmd_t fm_modetransfercmd;
    FM_SetOperationModeCmd_t fm_setoperationmodecmd;
    FM_SetCommissioningPhaseCmd_t fm_setcommissioningphasecmd;
    FM_SetSpacecraftTimeCmd_t fm_setspacecrafttimecmd;
    FM_SetAntennaDeployFlagCmd_t fm_setantennadeployflagcmd;
    FM_SetDaylightDetectionFlagCmd_t fm_setdaylightdetectionflagcmd;
    FM_SetCommMissingFlagCmd_t fm_setcommmissingflagcmd;
    FM_NoArgsCmd_t fm_noargscmd;

    GPS_LogOnceCmd_t gps_logoncecmd;
    GPS_LogOntimeCmd_t gps_logontimecmd;
    GPS_LogOnChangeCmd_t gps_logonchangecmd;
    GPS_NoArgsCmd_t gps_noargscmd;

    MTQ_ResetCompCmd_t mtq_resetcompcmd;
    MTQ_EnableCmd_t mtq_enablecmd;
    MTQ_DisableCmd_t mtq_disablecmd;
    MTQ_SetPolarityCmd_t mtq_setpolaritycmd;
    MTQ_SetDutyCmd_t mtq_setdutycmd;
    MTQ_SetPeriodCmd_t mtq_setperiodcmd;
    MTQ_NoArgsCmd_t mtq_noargscmd;

    RWA_ResetWheelCmd_t rwa_resetwheelcmd;
    RWA_ClearErrorsCmd_t rwa_clearerrorscmd;
    RWA_SetMotorPowerStateCmd_t rwa_setmotorpowerstatecmd;
    RWA_SetEncoderPowerStateCmd_t rwa_setencoderpowerstatecmd;
    RWA_SetHallPowerStateCmd_t rwa_sethallpowerstatecmd;
    RWA_SetControlModeCmd_t rwa_setcontrolmodecmd;
    RWA_SetBackupWheelModeCmd_t rwa_setbackupwheelmodecmd;
    RWA_SetWheelReferenceSpeedCmd_t rwa_setwheelreferencespeedcmd;
    RWA_SetWheelCommandedTorqueCmd_t rwa_setwheelcommandedtorquecmd;
    RWA_SetPwmGainCmd_t rwa_setpwmgaincmd;
    RWA_SetMainGainCmd_t rwa_setmaingaincmd;
    RWA_SetBackupGainCmd_t rwa_setbackupgaincmd;
    RWA_SetWheelReferenceSpeedAllAxisCmd_t rwa_setwheelreferencespeedallaxiscmd;
    RWA_NoArgsCmd_t rwa_noargscmd;

    PAY_CamDownloadOldImgCmd_t pay_camdownloadoldimgcmd;
    PAY_CamSendNoargCmd_t pay_camsendnoargcmd;
    PAY_CamSetExposureCmd_t pay_camsetexposurecmd;
    PAY_CamSetScpdCmd_t pay_camsetscpdcmd;
    PAY_NoArgsCmd_t pay_noargscmd;

    SNSR_STT_BootCmd_t snsr_stt_bootcmd;
    SNSR_STT_PingCmd_t snsr_stt_pingcmd;
    SNSR_STT_InitDeviceCmd_t snsr_stt_initdevicecmd;
    SNSR_STT_UnlockCmd_t snsr_stt_unlockcmd;
    SNSR_STT_SetParamCmd_t snsr_stt_setparamcmd;
    SNSR_STT_SendRS485Cmd_t snsr_stt_sendrs485cmd;
    SNSR_MMT_SetInternalControl0Cmd_t snsr_mmt_setinternalcontrol0cmd;
    SNSR_MMT_SetInternalControl2Cmd_t snsr_mmt_setinternalcontrol2cmd;
    SNSR_MMT_WriteToRegisterCmd_t snsr_mmt_writetoregistercmd;
    SNSR_IMU_SetGyroOffsetCmd_t snsr_imu_setgyrooffsetcmd;
    SNSR_IMU_SetConfigurationCmd_t snsr_imu_setconfigurationcmd;
    SNSR_IMU_SetGyroConfigurationCmd_t snsr_imu_setgyroconfigurationcmd;
    SNSR_IMU_SetAccelConfigurationCmd_t snsr_imu_setaccelconfigurationcmd;
    SNSR_IMU_SetAccelConfiguration2Cmd_t snsr_imu_setaccelconfiguration2cmd;
    SNSR_IMU_SetPowerManagement1Cmd_t snsr_imu_setpowermanagement1cmd;
    SNSR_IMU_WriteToRegisterCmd_t snsr_imu_writetoregistercmd;
    SNSR_FSS_IsolateCmd_t snsr_fss_isolatedcmd;
    SNSR_FSS_RestoreCmd_t snsr_fss_restorecmd;
    SNSR_CSS_IsolateCmd_t snsr_css_isolatedcmd;
    SNSR_CSS_RestoreCmd_t snsr_css_restorecmd;
    SNSR_NoArgsCmd_t snsr_noargscmd;

    STX_TransmitDataCmd_t stx_transmitdatacmd;
    STX_SetControlModeCmd_t stx_setcontrolmodecmd;
    STX_SetEncoderRateCmd_t stx_setencoderratecmd;
    STX_SetPaPowerCmd_t stx_setpapowercmd;
    STX_SetSynthOffsetCmd_t stx_setsynthoffsetcmd;
    STX_TransmitFileCmd_t stx_transmitfilecmd;
    STX_TransmitFileLongPathCmd_t stx_transmitfilelongpathcmd;
    STX_NoArgsCmd_t stx_noargscmd;

    TO_DownlinkQueryReplyCmd_Payload_t to_downlinkqueryreplypayloadcmd;
    TO_DownlinkQueryReplyCmd_t to_downlinkqueryreplycmd;
    TO_NoArgsCmd_t to_noargscmd;
    TO_DisableBeaconCmd_t to_disablebeaconcmd;

    UTRX_SetTxFreqCmd_t utrx_settxfreqcmd;
    UTRX_SetTxBaudCmd_t utrx_settxbaudcmd;
    UTRX_SetTxModIndexCmd_t utrx_settxmodindex;
    UTRX_SetTxModeCmd_t utrx_settxmodecmd;
    UTRX_SetRxFreqCmd_t utrx_setrxfreqcmd;
    UTRX_SetRxBaudCmd_t utrx_setrxbaudcmd;
    UTRX_SetRxModIndexCmd_t utrx_setrxmodindexcmd;
    UTRX_SetRxModeCmd_t utrx_setrxmodecmd;
    UTRX_SetRxBandwidthCmd_t utrx_setrxbandwidthcmd;
    UTRX_NoArgsCmd_t utrx_noargscmd;

    TS_InsertScheduleEntryCmd_t ts_insertscheduleentrycmd;
    TS_ClearScheduleEntryCmd_t ts_clearscheduleentrycmd;
    TS_ClearScheduleGroupCmd_t ts_clearschedulegroupcmd;
    TS_NoArgsCmd_t ts_noargscmd;

    ECM_GetHKAvgCmd_t ecm_gethkavgcmd;
    ECM_GetSystemStatusCmd_t ecm_getsystemstatuscmd;
    ECM_GetOcfStateCmd_t ecm_getocfstatecmd;
    ECM_Read_t ecm_readcmd;
    ECM_NoArgsCmd_t ecm_noargscmd;

    CFE_ES_RestartCmd_t es_restartcmd;
    CFE_ES_AppNameCmd_t es_restartappcmd;
    CFE_ES_StopAppCmd_t es_stopappcmd;
    CFE_ES_StartAppCmd_t es_startappcmd;
    CFE_ES_NoArgsCmd_t es_noargscmd;

    SCH_EntryCmd_t sch_enablecmd;
    SCH_EntryCmd_t sch_disablecmd;
    SCH_NoArgsCmd_t sch_noargscmd;
} Command;

class SGS_CMDWINDOW
{
private:
	bool _open;
	float _x_pos, _y_pos, _width, _height = 0;

	bool ChecksumState = false;
	bool SchedulerState = false;
	uint32_t ExecutionTimeBuf = 0;
	uint32_t ExecutionWindowBuf = 0;
	uint16_t EntryIDBuf = 0;
	uint16_t GroupIDBuf = 0;
	uint32_t gen_msgid = 0;
	uint16_t gen_fnccode = 0;
	uint32_t NowCMD = 0;
    Command* command = nullptr;
    char SendingLabelBuf[64];
    char DeleteLabelBuf[64];
    uint8_t _msgidbuf[2];
    uint16_t _msgidinfo = 0;

	char ComboboxLabel[32];
    char RepeatedLabel[64];
	char ADCSCMDLabels[128][64];
	char CICMDLabels[128][64];
	char EPSCMDLabels[128][64];
	char FMCMDLabels[128][64];
	char GPSCMDLabels[128][64];
	char MTQCMDLabels[128][64];
	char PAYCMDLabels[128][64];
	char RWACMDLabels[128][64];
	char SNSRCMDLabels[128][64];
	char STXCMDLabels[128][64];
	char TOCMDLabels[128][64];
	char UTRXCMDLabels[128][64];
	char TSCMDLabels[128][64];
	char ESCMDLabels[128][64];
	char SCHCMDLabels[128][64];
	char ECMCMDLabels[128][64];

    

public:
    cmd_packet_t * shellcmd;
	ImGuiWindowFlags WindowFlag;
	float FontScale;
    float CMDField = 0.4;

	SGS_CMDWINDOW();
	~SGS_CMDWINDOW();

    void Initialize_CMDLabels();
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	int CMDDataGenerator(uint32_t msgid, uint16_t fnccode, void* Requested, size_t RequestedSize);
	void Render();

};
#endif