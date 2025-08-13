#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_CMDWINDOW::SGS_CMDWINDOW()
{
    this->_open = true;
    this->command = new Command;
    memset(this->command, 0, sizeof(Command));
    memset(this->SendingLabelBuf, 0, sizeof(this->SendingLabelBuf));
    memset(this->DeleteLabelBuf, 0, sizeof(this->DeleteLabelBuf));
    memset(this->_msgidbuf, 0, sizeof(this->_msgidbuf));
    memset(this->ComboboxLabel, 0, sizeof(this->ComboboxLabel));
    memset(this->RepeatedLabel, 0, sizeof(this->RepeatedLabel));
    memset(this->ADCSCMDLabels, 0, sizeof(this->ADCSCMDLabels));
    memset(this->CICMDLabels, 0, sizeof(this->CICMDLabels));
    memset(this->EPSCMDLabels, 0, sizeof(this->EPSCMDLabels));
    memset(this->FMCMDLabels, 0, sizeof(this->FMCMDLabels));
    memset(this->GPSCMDLabels, 0, sizeof(this->GPSCMDLabels));
    memset(this->MTQCMDLabels, 0, sizeof(this->MTQCMDLabels));
    memset(this->PAYCMDLabels, 0, sizeof(this->PAYCMDLabels));
    memset(this->RWACMDLabels, 0, sizeof(this->RWACMDLabels));
    memset(this->SNSRCMDLabels, 0, sizeof(this->SNSRCMDLabels));
    memset(this->STXCMDLabels, 0, sizeof(this->STXCMDLabels));
    memset(this->TOCMDLabels, 0, sizeof(this->TOCMDLabels));
    memset(this->UTRXCMDLabels, 0, sizeof(this->UTRXCMDLabels));
    memset(this->TSCMDLabels, 0, sizeof(this->TSCMDLabels));
    memset(this->ESCMDLabels, 0, sizeof(this->ESCMDLabels));
    memset(this->SCHCMDLabels, 0, sizeof(this->SCHCMDLabels));
    memset(this->ECMCMDLabels, 0, sizeof(this->ECMCMDLabels));
    this->Initialize_CMDLabels();
}

SGS_CMDWINDOW::~SGS_CMDWINDOW()
{
    this->_open = false;
}

void SGS_CMDWINDOW::Initialize_CMDLabels()
{
    snprintf(ADCSCMDLabels[ADCS_NOOP_CC], 64, "Noop CMD");
    snprintf(ADCSCMDLabels[ADCS_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(ADCSCMDLabels[ADCS_PROCESS_CC], 64, "Process");
    snprintf(ADCSCMDLabels[ADCS_SETPARAM_SC], 64, "Set Parameter SC");
    snprintf(ADCSCMDLabels[ADCS_SETPARAM_AUX], 64, "Set Parameter AUX");
    snprintf(ADCSCMDLabels[ADCS_SETPARAM_GAIN], 64, "Set Parameter Gain");
    snprintf(ADCSCMDLabels[ADCS_SETPARAM_TLE], 64, "Set Parameter TLE");
    snprintf(ADCSCMDLabels[ADCS_SETPARAM_TARGET], 64, "Set Parameter Target");

    snprintf(CICMDLabels[CI_NOOP_CC], 64, "Noop CMD");
    snprintf(CICMDLabels[CI_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(CICMDLabels[CI_RESERVED_CC], 64, "Reserved");

    snprintf(EPSCMDLabels[EPS_NOOP_CC], 64, "Noop CMD");
    snprintf(EPSCMDLabels[EPS_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(EPSCMDLabels[EPS_RESERVED_CC], 64, "Reserved");
    snprintf(EPSCMDLabels[EPS_SYSTEM_RESET_CC], 64, "System Reset");
    snprintf(EPSCMDLabels[EPS_HW_NOOP_CC], 64, "HW Noop");
    snprintf(EPSCMDLabels[EPS_CANCEL_OPERATION_CC], 64, "Cancel Operation");
    snprintf(EPSCMDLabels[EPS_WATCHDOG_CC], 64, "Watchdog");
    snprintf(EPSCMDLabels[EPS_CORRECT_TIME_CC], 64, "Correct Time");
    snprintf(EPSCMDLabels[EPS_OUTPUT_BUS_GROUP_ON_CC], 64, "Output Bus Group On");
    snprintf(EPSCMDLabels[EPS_OUTPUT_BUS_GROUP_OFF_CC], 64, "Output Bus Group Off");
    snprintf(EPSCMDLabels[EPS_OUTPUT_BUS_GROUP_STATE_CC], 64, "Output Bus Group State");
    snprintf(EPSCMDLabels[EPS_OUTPUT_BUS_CHANNEL_ON_CC], 64, "Output Bus Channel On");
    snprintf(EPSCMDLabels[EPS_OUTPUT_BUS_CHANNEL_OFF_CC], 64, "Output Bus Channel Off");
    snprintf(EPSCMDLabels[EPS_SWITCH_TO_NOMINAL_MODE_CC], 64, "Switch to Nominal Mode");
    snprintf(EPSCMDLabels[EPS_SWITCH_TO_SAFETY_MODE_CC], 64, "Switch to Safety Mode");
    snprintf(EPSCMDLabels[EPS_SET_CONFIG_PARAMETER_CC], 64, "Set Config Parameter");
    snprintf(EPSCMDLabels[EPS_RESET_CONFIG_PARAMETER_CC], 64, "Reset Config Parameter");
    snprintf(EPSCMDLabels[EPS_RESET_CONFIG_CC], 64, "Reset Config");
    snprintf(EPSCMDLabels[EPS_LOAD_CONFIG_CC], 64, "Load Config");
    snprintf(EPSCMDLabels[EPS_SAVE_CONFIG_CC], 64, "Save Config");

    snprintf(FMCMDLabels[FM_NOOP_CC], 64, "Noop CMD");
    snprintf(FMCMDLabels[FM_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(FMCMDLabels[FM_RESERVED_CC], 64, "Reserved");
    snprintf(FMCMDLabels[FM_RESET_FM_CC], 64, "Reset FM");
    snprintf(FMCMDLabels[FM_RESET_APP_CC], 64, "Reset APP");
    snprintf(FMCMDLabels[FM_RESET_PROCESSOR_CC], 64, "Reset Processor");
    snprintf(FMCMDLabels[FM_RESET_POWER_CC], 64, "Reset Power");
    snprintf(FMCMDLabels[FM_MODE_TRANSFER_CC], 64, "Mode Transfer");
    snprintf(FMCMDLabels[FM_TERMINATE_EO_CC], 64, "Terminate EO");
    snprintf(FMCMDLabels[FM_TERMINATE_EO_OVERRIDE_CC], 64, "Terminate EO Override");
    snprintf(FMCMDLabels[FM_START_OBSERVATION_CC], 64, "Start Observation");
    snprintf(FMCMDLabels[FM_START_COMMUNICATION_CC], 64, "Start Communication");
    snprintf(FMCMDLabels[FM_START_COMMISSIONING_CC], 64, "Start Commissioning");
    snprintf(FMCMDLabels[FM_SET_OPERATION_MODE_CC], 64, "Set Operation Mode");
    snprintf(FMCMDLabels[FM_SET_COMMISSIONING_PHASE_CC], 64, "Set Commissioning Phase");
    snprintf(FMCMDLabels[FM_INITIATE_BASELINE_CC], 64, "Initiate Baseline");
    snprintf(FMCMDLabels[FM_SET_SPACECRAFT_TIME_CC], 64, "Set Spacecraft Time");
    snprintf(FMCMDLabels[FM_DEPLOY_ANTENNA_CC], 64, "Deploy Antenna");
    snprintf(FMCMDLabels[FM_USE_NOMINAL_BAUDRATES_CC], 64, "Use Nomial Baudrates");
    snprintf(FMCMDLabels[FM_SET_ANTENNA_DEPLOY_FLAG_CC], 64, "Set Antenna Deploy Flag");
    snprintf(FMCMDLabels[FM_SET_DAYLIGHT_DETECTION_FLAG_CC], 64, "Set Daylight Detection Flag");
    snprintf(FMCMDLabels[FM_SET_COMM_MISSING_FLAG_CC], 64, "Set COMM Missing Flag");
    snprintf(FMCMDLabels[FM_STORE_OBSDATA_TO_CDS_CC], 64, "Store Obsdata to CDS");
    snprintf(FMCMDLabels[FM_RESTORE_OBSDATA_FROM_CDS_CC], 64, "Restore Obsdata from CDS");
    snprintf(FMCMDLabels[FM_STORE_OBSDATA_TO_SD_CC], 64, "Store Obsdata to SD");
    snprintf(FMCMDLabels[FM_RESTORE_OBSDATA_FROM_SD_CC], 64, "Restore Obsdata from SD");

    snprintf(GPSCMDLabels[GPS_NOOP_CC], 64, "Noop CMD");
    snprintf(GPSCMDLabels[GPS_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(GPSCMDLabels[GPS_RESERVED_CC], 64, "Reserved");
    snprintf(GPSCMDLabels[GPS_RESET_APP_CC], 64, "Reset App");
    snprintf(GPSCMDLabels[GPS_RESET_HW_CC], 64, "Reset HW");
    snprintf(GPSCMDLabels[GPS_CLEAR_LOGS_CC], 64, "Clear Logs");
    snprintf(GPSCMDLabels[GPS_ENABLE_TIME_TONE_CC], 64, "Enable Time Tone");
    snprintf(GPSCMDLabels[GPS_DISABLE_TIME_TONE_CC], 64, "Disable Time Tone");
    snprintf(GPSCMDLabels[GPS_LOG_ONCE_CC], 64, "Log Once");
    snprintf(GPSCMDLabels[GPS_LOG_ONTIME_CC], 64, "Log Ontime");
    snprintf(GPSCMDLabels[GPS_LOG_ONCHANGE_CC], 64, "Log Onchange");
    snprintf(GPSCMDLabels[GPS_LOG_REQUEST_DFT_CC], 64, "Log Request DFT");
    snprintf(GPSCMDLabels[GPS_SET_IF_MODE_CC], 64, "Set If Mode");

    snprintf(MTQCMDLabels[MTQ_NOOP_CC], 64, "Noop CMD");
    snprintf(MTQCMDLabels[MTQ_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(MTQCMDLabels[MTQ_RESERVED_CC], 64, "Reserved");
    snprintf(MTQCMDLabels[MTQ_RESET], 64, "Reset");
    snprintf(MTQCMDLabels[MTQ_RESET_COMP], 64, "Reset Comp");
    snprintf(MTQCMDLabels[MTQ_SET_DUTY], 64, "Set Duty");
    snprintf(MTQCMDLabels[MTQ_ENABLE], 64, "Enable");
    snprintf(MTQCMDLabels[MTQ_DISABLE], 64, "Disable");
    snprintf(MTQCMDLabels[MTQ_SET_PERIOD], 64, "Set Period");
    snprintf(MTQCMDLabels[MTQ_SET_POLARITY], 64, "Set Polarity");

    snprintf(PAYCMDLabels[PAY_NOOP_CC], 64, "Noop CMD");
    snprintf(PAYCMDLabels[PAY_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(PAYCMDLabels[PAY_PROCESS_CC], 64, "Process");
    snprintf(PAYCMDLabels[PAY_CAM_FIND_CC], 64, "Cam Find");
    snprintf(PAYCMDLabels[PAY_CAM_CONNECT_CC], 64, "Cam Connect");
    snprintf(PAYCMDLabels[PAY_CAM_START_OPERATION_CC], 64, "Cam Start Operation");
    snprintf(PAYCMDLabels[PAY_CAM_STOP_OPERATION_CC], 64, "Cam Stop Operation");
    snprintf(PAYCMDLabels[PAY_CAM_DOWNLOAD_NEW_IMG_CC], 64, "Cam Download New Image");
    snprintf(PAYCMDLabels[PAY_CAM_DOWNLOAD_OLD_IMG_CC], 64, "Cam Download old Image");
    snprintf(PAYCMDLabels[PAY_CAM_SEND_NOARG_CC], 64, "Cam Send No Arg");
    snprintf(PAYCMDLabels[PAY_CAM_SET_EXPOSURE_CC], 64, "Cam Set Explosure");
    snprintf(PAYCMDLabels[PAY_CAM_SET_SCPD_CC], 64, "Cam Set SCPD");
    snprintf(PAYCMDLabels[PAY_BBE_SHUTDOWN_CC], 64, "BBE Shutdown");
    snprintf(PAYCMDLabels[PAY_INIT_DEVICE_CC], 64, "Init Device");

    snprintf(RWACMDLabels[RWA_NOOP_CC], 64, "Noop CMD");
    snprintf(RWACMDLabels[RWA_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(RWACMDLabels[RWA_RESERVED_CC], 64, "Reserved");
    snprintf(RWACMDLabels[RWA_RESET_ALL_CC], 64, "Reset All");
    snprintf(RWACMDLabels[RWA_RESET_WHEEL_CC], 64, "Reset Wheel");
    snprintf(RWACMDLabels[RWA_SET_WHEEL_REFERENCE_SPEED_CC], 64, "Set Wheel Reference Speed");
    snprintf(RWACMDLabels[RWA_SET_WHEEL_REFERENCE_SPEED_ALL_AXIS_CC], 64, "Set Wheel Reference Speed All Axis");
    snprintf(RWACMDLabels[RWA_SET_WHEEL_COMMANDED_TORQUE_CC], 64, "Set Wheel Commanded Torque");
    snprintf(RWACMDLabels[RWA_SET_MOTOR_POWER_STATE_CC], 64, "Set Motor Power State");
    snprintf(RWACMDLabels[RWA_SET_ENCODER_POWER_STATE_CC], 64, "Set Encoder Power State");
    snprintf(RWACMDLabels[RWA_SET_HALL_POWER_STATE_CC], 64, "Set Hall Power State");
    snprintf(RWACMDLabels[RWA_SET_CONTROL_MODE_CC], 64, "Set Control Mode");
    snprintf(RWACMDLabels[RWA_SET_BACKUP_WHEEL_MODE_CC], 64, "Set Backup Wheel Mode");
    snprintf(RWACMDLabels[RWA_SET_CLEAR_ERRORS_CC], 64, "Set Clear Errors");
    snprintf(RWACMDLabels[RWA_SET_PWM_GAIN_CC], 64, "Set PWM Gain");
    snprintf(RWACMDLabels[RWA_SET_MAIN_GAIN_CC], 64, "Set Main Gain");
    snprintf(RWACMDLabels[RWA_SET_BACKUP_GAIN_CC], 64, "Set Backup Gain");

    snprintf(SNSRCMDLabels[SNSR_NOOP_CC], 64, "Noop CMD");
    snprintf(SNSRCMDLabels[SNSR_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(SNSRCMDLabels[SNSR_RESERVED_CC], 64, "Reserved");
    snprintf(SNSRCMDLabels[SNSR_SUN_ALARM_OFF_CC], 64, "Sun Alarm Off");
    snprintf(SNSRCMDLabels[SNSR_STT_BOOT_CC], 64, "STT Boot");
    snprintf(SNSRCMDLabels[SNSR_STT_PING_CC], 64, "STT Ping");
    snprintf(SNSRCMDLabels[SNSR_STT_UNLOCK_CC], 64, "STT Unlock");
    snprintf(SNSRCMDLabels[SNSR_STT_REBOOT_CC], 64, "STT Reboot");
    snprintf(SNSRCMDLabels[SNSR_STT_SET_PARAMS_DFT_CC], 64, "STT Set Params DFT");
    snprintf(SNSRCMDLabels[SNSR_STT_SET_PARAM_CC], 64, "STT Set Params");
    snprintf(SNSRCMDLabels[SNSR_STT_SEND_RS485_CC], 64, "STT Send RS485");
    snprintf(SNSRCMDLabels[SNSR_STT_INIT_DEVICE_CC], 64, "STT Init Device");
    snprintf(SNSRCMDLabels[SNSR_MMT_RESET_CC], 64, "MMT Reset");
    snprintf(SNSRCMDLabels[SNSR_MMT_SET_INTERNAL_CONTROL0_CC], 64, "MMT Set Internal Control 0");
    snprintf(SNSRCMDLabels[SNSR_MMT_SET_INTERNAL_CONTROL2_CC], 64, "MMT Set Internal Control 2");
    snprintf(SNSRCMDLabels[SNSR_MMT_WRITE_TO_REGISTER_CC], 64, "MMT Write to Register");
    snprintf(SNSRCMDLabels[SNSR_IMU_RESET_CC], 64, "IMU Reset");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_GYRO_OFFSET_CC], 64, "IMU Set Gyro Offset");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_CONFIGURATION_CC], 64, "IMU Set Configuration");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_GYRO_CONFIGURATION_CC], 64, "IMU Set Gyro Configuration");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_ACCEL_CONFIGURATION_CC], 64, "IMU Set Acceleration Configuration");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_ACCEL_CONFIGURATION2_CC], 64, "IMU Set Acceleration Configuration 2");
    snprintf(SNSRCMDLabels[SNSR_IMU_SET_POWER_MANAGEMENT_CC], 64, "IMU Set Power Management");
    snprintf(SNSRCMDLabels[SNSR_IMU_WRITE_TO_REGISTER_CC], 64, "IMU Write to Register");
    snprintf(SNSRCMDLabels[SNSR_IMU_ISOLATE_CC], 64, "IMU Isolate");
    snprintf(SNSRCMDLabels[SNSR_IMU_RESTORE_CC], 64, "IMU Restore");
    snprintf(SNSRCMDLabels[SNSR_MMT_ISOLATE_CC], 64, "MMT Isolate");
    snprintf(SNSRCMDLabels[SNSR_MMT_RESTORE_CC], 64, "MMT Restore");
    snprintf(SNSRCMDLabels[SNSR_FSS_ISOLATE_CC], 64, "FSS Isolate");
    snprintf(SNSRCMDLabels[SNSR_FSS_RESTORE_CC], 64, "FSS Restore");
    snprintf(SNSRCMDLabels[SNSR_CSS_ISOLATE_CC], 64, "CSS Isolate");
    snprintf(SNSRCMDLabels[SNSR_CSS_RESTORE_CC], 64, "CSS Restore");
    snprintf(SNSRCMDLabels[SNSR_STT_ISOLATE_CC], 64, "STT Isolate");
    snprintf(SNSRCMDLabels[SNSR_STT_RESTORE_CC], 64, "STT Restore");
    snprintf(SNSRCMDLabels[SNSR_IMU_WHO_I_AM_CC], 64, "IMU Who I Am");
    snprintf(SNSRCMDLabels[SNSR_MMT_GET_PRODUCT_ID_CC], 64, "MMT Get Product ID");

    snprintf(STXCMDLabels[STX_NOOP_CC], 64, "Noop CMD");
    snprintf(STXCMDLabels[STX_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(STXCMDLabels[STX_PROCESS_CC], 64, "Process");
    snprintf(STXCMDLabels[STX_RESET_CC], 64, "Reset");
    snprintf(STXCMDLabels[STX_SET_CONTROL_MODE_CC], 64, "Set Control Mode");
    snprintf(STXCMDLabels[STX_SET_ENCODER_RATE_CC], 64, "Set Encoder Rate");
    snprintf(STXCMDLabels[STX_SET_PA_POWER_CC], 64, "Set PA Power");
    snprintf(STXCMDLabels[STX_SET_SYNTH_OFFSET_CC], 64, "Set Synth Offset");
    snprintf(STXCMDLabels[STX_TRANSMIT_READY_CC], 64, "Transmit Ready");
    snprintf(STXCMDLabels[STX_TRANSMIT_DATA_CC], 64, "Transmit Data");
    snprintf(STXCMDLabels[STX_TRANSMIT_FILE_CC], 64, "Transmit File");
    snprintf(STXCMDLabels[STX_TRANSMIT_FILE_LONG_PATH_CC], 64, "Transmit File Log Path");
    snprintf(STXCMDLabels[STX_TRANSMIT_END_CC], 64, "Transmit End");

    snprintf(TOCMDLabels[TO_NOOP_CC], 64, "Noop CMD");
    snprintf(TOCMDLabels[TO_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(TOCMDLabels[TO_PROCESS_CC], 64, "Process");
    snprintf(TOCMDLabels[TO_ENABLE_BEACON_CC], 64, "Enable Beacon");
    snprintf(TOCMDLabels[TO_DISABLE_BEACON_CC], 64, "Disable Beacon");
    snprintf(TOCMDLabels[TO_DOWNLINK_QUERY_REPLY_CC], 64, "Downlink Query Reply");
    snprintf(TOCMDLabels[TO_ENABLE_CC], 64, "Enable");

    snprintf(UTRXCMDLabels[UTRX_NOOP_CC], 64, "Noop CMD");
    snprintf(UTRXCMDLabels[UTRX_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(UTRXCMDLabels[UTRX_RESERVED_CC], 64, "Reserved");
    snprintf(UTRXCMDLabels[UTRX_REBOOT_CC], 64, "Reboot");
    snprintf(UTRXCMDLabels[UTRX_SET_TX_FREQ_CC], 64, "Set TX Frequency");
    snprintf(UTRXCMDLabels[UTRX_SET_TX_BAUD_CC], 64, "Set TX Baudrate");
    snprintf(UTRXCMDLabels[UTRX_SET_TX_MODIND_CC], 64, "Set TX Mod Index");
    snprintf(UTRXCMDLabels[UTRX_SET_TX_MODE_CC], 64, "Set TX Mode");
    snprintf(UTRXCMDLabels[UTRX_SET_RX_FREQ_CC], 64, "Set RX Frequency");
    snprintf(UTRXCMDLabels[UTRX_SET_RX_BAUD_CC], 64, "Set RX Baudrate");
    snprintf(UTRXCMDLabels[UTRX_SET_RX_MODIND_CC], 64, "Set RX Mode Index");
    snprintf(UTRXCMDLabels[UTRX_SET_RX_MODE_CC], 64, "Set RX Mode");
    snprintf(UTRXCMDLabels[UTRX_SET_RX_BW_CC], 64, "Set RX Bandwidth");
    snprintf(UTRXCMDLabels[UTRX_STUB_PING_CC], 64, "Stup Ping");

    snprintf(TSCMDLabels[TS_NOOP_CC], 64, "Noop CMD");
    snprintf(TSCMDLabels[TS_RESET_COUNTERS_CC], 64, "Reset Counters");
    snprintf(TSCMDLabels[TS_PROCESS_CC], 64, "Process");
    snprintf(TSCMDLabels[TS_INSERT_SCHEDULE_ENTRY_CC], 64, "Insert Schedule Entry");
    snprintf(TSCMDLabels[TS_CLEAR_SCHEDULE_ENTRY_CC], 64, "Clear Schedule Entry");
    snprintf(TSCMDLabels[TS_CLEAR_SCHEDULE_GROUP_CC], 64, "Clear Schedule Group");
    snprintf(TSCMDLabels[TS_CLEAR_ALL_SCHEDULE_CC], 64, "Clear All Schedule");

    snprintf(ECMCMDLabels[ECM_GET_HK_AVG_CC], 64, "Get HK AVG");
    snprintf(ECMCMDLabels[ECM_GET_SYSTEM_STATUS_CC], 64, "Get System Status");
    snprintf(ECMCMDLabels[ECM_GET_OCF_STATE_CC], 64, "Get OCF State");
    snprintf(ECMCMDLabels[ECM_READ_CC], 64, "Read");


    snprintf(ESCMDLabels[CFE_ES_RESTART_CC], 64, "Restart");
    snprintf(ESCMDLabels[CFE_ES_RESTART_APP_CC], 64, "Restart App");
    snprintf(ESCMDLabels[CFE_ES_STOP_APP_CC], 64, "Stop App");
    snprintf(ESCMDLabels[CFE_ES_START_APP_CC], 64, "Start App");



    snprintf(SCHCMDLabels[SCH_ENABLE_CC], 64, "SCH Enable");
    snprintf(SCHCMDLabels[SCH_DISABLE_CC], 64, "SCH Disable");
}

int SGS_CMDWINDOW::CMDDataGenerator(uint32_t msgid, uint16_t fnccode, void* Requested, size_t RequestedSize)
{
    ImGui::Checkbox("Checksum", &this->ChecksumState);
    ImGui::SameLine();
    ImGui::Checkbox("Scheduler", &this->SchedulerState);
    if (this->SchedulerState)
    {
        this->ChecksumState = true;
        ImGui::Text("Execution Time     : ");
        ImGui::SameLine();
        ImGui::InputScalar("##ExecutionTime", ImGuiDataType_U32, &this->ExecutionTimeBuf, NULL, NULL, "%u");
        ImGui::Text("Execution Window   : ");
        ImGui::SameLine();
        ImGui::InputScalar("##ExecutionWindow", ImGuiDataType_U32, &this->ExecutionWindowBuf, NULL, NULL, "%u");
        ImGui::Text("Entry ID           : ");
        ImGui::SameLine();
        ImGui::InputScalar("##EntryID", ImGuiDataType_U16, &this->EntryIDBuf, NULL, NULL, "%u");
        ImGui::Text("GroupID            : ");
        ImGui::SameLine();
        ImGui::InputScalar("##GroupID", ImGuiDataType_U16, &this->GroupIDBuf, NULL, NULL, "%u");
    }
    if (ImGui::Button("Generate", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        SatCMD[this->NowCMD] = new CmdGenerator_GS();
        memset(SatCMD[this->NowCMD], 0, sizeof(SatCMD[this->NowCMD]));
        console->AddLog(SGS_CONMOD_DBG, "[DEBUG]##Generate : %u, %u, %d\n", msgid, fnccode, RequestedSize);
        if (RequestedSize == sizeof(CFE_MSG_CommandHeader))
            SatCMD[this->NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, NULL);
        else
            SatCMD[this->NowCMD]->GenerateCmdHeader(msgid, fnccode, RequestedSize, (void *)((int)Requested + 8));
        if (this->SchedulerState)
        {
            SatCMD[this->NowCMD]->Scheduled = true;
            SatCMD[this->NowCMD]->Scheduling(this->ExecutionTimeBuf, this->ExecutionWindowBuf, this->EntryIDBuf, this->GroupIDBuf);
        }
        else if (!this->ChecksumState)
        {
            SatCMD[this->NowCMD]->Checksum = false;
        }

        this->NowCMD++;

        console->AddLog(SGS_CONMOD_LOG, "Generated Command Message.");
        memset(Requested, 0, RequestedSize);
        this->ChecksumState = true;
        this->SchedulerState = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        memset(Requested, 0, RequestedSize);
        return 0;
    }
    return 1;

}

void SGS_CMDWINDOW::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}

void SGS_CMDWINDOW::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("Propagator##SGS_PropagatorWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CMDTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("ADCS"))
        {
            ImGui::BeginChild("##cmdtitle_adcs", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != ADCS_CMD_MID)
            {
                this->gen_msgid = ADCS_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, ADCSCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(ADCSCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(ADCSCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
            switch (this->gen_fnccode)
            {
            case ADCS_SETPARAM_SC: {
                ImGui::PushItemWidth(200);
                ImGui::Text("Contact to Manager. This packet is too long.");
                ImGui::Text("Mass : ");
                ImGui::SameLine();
                ImGui::InputDouble("##mass", &command->adcs_setparamsccmd.Params.mass);
                ImGui::Text("MOI : ");
                ImGui::Text("[0][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[0][0]", &command->adcs_setparamsccmd.Params.MOI[0][0]);
                ImGui::SameLine();
                ImGui::Text("[0][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[0][1]", &command->adcs_setparamsccmd.Params.MOI[0][1]);
                ImGui::SameLine();
                ImGui::Text("[0][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[0][2]", &command->adcs_setparamsccmd.Params.MOI[0][2]);
                ImGui::Text("[1][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[1][0]", &command->adcs_setparamsccmd.Params.MOI[1][0]);
                ImGui::SameLine();
                ImGui::Text("[1][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[1][1]", &command->adcs_setparamsccmd.Params.MOI[1][1]);
                ImGui::SameLine();
                ImGui::Text("[1][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[1][2]", &command->adcs_setparamsccmd.Params.MOI[1][2]);
                ImGui::Text("[2][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[2][0]", &command->adcs_setparamsccmd.Params.MOI[2][0]);
                ImGui::SameLine();
                ImGui::Text("[2][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[2][1]", &command->adcs_setparamsccmd.Params.MOI[2][1]);
                ImGui::SameLine();
                ImGui::Text("[2][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##moi[2][2]", &command->adcs_setparamsccmd.Params.MOI[2][2]);
                ImGui::Text("Inv MOI : ");
                ImGui::Text("[0][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[0][0]", &command->adcs_setparamsccmd.Params.inv_MOI[0][0]);
                ImGui::SameLine();
                ImGui::Text("[0][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[0][1]", &command->adcs_setparamsccmd.Params.inv_MOI[0][1]);
                ImGui::SameLine();
                ImGui::Text("[0][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[0][2]", &command->adcs_setparamsccmd.Params.inv_MOI[0][2]);
                ImGui::Text("[1][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[1][0]", &command->adcs_setparamsccmd.Params.inv_MOI[1][0]);
                ImGui::SameLine();
                ImGui::Text("[1][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[1][1]", &command->adcs_setparamsccmd.Params.inv_MOI[1][1]);
                ImGui::SameLine();
                ImGui::Text("[1][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[1][2]", &command->adcs_setparamsccmd.Params.inv_MOI[1][2]);
                ImGui::Text("[2][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[2][0]", &command->adcs_setparamsccmd.Params.inv_MOI[2][0]);
                ImGui::SameLine();
                ImGui::Text("[2][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[2][1]", &command->adcs_setparamsccmd.Params.inv_MOI[2][1]);
                ImGui::SameLine();
                ImGui::Text("[2][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##invmoi[2][2]", &command->adcs_setparamsccmd.Params.inv_MOI[2][2]);
                ImGui::Text("CSS SF : ");
                ImGui::Text("[0]");
                ImGui::SameLine();
                ImGui::InputDouble("##css_sf[0]", &command->adcs_setparamsccmd.Params.css_sf[0]);
                ImGui::SameLine();
                ImGui::Text("[1]");
                ImGui::SameLine();
                ImGui::InputDouble("##css_sf[0]", &command->adcs_setparamsccmd.Params.css_sf[1]);
                ImGui::SameLine();
                ImGui::Text("[2]");
                ImGui::SameLine();
                ImGui::InputDouble("##css_sf[0]", &command->adcs_setparamsccmd.Params.css_sf[2]);
                ImGui::Text("[3]");
                ImGui::SameLine();
                ImGui::InputDouble("##css_sf[0]", &command->adcs_setparamsccmd.Params.css_sf[3]);
                ImGui::SameLine();
                ImGui::Text("[4]");
                ImGui::SameLine();
                ImGui::InputDouble("##css_sf[0]", &command->adcs_setparamsccmd.Params.css_sf[4]);
                ImGui::Text("MMT_A : ");
                ImGui::Text("[0][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[0][0]", &command->adcs_setparamsccmd.Params.mmt_A[0][0]);
                ImGui::SameLine();
                ImGui::Text("[0][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[0][1]", &command->adcs_setparamsccmd.Params.mmt_A[0][1]);
                ImGui::SameLine();
                ImGui::Text("[0][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[0][2]", &command->adcs_setparamsccmd.Params.mmt_A[0][2]);
                ImGui::Text("[1][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[1][0]", &command->adcs_setparamsccmd.Params.mmt_A[1][0]);
                ImGui::SameLine();
                ImGui::Text("[1][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[1][1]", &command->adcs_setparamsccmd.Params.mmt_A[1][1]);
                ImGui::SameLine();
                ImGui::Text("[1][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[1][2]", &command->adcs_setparamsccmd.Params.mmt_A[1][2]);
                ImGui::Text("[2][0]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[2][0]", &command->adcs_setparamsccmd.Params.mmt_A[2][0]);
                ImGui::SameLine();
                ImGui::Text("[2][1]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[2][1]", &command->adcs_setparamsccmd.Params.mmt_A[2][1]);
                ImGui::SameLine();
                ImGui::Text("[2][2]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_A[2][2]", &command->adcs_setparamsccmd.Params.mmt_A[2][2]);
                ImGui::Text("MMT bias : ");
                ImGui::Text("[0]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_bias[0]", &command->adcs_setparamsccmd.Params.mmt_bias[0]);
                ImGui::SameLine();
                ImGui::Text("[1]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_bias[0]", &command->adcs_setparamsccmd.Params.mmt_bias[1]);
                ImGui::SameLine();
                ImGui::Text("[2]");
                ImGui::SameLine();
                ImGui::InputDouble("##mmt_bias[0]", &command->adcs_setparamsccmd.Params.mmt_bias[2]);
                ImGui::Text("Gyro bias A : ");
                ImGui::Text("[0]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_a[0]", &command->adcs_setparamsccmd.Params.gyro_bias_a[0]);
                ImGui::SameLine();
                ImGui::Text("[1]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_a[0]", &command->adcs_setparamsccmd.Params.gyro_bias_a[1]);
                ImGui::SameLine();
                ImGui::Text("[2]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_a[0]", &command->adcs_setparamsccmd.Params.gyro_bias_a[2]);
                ImGui::Text("Gyro bias B : ");
                ImGui::Text("[0]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_b[0]", &command->adcs_setparamsccmd.Params.gyro_bias_b[0]);
                ImGui::SameLine();
                ImGui::Text("[1]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_b[0]", &command->adcs_setparamsccmd.Params.gyro_bias_b[1]);
                ImGui::SameLine();
                ImGui::Text("[2]");
                ImGui::SameLine();
                ImGui::InputDouble("##gyro_bias_b[0]", &command->adcs_setparamsccmd.Params.gyro_bias_b[2]);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_setparamsccmd, sizeof(ADCS_SetParamScCmd_t));
                break;
            }
            case(ADCS_SETPARAM_TLE): {
                ImGui::Text("Epoch Year : ");
                ImGui::SameLine();
                ImGui::InputScalar("##epochyear", ImGuiDataType_U32, &command->adcs_setparamtlecmd.Params.epochYear, NULL, NULL, "%u");
                ImGui::Text("Epoch Day : ");
                ImGui::SameLine();
                ImGui::InputFloat("##epochday", &command->adcs_setparamtlecmd.Params.epochDay);
                ImGui::Text("Bstar : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.Bstar);
                ImGui::Text("Inclination : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.INC);
                ImGui::Text("RAAN : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.RAAN);
                ImGui::Text("Eccentricity : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.ECC);
                ImGui::Text("AOP : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.AOP);
                ImGui::Text("Mean Anomoly : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.MA);
                ImGui::Text("Mean Motion : ");
                ImGui::SameLine();
                ImGui::InputDouble("##bstar", &command->adcs_setparamtlecmd.Params.MM);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_setparamtlecmd, sizeof(ADCS_SetParamTleCmd_t));
                break;
            }
            case(ADCS_SETPARAM_AUX): {
                ImGui::Text("mu_E: ");
                ImGui::SameLine();
                ImGui::InputDouble("##mu_E", &command->adcs_setparamauxcmd.Params.mu_E);
                ImGui::Text("R_E : ");
                ImGui::SameLine();
                ImGui::InputDouble("##R_E", &command->adcs_setparamauxcmd.Params.R_E);
                ImGui::Text("Coef_J2 : ");
                ImGui::SameLine();
                ImGui::InputDouble("##coef_J2", &command->adcs_setparamauxcmd.Params.coef_J2);
                ImGui::Text("w_E : ");
                ImGui::SameLine();
                ImGui::InputDouble("##w_E", &command->adcs_setparamauxcmd.Params.w_E);
                ImGui::Text("wn : ");
                ImGui::SameLine();
                ImGui::InputDouble("##wn", &command->adcs_setparamauxcmd.Params.wn);
                ImGui::Text("tstp : ");
                ImGui::SameLine();
                ImGui::InputDouble("##tstp", &command->adcs_setparamauxcmd.Params.tstp);
                ImGui::Text("quit_TRIAD : ");
                ImGui::SameLine();
                ImGui::InputScalar("##quit_TRIAD", ImGuiDataType_U32, &command->adcs_setparamauxcmd.Params.quit_TRIAD, NULL, NULL, "%u");
                ImGui::Text("num_AD_loop : ");
                ImGui::SameLine();
                ImGui::InputScalar("##num_AD_loop", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.num_AD_loop, NULL, NULL, "%u");
                ImGui::Text("num_AC_loop : ");
                ImGui::SameLine();
                ImGui::InputScalar("##num_AC_loop", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.num_AC_loop, NULL, NULL, "%u");
                ImGui::Text("option_prg_orb_est : ");
                ImGui::SameLine();
                ImGui::InputScalar("##option_prg_orb_est", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.option_prg_orb_est, NULL, NULL, "%u");
                ImGui::Text("option_gyroless : ");
                ImGui::SameLine();
                ImGui::InputScalar("##option_gyroless", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.option_gyroless, NULL, NULL, "%u");
                ImGui::Text("option_ignoreSTT : ");
                ImGui::SameLine();
                ImGui::InputScalar("##option_ignoreSTT", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.option_ignoreSTT, NULL, NULL, "%u");
                ImGui::Text("option_ignoreCSS : ");
                ImGui::SameLine();
                ImGui::InputScalar("##option_ignoreCSS", ImGuiDataType_U8, &command->adcs_setparamauxcmd.Params.option_ignoreCSS, NULL, NULL, "%u");
                ImGui::Text("est_P_atd_STT_initial : ");
                ImGui::SameLine();
                ImGui::InputDouble("##est_P_atd_initial", &command->adcs_setparamauxcmd.Params.est_P_atd_STT_initial);
                ImGui::Text("GS_sband[0] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_sband[0]", &command->adcs_setparamauxcmd.Params.GS_sband[0]);
                ImGui::Text("GS_sband[1] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_sband[1]", &command->adcs_setparamauxcmd.Params.GS_sband[1]);
                ImGui::Text("GS_sband[2] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_sband[2]", &command->adcs_setparamauxcmd.Params.GS_sband[2]);
                ImGui::Text("GS_uhf[0] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_uhf[0]", &command->adcs_setparamauxcmd.Params.GS_uhf[0]);
                ImGui::Text("GS_uhf[1] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_uhf[1]", &command->adcs_setparamauxcmd.Params.GS_uhf[1]);
                ImGui::Text("GS_uhf[2] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##GS_uhf[2]", &command->adcs_setparamauxcmd.Params.GS_uhf[2]);
                ImGui::Text("wrpm_WHLdot_dump : ");
                ImGui::SameLine();
                ImGui::InputDouble("##wrpm_WHLdot_dump", &command->adcs_setparamauxcmd.Params.wrpm_WHLdot_dump);
                ImGui::Text("wrpm_WHL_dump_crit : ");
                ImGui::SameLine();
                ImGui::InputDouble("##wrpm_WHL_dump_crit", &command->adcs_setparamauxcmd.Params.wrpm_WHL_dump_crit);
                ImGui::Text("wrpm_WHL_dump_goal : ");
                ImGui::SameLine();
                ImGui::InputDouble("##wrpm_WHL_dump_goal", &command->adcs_setparamauxcmd.Params.wrpm_WHL_dump_goal);
                ImGui::Text("w_stanby_detumbling_start : ");
                ImGui::SameLine();
                ImGui::InputDouble("##w_stanby_detumbling_start", &command->adcs_setparamauxcmd.Params.w_standby_detumbling_start);
                ImGui::Text("w_stanby_detumbling_goal : ");
                ImGui::SameLine();
                ImGui::InputDouble("##w_stanby_detumblig_goal", &command->adcs_setparamauxcmd.Params.w_standby_detumbling_goal);
                ImGui::Text("w_stanby_forcedSpin_start : ");
                ImGui::SameLine();
                ImGui::InputDouble("##w_stanby_forcedSpin_start", &command->adcs_setparamauxcmd.Params.w_standby_forcedSpin_start);
                ImGui::Text("angle_stanby_forcedSpin_start : ");
                ImGui::SameLine();
                ImGui::InputDouble("##angle_stanby_forcedSpin_start", &command->adcs_setparamauxcmd.Params.angle_standby_forcedSpin_start);
                ImGui::Text("M_stanby_forcedSpin[0] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##M_stanby_forcedSpin[0]", &command->adcs_setparamauxcmd.Params.M_standby_forcedSpin[0]);
                ImGui::Text("M_stanby_forcedSpin[1] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##M_stanby_forcedSpin[1]", &command->adcs_setparamauxcmd.Params.M_standby_forcedSpin[1]);
                ImGui::Text("M_stanby_forcedSpin[2] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##M_stanby_forcedSpin[2]", &command->adcs_setparamauxcmd.Params.M_standby_forcedSpin[2]);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_setparamauxcmd, sizeof(ADCS_SetParamAuxCmd_t));
                break;
            }
            case(ADCS_SETPARAM_TARGET): {
                ImGui::Text("r_Target[0] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##r_Target[0]", &command->adcs_setparamtargetcmd.Params.r_Target[0]);
                ImGui::Text("r_Target[1] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##r_Target[1]", &command->adcs_setparamtargetcmd.Params.r_Target[1]);
                ImGui::Text("r_Target[2] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##r_Target[2]", &command->adcs_setparamtargetcmd.Params.r_Target[2]);
                ImGui::Text("q_Target[0] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##q_Target[0]", &command->adcs_setparamtargetcmd.Params.q_Target[0]);
                ImGui::Text("q_Target[1] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##q_Target[1]", &command->adcs_setparamtargetcmd.Params.q_Target[1]);
                ImGui::Text("q_Target[2] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##q_Target[2]", &command->adcs_setparamtargetcmd.Params.q_Target[2]);
                ImGui::Text("q_Target[3] : ");
                ImGui::SameLine();
                ImGui::InputDouble("##q_Target[3]", &command->adcs_setparamtargetcmd.Params.q_Target[3]);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_setparamtargetcmd, sizeof(ADCS_SetParamTargetCmd_t));
                break;
            }
            case(ADCS_SETPARAM_GAIN): {
                ImGui::Text("WhichGain : ");
                ImGui::SameLine();
                ImGui::InputScalar("##adcs_setparamgain_whichgain", ImGuiDataType_U8, &command->adcs_setparamgaincmd.Payload.WhichGain, NULL, NULL, "%u");
                ImGui::Text("Mode : ");
                ImGui::SameLine();
                ImGui::InputScalar("##adcs_setparamgain_Mode", ImGuiDataType_U8, &command->adcs_setparamgaincmd.Payload.Mode, NULL, NULL, "%u");
                ImGui::Text("Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##adcs_setparamgain_num", ImGuiDataType_U16, &command->adcs_setparamgaincmd.Payload.Num, NULL, NULL, "%u");
                for (int i = 0; i < 31; i++)
                {
                    ImGui::Text("Gain[%d]", i);
                    ImGui::SameLine();
                    snprintf(this->RepeatedLabel, sizeof(this->RepeatedLabel), "##adcs_setparamgain_gain_%d", i);
                    ImGui::InputDouble(this->RepeatedLabel, &command->adcs_setparamgaincmd.Payload.Gain[i]);
                    if (i % 4 != 3)
                        ImGui::SameLine();
                }
                ImGui::Text("");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_setparamgaincmd, sizeof(ADCS_SetParamGainCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->adcs_noargscmd, sizeof(ADCS_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("GPS"))
        {
            ImGui::BeginChild("##cmdtitle_gps", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != GPS_CMD_MID)
            {
                this->gen_msgid = GPS_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, GPSCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(GPSCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(GPSCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(GPS_LOG_ONCE_CC):
            {
                ImGui::Text("Msg ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##logoncemsgid", ImGuiDataType_S16, &command->gps_logoncecmd.MsgId, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->gps_logoncecmd, sizeof(GPS_LogOnceCmd_t));
                break;
            }
            case(GPS_LOG_ONTIME_CC):
            {
                ImGui::Text("Msg ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##gps_logontime_msgid", ImGuiDataType_S16, &command->gps_logontimecmd.MsgId, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->gps_logontimecmd, sizeof(GPS_LogOntimeCmd_t));
                break;
            }
            case(GPS_LOG_ONCHANGE_CC):
            {
                ImGui::Text("Msg ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##gps_logonchange_msgid", ImGuiDataType_S16, &command->gps_logonchangecmd.MsgId, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->gps_logonchangecmd, sizeof(GPS_LogOnChangeCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->gps_noargscmd, sizeof(GPS_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("EPS"))
        {
            ImGui::BeginChild("##cmdtitle_eps", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != EPS_CMD_MID)
            {
                this->gen_msgid = EPS_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, EPSCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(EPSCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(EPSCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(EPS_SET_CONFIG_PARAMETER_CC): {
                ImGui::Text("Parameter ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ParameterID", ImGuiDataType_U16, &command->eps_setconfig.ParameterID, NULL, NULL, "%u");
                ImGui::Text("Parameter Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameter Length", ImGuiDataType_U8, &command->eps_setconfig.ParameterLength, NULL, NULL, "%u");
                ImGui::Text("Parameter[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[0]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[0], NULL, NULL, "%u");
                ImGui::Text("Parameter[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[1]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[1], NULL, NULL, "%u");
                ImGui::Text("Parameter[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[2]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[2], NULL, NULL, "%u");
                ImGui::Text("Parameter[3] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[3]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[3], NULL, NULL, "%u");
                ImGui::Text("Parameter[4] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[4]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[4], NULL, NULL, "%u");
                ImGui::Text("Parameter[5] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[5]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[5], NULL, NULL, "%u");
                ImGui::Text("Parameter[6] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[6]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[6], NULL, NULL, "%u");
                ImGui::Text("Parameter[7] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameters[7]", ImGuiDataType_U8, &command->eps_setconfig.Parameter[7], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_setconfig, sizeof(EPS_SetConfigurationParameterCmd_t));
                break;
            }
            case(EPS_RESET_CONFIG_PARAMETER_CC): {
                ImGui::Text("Parameter Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameter Length", ImGuiDataType_U8, &command->eps_resetconfig.ParameterLength, NULL, NULL, "%u");
                ImGui::Text("Parameter ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Parameter ID", ImGuiDataType_U16, &command->eps_resetconfig.ParameterID, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_setconfig, sizeof(EPS_SetConfigurationParameterCmd_t));
                break;
            }
            case(EPS_CORRECT_TIME_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_correcttime_Arg : ", ImGuiDataType_S32, &command->eps_correcttimecmd.Arg, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_correcttimecmd, sizeof(EPS_CorrectTimeCmd_t));
                break;
            }
            case(EPS_OUTPUT_BUS_GROUP_ON_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_outputbusgroupon_arg", ImGuiDataType_U16, &command->eps_outputbusgrouponcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_outputbusgrouponcmd, sizeof(EPS_OutputBusGroupOnCmd_t));
                break;
            }
            case(EPS_OUTPUT_BUS_GROUP_OFF_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_outputbusgroupoff_arg", ImGuiDataType_U16, &command->eps_outputbusgroupoffcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_outputbusgroupoffcmd, sizeof(EPS_OUTPUT_BUS_GROUP_OFF_CC));
                break;
            }
            case(EPS_OUTPUT_BUS_GROUP_STATE_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_outputbusgroupstate_arg", ImGuiDataType_U16, &command->eps_outputbusgroupstatecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_outputbusgroupstatecmd, sizeof(EPS_OutputBusGroupStateCmd_t));
                break;
            }
            case(EPS_OUTPUT_BUS_CHANNEL_ON_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_outputbuschannelon", ImGuiDataType_U8, &command->eps_outputbuschanneloffcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_outputbuschanneloffcmd, sizeof(EPS_OutputBusChannelOnCmd_t));
                break;
            }
            case(EPS_OUTPUT_BUS_CHANNEL_OFF_CC): {
                ImGui::Text("Args : ");
                ImGui::SameLine();
                ImGui::InputScalar("##eps_outputbuschanneloff_arg", ImGuiDataType_U8, &command->eps_outputbuschanneloffcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_outputbuschanneloffcmd, sizeof(EPS_OutputBusChannelOffCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->eps_noargscmd, sizeof(EPS_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("FM"))
        {
            ImGui::BeginChild("##cmdtitle_fm", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != FM_CMD_MID)
            {
                this->gen_msgid = FM_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, FMCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(FMCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(FMCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode)
            {
            case FM_RESET_APP_CC: {
                ImGui::Text("Application ID             : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Application ID", ImGuiDataType_U32, &command->fm_resetappcmd.AppId, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_resetappcmd, sizeof(FM_ResetAppCmd_t));
                break;
            }
            case FM_MODE_TRANSFER_CC: {
                ImGui::Text("Mode                       : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Mode", ImGuiDataType_U8, &command->fm_modetransfercmd.Mode, NULL, NULL, "%u");
                ImGui::Text("Submode                    : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Submode", ImGuiDataType_U8, &command->fm_modetransfercmd.Submode, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_modetransfercmd, sizeof(FM_ModeTransferCmd_t));
                break;
            }
            case FM_SET_OPERATION_MODE_CC: {
                ImGui::Text("Mode                       : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Mode", ImGuiDataType_U8, &command->fm_setoperationmodecmd.Mode, NULL, NULL, "%u");
                ImGui::Text("Submode                    : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Submode", ImGuiDataType_U8, &command->fm_setoperationmodecmd.Submode, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setoperationmodecmd, sizeof(FM_SetOperationModeCmd_t));
                break;
            }
            case FM_SET_COMMISSIONING_PHASE_CC: {
                ImGui::Text("Commissioning Phase        : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Commissioning Phase", ImGuiDataType_U8, &command->fm_setcommissioningphasecmd.CommissioningPhase, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setcommissioningphasecmd, sizeof(FM_SetCommissioningPhaseCmd_t));
                break;
            }
            case FM_SET_SPACECRAFT_TIME_CC: {
                ImGui::Text("Second                     : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Second", ImGuiDataType_U32, &command->fm_setspacecrafttimecmd.Seconds, NULL, NULL, "%u");
                ImGui::Text("Subsecond                  : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Subsecond", ImGuiDataType_U32, &command->fm_setspacecrafttimecmd.Subseconds, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setspacecrafttimecmd, sizeof(FM_SetSpacecraftTimeCmd_t));
                break;
            }
            case FM_SET_ANTENNA_DEPLOY_FLAG_CC: {
                ImGui::Text("Antenna Deploy Flag        : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Antenna Deploy Flag", ImGuiDataType_U8, &command->fm_setantennadeployflagcmd.AntennaDeployFlag, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setantennadeployflagcmd, sizeof(FM_SetAntennaDeployFlagCmd_t));
                break;
            }
            case FM_SET_DAYLIGHT_DETECTION_FLAG_CC: {
                ImGui::Text("Daylight Detection Flag    : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Daylight Detection Flag", ImGuiDataType_U8, &command->fm_setdaylightdetectionflagcmd.DaylightDetectionFlag, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setdaylightdetectionflagcmd, sizeof(FM_SetDaylightDetectionFlagCmd_t));
                break;
            }
            case FM_SET_COMM_MISSING_FLAG_CC: {
                ImGui::Text("Commissioning Flag         : ");
                ImGui::SameLine();
                ImGui::InputScalar("##Commissioning Flag", ImGuiDataType_U8, &command->fm_setcommmissingflagcmd.CommunicationMissing, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_setcommmissingflagcmd, sizeof(FM_SetCommMissingFlagCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->fm_noargscmd, sizeof(FM_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("MTQ"))
        {
            ImGui::BeginChild("##cmdtitle_mtq", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != MTQ_CMD_MID)
            {
                this->gen_msgid = MTQ_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, MTQCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(MTQCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(MTQCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(MTQ_RESET_COMP): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_resetcompcmd0", ImGuiDataType_U8, &command->mtq_resetcompcmd.Args[0], NULL, NULL, "%u");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_resetcompcmd1", ImGuiDataType_U8, &command->mtq_resetcompcmd.Args[1], NULL, NULL, "%u");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_resetcompcmd2", ImGuiDataType_U8, &command->mtq_resetcompcmd.Args[2], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_resetcompcmd, sizeof(MTQ_ResetCompCmd_t));
                break;
            }
            case(MTQ_ENABLE): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_enablecmd0", ImGuiDataType_U8, &command->mtq_enablecmd.Args[0], NULL, NULL, "%u");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_enablecmd1", ImGuiDataType_U8, &command->mtq_enablecmd.Args[1], NULL, NULL, "%u");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_enablecmd2", ImGuiDataType_U8, &command->mtq_enablecmd.Args[2], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_enablecmd, sizeof(MTQ_EnableCmd_t));
                break;
            }
            case(MTQ_DISABLE): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_diablecmd0", ImGuiDataType_U8, &command->mtq_disablecmd.Args[0], NULL, NULL, "%u");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_disablecmd1", ImGuiDataType_U8, &command->mtq_disablecmd.Args[1], NULL, NULL, "%u");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_disablecmd2", ImGuiDataType_U8, &command->mtq_disablecmd.Args[2], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_disablecmd, sizeof(MTQ_DisableCmd_t));
                break;
            }
            case(MTQ_SET_POLARITY): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setpolarity0", ImGuiDataType_U8, &command->mtq_setpolaritycmd.Args[0], NULL, NULL, "%u");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setpolarity1", ImGuiDataType_U8, &command->mtq_setpolaritycmd.Args[1], NULL, NULL, "%u");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setpolarity2", ImGuiDataType_U8, &command->mtq_setpolaritycmd.Args[2], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_setpolaritycmd, sizeof(MTQ_SetPolarityCmd_t));
                break;
            }
            case(MTQ_SET_DUTY): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setdutycmd0", ImGuiDataType_S8, &command->mtq_setdutycmd.Args[0], NULL, NULL, "%d");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setdutycmd1", ImGuiDataType_S8, &command->mtq_setdutycmd.Args[1], NULL, NULL, "%d");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setdutycmd2", ImGuiDataType_U8, &command->mtq_setdutycmd.Args[2], NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_setdutycmd, sizeof(MTQ_SetDutyCmd_t));
                break;
            }
            case(MTQ_SET_PERIOD): {
                ImGui::Text("Args");
                ImGui::Text("[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setperiodcmd0", ImGuiDataType_U32, &command->mtq_setperiodcmd.Args[0], NULL, NULL, "%u");
                ImGui::Text("[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setperiodcmd1", ImGuiDataType_U32, &command->mtq_setperiodcmd.Args[1], NULL, NULL, "%u");
                ImGui::Text("[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##mtq_setperiodcmd2", ImGuiDataType_U32, &command->mtq_setperiodcmd.Args[2], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_setperiodcmd, sizeof(MTQ_SetPeriodCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->mtq_noargscmd, sizeof(MTQ_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Payload"))
        {
            ImGui::BeginChild("##cmdtitle_pay", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != PAY_CMD_MID)
            {
                this->gen_msgid = PAY_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, PAYCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(PAYCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(PAYCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(PAY_CAM_DOWNLOAD_OLD_IMG_CC): {
                ImGui::Text("Index : ");
                ImGui::SameLine();
                ImGui::InputScalar("##pay_camdownloadoldimg", ImGuiDataType_U16, &command->pay_camdownloadoldimgcmd.Index, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->pay_camdownloadoldimgcmd, sizeof(PAY_CamDownloadOldImgCmd_t));
                break;
            }
            case(PAY_CAM_SEND_NOARG_CC): {
                ImGui::Text("CMD Code : ");
                ImGui::SameLine();
                ImGui::InputScalar("##pay_camsendnoargcmd", ImGuiDataType_U8, &command->pay_camsendnoargcmd.CmdCode, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->pay_camsendnoargcmd, sizeof(PAY_CamDownloadOldImgCmd_t));
                break;
            }
            case(PAY_CAM_SET_EXPOSURE_CC): {
                ImGui::Text("Exposure : ");
                ImGui::SameLine();
                ImGui::InputScalar("##pay_camsetexposure", ImGuiDataType_U32, &command->pay_camsetexposurecmd.Exposure, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->pay_camsetexposurecmd, sizeof(PAY_CamSetExposureCmd_t));
                break;
            }
            case(PAY_CAM_SET_SCPD_CC): {
                ImGui::Text("Index : ");
                ImGui::SameLine();
                ImGui::InputScalar("##pay_camsetscpd", ImGuiDataType_U32, &command->pay_camsetscpdcmd.SCPD, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->pay_camsetscpdcmd, sizeof(PAY_CamSetScpdCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->pay_noargscmd, sizeof(PAY_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("RWA"))
        {
            ImGui::BeginChild("##cmdtitle_rwa", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != RWA_CMD_MID)
            {
                this->gen_msgid = RWA_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, RWACMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(RWACMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(RWACMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(RWA_RESET_WHEEL_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_resetwheel", ImGuiDataType_U8, &command->rwa_resetwheelcmd.WhlNum, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_resetwheelcmd, sizeof(RWA_ResetWheelCmd_t));
                break;
            }
            case(RWA_SET_CLEAR_ERRORS_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_clearerrors", ImGuiDataType_U8, &command->rwa_clearerrorscmd.WhlNum, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_clearerrorscmd, sizeof(RWA_ClearErrorsCmd_t));
                break;
            }
            case(RWA_SET_MOTOR_POWER_STATE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmotorpowerstate_wheelnum", ImGuiDataType_U8, &command->rwa_setmotorpowerstatecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmotorpowerstate_arg", ImGuiDataType_U8, &command->rwa_setmotorpowerstatecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setmotorpowerstatecmd, sizeof(RWA_SetMotorPowerStateCmd_t));
                break;
            }
            case(RWA_SET_ENCODER_POWER_STATE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setencoderpowerstate_wheelnum", ImGuiDataType_U8, &command->rwa_setencoderpowerstatecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setencoderpowerstate_arg", ImGuiDataType_U8, &command->rwa_setencoderpowerstatecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setencoderpowerstatecmd, sizeof(RWA_SetEncoderPowerStateCmd_t));
                break;
            }
            case(RWA_SET_HALL_POWER_STATE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_sethallpowerstate_wheelnum", ImGuiDataType_U8, &command->rwa_sethallpowerstatecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_sethallpowerstate_arg", ImGuiDataType_U8, &command->rwa_sethallpowerstatecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_sethallpowerstatecmd, sizeof(RWA_SetHallPowerStateCmd_t));
                break;
            }
            case(RWA_SET_CONTROL_MODE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setcontrolmode_wheelnum", ImGuiDataType_U8, &command->rwa_setcontrolmodecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setcontrolmode_arg", ImGuiDataType_U8, &command->rwa_setcontrolmodecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setcontrolmodecmd, sizeof(RWA_SetControlModeCmd_t));
                break;
            }
            case(RWA_SET_BACKUP_WHEEL_MODE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupwheelmode_wheelnum", ImGuiDataType_U8, &command->rwa_setbackupwheelmodecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupwheelmode_arg", ImGuiDataType_U8, &command->rwa_setbackupwheelmodecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setbackupwheelmodecmd, sizeof(RWA_SetBackupWheelModeCmd_t));
                break;
            }
            case(RWA_SET_WHEEL_REFERENCE_SPEED_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelreferencespeed_wheelnum", ImGuiDataType_U8, &command->rwa_setwheelreferencespeedcmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelreferencespeed_arg", ImGuiDataType_S16, &command->rwa_setwheelreferencespeedcmd.Arg, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setwheelreferencespeedcmd, sizeof(RWA_SetWheelReferenceSpeedCmd_t));
                break;
            }
            case(RWA_SET_WHEEL_COMMANDED_TORQUE_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelcommandedtorque_wheelnum", ImGuiDataType_U8, &command->rwa_setwheelcommandedtorquecmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelcommandedtorque_arg", ImGuiDataType_S16, &command->rwa_setwheelcommandedtorquecmd.Arg, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setwheelcommandedtorquecmd, sizeof(RWA_SetWheelCommandedTorqueCmd_t));
                break;
            }
            case(RWA_SET_PWM_GAIN_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setpwmgain_wheelnum", ImGuiDataType_U8, &command->rwa_setpwmgaincmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("K : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setpwmgain_k", ImGuiDataType_S16, &command->rwa_setpwmgaincmd.Input.K, NULL, NULL, "%d");
                ImGui::Text("Kmultiplier : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setpwmgain_kmultiplier", ImGuiDataType_U8, &command->rwa_setpwmgaincmd.Input.Kmultiplier, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setpwmgaincmd, sizeof(RWA_SetPwmGainCmd_t));
                break;
            }
            case(RWA_SET_MAIN_GAIN_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmaingain_wheelnum", ImGuiDataType_U8, &command->rwa_setmaingaincmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Ki : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmaingain_ki", ImGuiDataType_U16, &command->rwa_setmaingaincmd.Input.Ki, NULL, NULL, "%u");
                ImGui::Text("Kimultiplier : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmaingain_kimultiplier", ImGuiDataType_U8, &command->rwa_setmaingaincmd.Input.KiMultiplier, NULL, NULL, "%u");
                ImGui::Text("Kd : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmaingain_kd", ImGuiDataType_U16, &command->rwa_setmaingaincmd.Input.Kd, NULL, NULL, "%u");
                ImGui::Text("Kdmultiplier : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setmaingain_kdmultiplier", ImGuiDataType_U8, &command->rwa_setmaingaincmd.Input.KdMultiplier, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setmaingaincmd, sizeof(RWA_SetMainGainCmd_t));
                break;
            }
            case(RWA_SET_BACKUP_GAIN_CC): {
                ImGui::Text("Wheel Num : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupgain_wheelnum", ImGuiDataType_U8, &command->rwa_setbackupgaincmd.WhlNum, NULL, NULL, "%u");
                ImGui::Text("Ki : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupgain_ki", ImGuiDataType_U16, &command->rwa_setbackupgaincmd.Input.Ki, NULL, NULL, "%u");
                ImGui::Text("Kimultiplier : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupgain_kimultiplier", ImGuiDataType_U8, &command->rwa_setbackupgaincmd.Input.KiMultiplier, NULL, NULL, "%u");
                ImGui::Text("Kd : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupgain_kd", ImGuiDataType_U16, &command->rwa_setbackupgaincmd.Input.Kd, NULL, NULL, "%u");
                ImGui::Text("Kdmultiplier : ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setbackupgain_kdmultiplier", ImGuiDataType_U8, &command->rwa_setbackupgaincmd.Input.KdMultiplier, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setbackupgaincmd, sizeof(RWA_SetBackupGainCmd_t));
                break;
            }
            case(RWA_SET_WHEEL_REFERENCE_SPEED_ALL_AXIS_CC): {
                ImGui::Text("Wheel Ref Speed[0]: ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelreferencespeedallaxis[0]", ImGuiDataType_S16, &command->rwa_setwheelreferencespeedallaxiscmd.WheelRefSpeed[0], NULL, NULL, "%d");
                ImGui::Text("Wheel Ref Speed[1]: ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelreferencespeedallaxis[1]", ImGuiDataType_S16, &command->rwa_setwheelreferencespeedallaxiscmd.WheelRefSpeed[1], NULL, NULL, "%d");
                ImGui::Text("Wheel Ref Speed[2]: ");
                ImGui::SameLine();
                ImGui::InputScalar("##rwa_setwheelreferencespeedallaxis[2]", ImGuiDataType_S16, &command->rwa_setwheelreferencespeedallaxiscmd.WheelRefSpeed[2], NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_setwheelreferencespeedallaxiscmd, sizeof(RWA_SetWheelReferenceSpeedAllAxisCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->rwa_noargscmd, sizeof(RWA_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndTabItem();
            ImGui::EndChild();
        }
        if (ImGui::BeginTabItem("Sensor"))
        {
            ImGui::BeginChild("##cmdtitle_snsr", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != SNSR_CMD_MID)
            {
                this->gen_msgid = SNSR_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, SNSRCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(SNSRCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(SNSRCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(SNSR_STT_BOOT_CC): {
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttboot_arg", ImGuiDataType_U8, &command->snsr_stt_bootcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_stt_bootcmd, sizeof(SNSR_STT_BootCmd_t));
                break;
            }
            case(SNSR_STT_PING_CC): {
                ImGui::Text("Arg : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttping_arg", ImGuiDataType_U32, &command->snsr_stt_pingcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_stt_pingcmd, sizeof(SNSR_STT_PingCmd_t));
                break;
            }
            case(SNSR_STT_UNLOCK_CC): {
                ImGui::Text("Image : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttunlock_image", ImGuiDataType_U8, &command->snsr_stt_unlockcmd.Image, NULL, NULL, "%u");
                ImGui::Text("Code : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttunlock_code", ImGuiDataType_U32, &command->snsr_stt_unlockcmd.Code, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_stt_unlockcmd, sizeof(SNSR_STT_UnlockCmd_t));
                break;
            }
            case(SNSR_STT_SET_PARAM_CC): {
                ImGui::Text("Param ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttsetparam_paramid", ImGuiDataType_U8, &command->snsr_stt_setparamcmd.ParamId, NULL, NULL, "%u");
                ImGui::Text("Param Size : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttsetparam_paramsize", ImGuiDataType_U16, &command->snsr_stt_setparamcmd.ParamSize, NULL, NULL, "%u");
                ImGui::Text("Param : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttsetparam_param", ImGuiDataType_U8, &command->snsr_stt_setparamcmd.Param[0], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_stt_setparamcmd, sizeof(SNSR_STT_SetParamCmd_t));
                break;
            }
            case(SNSR_STT_SEND_RS485_CC): {
                ImGui::Text("Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttsendrs485_length", ImGuiDataType_U16, &command->snsr_stt_sendrs485cmd.Length, NULL, NULL, "%u");
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_sttsendrs485_length", ImGuiDataType_U8, &command->snsr_stt_sendrs485cmd.Data[0], NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_stt_sendrs485cmd, sizeof(SNSR_STT_SendRS485Cmd_t));
                break;
            }
            case(SNSR_MMT_SET_INTERNAL_CONTROL0_CC): {
                ImGui::Text("TM_M : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol0cmd_ tm_m", &command->snsr_mmt_setinternalcontrol0cmd.TM_M);
                ImGui::Text("TM_T : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol0cmd_ tm_t", &command->snsr_mmt_setinternalcontrol0cmd.TM_T);
                ImGui::Text("Start_MDT : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol0cmd_ start_mdt", &command->snsr_mmt_setinternalcontrol0cmd.Start_MDT);
                ImGui::Text("Set : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol0cmd_ set", &command->snsr_mmt_setinternalcontrol0cmd.Set);
                ImGui::Text("Reset : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol0cmd_ reset", &command->snsr_mmt_setinternalcontrol0cmd.Reset);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_mmt_setinternalcontrol0cmd, sizeof(SNSR_MMT_SetInternalControl0Cmd_t));
                break;
            }
            case(SNSR_MMT_SET_INTERNAL_CONTROL2_CC): {
                ImGui::Text("CM_Freq : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_mmt_setinternalcontrol2_cm_freq", ImGuiDataType_U8, &command->snsr_mmt_setinternalcontrol2cmd.CM_Freq, NULL, NULL, "%u");
                ImGui::Text("INT_MDT_EN : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol2_intmdten", &command->snsr_mmt_setinternalcontrol2cmd.INT_MDT_EN);
                ImGui::Text("INT_Meas_Done_EN : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_mmt_setinternalcontrol2_int_meas_done_en", &command->snsr_mmt_setinternalcontrol2cmd.INT_Meas_Done_EN);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_mmt_setinternalcontrol2cmd, sizeof(SNSR_MMT_SetInternalControl2Cmd_t));
                break;
            }
            case(SNSR_MMT_WRITE_TO_REGISTER_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_mmt_write_to_register_data", ImGuiDataType_U8, &command->snsr_mmt_writetoregistercmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_mmt_writetoregistercmd, sizeof(SNSR_MMT_WriteToRegisterCmd_t));
                break;
            }
            case(SNSR_IMU_SET_GYRO_OFFSET_CC): {
                ImGui::Text("Offset[0] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setgyrooffsetcmd_offset0", ImGuiDataType_S16, &command->snsr_imu_setgyrooffsetcmd.Offset[0], NULL, NULL, "%d");
                ImGui::Text("Offset[1] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setgyrooffsetcmd_offset1", ImGuiDataType_S16, &command->snsr_imu_setgyrooffsetcmd.Offset[1], NULL, NULL, "%d");
                ImGui::Text("Offset[2] : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setgyrooffsetcmd_offset2", ImGuiDataType_S16, &command->snsr_imu_setgyrooffsetcmd.Offset[2], NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setgyrooffsetcmd, sizeof(SNSR_IMU_SetGyroOffsetCmd_t));
                break;
            }
            case(SNSR_IMU_SET_CONFIGURATION_CC): {
                ImGui::Text("FifoMode : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setconfiguration_fifomode", ImGuiDataType_U8, &command->snsr_imu_setconfigurationcmd.FifoMode, NULL, NULL, "%u");
                ImGui::Text("ExtSyncSet : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setconfiguration_extsyncset", ImGuiDataType_U8, &command->snsr_imu_setconfigurationcmd.ExtSyncSet, NULL, NULL, "%u");
                ImGui::Text("Config DLPF : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setconfiguration_configdlpf", ImGuiDataType_U8, &command->snsr_imu_setconfigurationcmd.ConfigDLPF, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setconfigurationcmd, sizeof(SNSR_IMU_SetConfigurationCmd_t));
                break;
            }
            case(SNSR_IMU_SET_GYRO_CONFIGURATION_CC): {
                ImGui::Text("Accel Full Scale : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setgyroconfiguration_accelfullscale", ImGuiDataType_U8, &command->snsr_imu_setgyroconfigurationcmd.AccelFullScale, NULL, NULL, "%u");
                ImGui::Text("Filter Choice : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setgyroconfiguration_filterchoice", ImGuiDataType_U8, &command->snsr_imu_setgyroconfigurationcmd.FilterChoice, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setgyroconfigurationcmd, sizeof(SNSR_IMU_SetGyroConfigurationCmd_t));
                break;
            }
            case(SNSR_IMU_SET_ACCEL_CONFIGURATION_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setaccelconfiguration_args", ImGuiDataType_U8, &command->snsr_imu_setaccelconfigurationcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setaccelconfigurationcmd, sizeof(SNSR_IMU_SetAccelConfigurationCmd_t));
                break;
            }
            case(SNSR_IMU_SET_ACCEL_CONFIGURATION2_CC): {
                ImGui::Text("Fifo Size : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setaccelconfiguration2_fifosize", ImGuiDataType_U8, &command->snsr_imu_setaccelconfiguration2cmd.FifoSize, NULL, NULL, "%u");
                ImGui::Text("DEC2_CFG : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setaccelconfiguration2_dec2cfg", ImGuiDataType_U8, &command->snsr_imu_setaccelconfiguration2cmd.DEC2_CFG, NULL, NULL, "%u");
                ImGui::Text("AccelFilter Choice : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_imu_setaccelconfiguration2_accelfilterchoice", &command->snsr_imu_setaccelconfiguration2cmd.AccelFilterChoice);
                ImGui::Text("A_DLPF_CFG : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_setaccelconfiguration2_adlpfcfg", ImGuiDataType_U8, &command->snsr_imu_setaccelconfiguration2cmd.A_DLPF_CFG, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setaccelconfiguration2cmd, sizeof(SNSR_IMU_SetAccelConfiguration2Cmd_t));
                break;
            }
            case(SNSR_IMU_SET_POWER_MANAGEMENT_CC): {
                ImGui::Text("Device Reset : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_imu_setpowermanagement_devicereset", &command->snsr_imu_setpowermanagement1cmd.DEVICE_RESET);
                ImGui::Text("Sleep : ");
                ImGui::SameLine();
                ImGui::Checkbox("##snsr_setpowermanagement_sleep", &command->snsr_imu_setpowermanagement1cmd.SLEEP);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_setpowermanagement1cmd, sizeof(SNSR_IMU_SetPowerManagement1Cmd_t));
                break;
            }
            case(SNSR_IMU_WRITE_TO_REGISTER_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_imu_writetoregister_data", ImGuiDataType_U8, &command->snsr_imu_writetoregistercmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_imu_writetoregistercmd, sizeof(SNSR_IMU_WriteToRegisterCmd_t));
                break;
            }
            case(SNSR_FSS_ISOLATE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_fss_isolate", ImGuiDataType_U8, &command->snsr_fss_isolatedcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_fss_isolatedcmd, sizeof(SNSR_FSS_IsolateCmd_t));
                break;
            }
            case(SNSR_FSS_RESTORE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_fss_restore", ImGuiDataType_U8, &command->snsr_fss_restorecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_fss_restorecmd, sizeof(SNSR_FSS_RestoreCmd_t));
                break;
            }
            case(SNSR_CSS_ISOLATE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_css_isolate", ImGuiDataType_U8, &command->snsr_css_isolatedcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_css_isolatedcmd, sizeof(SNSR_CSS_IsolateCmd_t));
                break;
            }
            case(SNSR_CSS_RESTORE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##snsr_css_restore", ImGuiDataType_U8, &command->snsr_css_restorecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_css_restorecmd, sizeof(SNSR_CSS_RestoreCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->snsr_noargscmd, sizeof(SNSR_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("STX"))
        {
            ImGui::BeginChild("##cmdtitle_stx", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != STX_CMD_MID)
            {
                this->gen_msgid = STX_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, STXCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(STXCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(STXCMDLabels[i], SelectedGen))
                            this->gen_fnccode = i;
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(STX_TRANSMIT_DATA_CC): {
                ImGui::Text("Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitdata_length", ImGuiDataType_S16, &command->stx_transmitdatacmd.Length, NULL, NULL, "%d");
                ImGui::Text("Buf Push Delay : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitdata_bufpushdelay", ImGuiDataType_U16, &command->stx_transmitdatacmd.BufPushDelay, NULL, NULL, "%u");
                ImGui::Text("Data(ASCII, 16byte) : ");
                ImGui::SameLine();
                ImGui::InputText("##stx_transmitdata_data", (char*)command->stx_transmitdatacmd.Data, sizeof(command->stx_transmitdatacmd.Data));
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_transmitdatacmd, sizeof(STX_TransmitDataCmd_t));
                break;
            }
            case(STX_SET_CONTROL_MODE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_setcontrolmodecmd", ImGuiDataType_U8, &command->stx_setcontrolmodecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_setcontrolmodecmd, sizeof(STX_SetControlModeCmd_t));
                break;
            }
            case(STX_SET_ENCODER_RATE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_setencoderrate", ImGuiDataType_U8, &command->stx_setencoderratecmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_setencoderratecmd, sizeof(STX_SetEncoderRateCmd_t));
                break;
            }
            case(STX_SET_PA_POWER_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_setpapower", ImGuiDataType_U8, &command->stx_setpapowercmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_setpapowercmd, sizeof(STX_SetPaPowerCmd_t));
                break;
            }
            case(STX_SET_SYNTH_OFFSET_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_setsynthoffset", ImGuiDataType_U8, &command->stx_setsynthoffsetcmd.Arg, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_setsynthoffsetcmd, sizeof(STX_SetSynthOffsetCmd_t));
                break;
            }
            case(STX_TRANSMIT_FILE_CC): {
                ImGui::Text("Offset : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfile_offset", ImGuiDataType_U32, &command->stx_transmitfilecmd.Offset, NULL, NULL, "%u");
                ImGui::Text("Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfile_length", ImGuiDataType_U32, &command->stx_transmitfilecmd.Length, NULL, NULL, "%u");
                ImGui::Text("BufPushDelay : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfile_BufPushDelay", ImGuiDataType_U16, &command->stx_transmitfilecmd.BufPushDelay, NULL, NULL, "%u");
                ImGui::Text("Path : ");
                ImGui::SameLine();
                ImGui::InputText("##stx_transmitfile_path", command->stx_transmitfilecmd.Path, sizeof(command->stx_transmitfilecmd.Path));
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_transmitfilecmd, sizeof(STX_TransmitFileCmd_t));
                break;
            }
            case(STX_TRANSMIT_FILE_LONG_PATH_CC): {
                ImGui::Text("Offset : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfilelongpath_offset", ImGuiDataType_U32, &command->stx_transmitfilelongpathcmd.Offset, NULL, NULL, "%u");
                ImGui::Text("Length : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfilelongpath_length", ImGuiDataType_U32, &command->stx_transmitfilelongpathcmd.Length, NULL, NULL, "%u");
                ImGui::Text("BufPushDelay : ");
                ImGui::SameLine();
                ImGui::InputScalar("##stx_transmitfilelongpath_BufPushDelay", ImGuiDataType_U16, &command->stx_transmitfilelongpathcmd.BufPushDelay, NULL, NULL, "%u");
                ImGui::Text("Path : ");
                ImGui::SameLine();
                ImGui::InputText("##stx_transmitfile_path", command->stx_transmitfilelongpathcmd.Path, sizeof(command->stx_transmitfilelongpathcmd.Path));
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_transmitfilelongpathcmd, sizeof(STX_TransmitFileLongPathCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->stx_noargscmd, sizeof(STX_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("TO"))
        {
            ImGui::BeginChild("##cmdtitle_to", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != TO_CMD_MID)
            {
                this->gen_msgid = TO_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, TOCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(TOCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(TOCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(TO_DOWNLINK_QUERY_REPLY_CC): {
                ImGui::Text("Target : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_downlinkqueryreply_target", ImGuiDataType_U16, &command->to_downlinkqueryreplycmd.Payload.Target, NULL, NULL, "%u");
                ImGui::Text("FileStatus : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_downlinkqueryreply_filestatus", ImGuiDataType_U16, &command->to_downlinkqueryreplycmd.Payload.FileStatus, NULL, NULL, "%u");
                ImGui::Text("NumFiles : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_downlinkqueryreply_numfiles", ImGuiDataType_U32, &command->to_downlinkqueryreplycmd.Payload.NumFiles, NULL, NULL, "%u");
                ImGui::Text("Offset : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_downlinkqueryreply_offset", ImGuiDataType_U32, &command->to_downlinkqueryreplycmd.Payload.Offset, NULL, NULL, "%u");
                ImGui::Text("Frequency : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_downlinkqueryreply_frequency", ImGuiDataType_U32, &command->to_downlinkqueryreplycmd.Payload.Frequency, NULL, NULL, "%u");
                ImGui::Text("Conn(void pointer) : ");
                ImGui::SameLine();
                ImGui::Text("Contact To Manager.");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->to_downlinkqueryreplycmd, sizeof(command->to_downlinkqueryreplycmd));
                break;
            }
            case(TO_DISABLE_BEACON_CC): {
                ImGui::Text("Timeoutsmin : ");
                ImGui::SameLine();
                ImGui::InputScalar("##to_disablereply_target", ImGuiDataType_S32, &command->to_disablebeaconcmd.timeoutsmin, NULL, NULL, "%d");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->to_disablebeaconcmd, sizeof(TO_DisableBeaconCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->to_noargscmd, sizeof(TO_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("UTRX"))
        {
            ImGui::BeginChild("##cmdtitle_utrx", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != UTRX_CMD_MID)
            {
                this->gen_msgid = UTRX_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, UTRXCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(UTRXCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(UTRXCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(UTRX_SET_TX_FREQ_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_settxfreq", ImGuiDataType_U32, &command->utrx_settxfreqcmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_settxfreqcmd, sizeof(UTRX_SetTxFreqCmd_t));
                break;
            }
            case(UTRX_SET_TX_BAUD_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_settxbaud", ImGuiDataType_U32, &command->utrx_settxbaudcmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_settxbaudcmd, sizeof(UTRX_SetTxBaudCmd_t));
                break;
            }
            case(UTRX_SET_TX_MODIND_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputFloat("##utrx_settxmodind", &command->utrx_settxmodindex.Arg);

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_settxmodindex, sizeof(UTRX_SetTxModIndexCmd_t));
                break;
            }
            case(UTRX_SET_TX_MODE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_settxmode", ImGuiDataType_U8, &command->utrx_settxmodecmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_settxmodecmd, sizeof(UTRX_SetTxModeCmd_t));
                break;
            }
            case(UTRX_SET_RX_FREQ_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_setrxfreq", ImGuiDataType_U32, &command->utrx_setrxfreqcmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_setrxfreqcmd, sizeof(UTRX_SetRxFreqCmd_t));
                break;
            }
            case(UTRX_SET_RX_BAUD_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_setrxbaud", ImGuiDataType_U32, &command->utrx_setrxbaudcmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_setrxbaudcmd, sizeof(UTRX_SetRxBaudCmd_t));
                break;
            }
            case(UTRX_SET_RX_MODIND_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputFloat("##utrx_setrxmodindex", &command->utrx_setrxmodindexcmd.Arg);

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_setrxmodindexcmd, sizeof(UTRX_SetRxModIndexCmd_t));
                break;
            }
            case(UTRX_SET_RX_MODE_CC): {
                ImGui::Text("Data : ");
                ImGui::SameLine();
                ImGui::InputScalar("##utrx_setrxmode", ImGuiDataType_U8, &command->utrx_setrxmodecmd.Arg, NULL, NULL, "%u");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_setrxmodecmd, sizeof(UTRX_SetRxModeCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->utrx_noargscmd, sizeof(UTRX_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("TS"))
        {
            ImGui::BeginChild("##cmdtitle_ts", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != TS_CMD_MID)
            {
                this->gen_msgid = TS_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, TSCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(TSCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(TSCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(TS_INSERT_SCHEDULE_ENTRY_CC): {
                ImGui::Text("Execution Time : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_insertscheduleentry_executiontime", ImGuiDataType_U32, &command->ts_insertscheduleentrycmd.ExecutionTime, NULL, NULL, "%u");
                ImGui::Text("Execution Window : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_insertscheduleentry_executionwindow", ImGuiDataType_U32, &command->ts_insertscheduleentrycmd.ExecutionWindow, NULL, NULL, "%u");
                ImGui::Text("Entry ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_insertscheduleentry_entryid", ImGuiDataType_U16, &command->ts_insertscheduleentrycmd.EntryId, NULL, NULL, "%u");
                ImGui::Text("Entry Group : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_insertscheduleentry_entrygroup", ImGuiDataType_U16, &command->ts_insertscheduleentrycmd.EntryGroup, NULL, NULL, "%u");
                ImGui::Text("Execution Msg : ");
                ImGui::SameLine();
                ImGui::Text("Contact to Manager.");

                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ts_insertscheduleentrycmd, sizeof(TS_InsertScheduleEntryCmd_t));
                break;
            }
            case(TS_CLEAR_SCHEDULE_ENTRY_CC): {
                ImGui::Text("Entry ID : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_clearscheduleentry_entryid", ImGuiDataType_U16, &command->ts_clearscheduleentrycmd.EntryId, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ts_clearscheduleentrycmd, sizeof(TS_ClearScheduleEntryCmd_t));
                break;
            }
            case(TS_CLEAR_SCHEDULE_GROUP_CC): {
                ImGui::Text("Entry Group : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ts_clearscheduleentry_entrygroup", ImGuiDataType_U16, &command->ts_clearschedulegroupcmd.EntryGroup, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ts_clearschedulegroupcmd, sizeof(TS_ClearScheduleGroupCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ts_noargscmd, sizeof(TS_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ECM"))
        {
            ImGui::BeginChild("##cmdtitle_ecm", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != ECM_CMD_MID)
            {
                this->gen_msgid = ECM_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, ECMCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(ECMCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(ECMCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }

                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case (ECM_READ_CC): {
                ImGui::Text("TXlen : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ecm_read_txlen", ImGuiDataType_U8, &command->ecm_readcmd.txlen, NULL, NULL, "%u");
                ImGui::Text("RXlen : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ecm_read_rxlen", ImGuiDataType_U8, &command->ecm_readcmd.rxlen, NULL, NULL, "%u");
                ImGui::Text("CC    : ");
                ImGui::SameLine();
                ImGui::InputScalar("##ecm_read_cc", ImGuiDataType_U8, &command->ecm_readcmd.cc, NULL, NULL, "%u");
                ImGui::Text("data(ASCII): ");
                ImGui::SameLine();
                ImGui::InputText("##ecm_read_data", (char*)&command->ecm_readcmd.data, sizeof(command->ecm_readcmd.data));
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ecm_readcmd, sizeof(ECM_Read_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->ecm_noargscmd, sizeof(ECM_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ES"))
        {
            ImGui::BeginChild("##cmdtitle_es", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != ES_CMD_MID)
            {
                this->gen_msgid = ES_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, ESCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(ESCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(ESCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }

                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case (CFE_ES_RESTART_CC): {
                ImGui::Text("RestartType : ");
                ImGui::SameLine();
                ImGui::InputScalar("##es_restart_restarttype", ImGuiDataType_U16, &command->es_restartcmd.Payload.RestartType, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->es_restartcmd, sizeof(CFE_ES_RestartCmd_t));
                break;
            }
            case (CFE_ES_RESTART_APP_CC): {
                ImGui::Text("Appname : ");
                ImGui::SameLine();
                ImGui::InputText("##es_restartapp", command->es_restartappcmd.Payload.Application, CFE_MISSION_MAX_API_LEN);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->es_restartappcmd, sizeof(CFE_ES_AppNameCmd_t));
                break;
            }
            case (CFE_ES_STOP_APP_CC): {
                ImGui::Text("Application : ");
                ImGui::SameLine();
                ImGui::InputText("##es_stopapp", command->es_stopappcmd.Payload.Application, 19);
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->es_stopappcmd, sizeof(CFE_ES_StopAppCmd_t));
                break;
            }
            case (CFE_ES_START_APP_CC): {
                ImGui::Text("Application : ");
                ImGui::SameLine();
                ImGui::InputText("##es_startapp_application", command->es_startappcmd.Payload.Application, CFE_MISSION_MAX_API_LEN);
                ImGui::Text("AppEntryPoint : ");
                ImGui::SameLine();
                ImGui::InputText("##es_startapp_appentrypoint", command->es_startappcmd.Payload.AppEntryPoint, CFE_MISSION_MAX_API_LEN);
                ImGui::Text("AppFileName : ");
                ImGui::SameLine();
                ImGui::InputText("##es_startapp_appfilename", command->es_startappcmd.Payload.AppFileName, CFE_MISSION_MAX_PATH_LEN);
                ImGui::Text("StackSize : ");
                ImGui::SameLine();
                ImGui::InputScalar("##es_startapp_stacksize", ImGuiDataType_U32, &command->es_startappcmd.Payload.StackSize, NULL, NULL, "%u");
                ImGui::Text("ExceptionAction : ");
                ImGui::SameLine();
                ImGui::InputScalar("##es_startapp_exceptionaction", ImGuiDataType_U8, &command->es_startappcmd.Payload.ExceptionAction, NULL, NULL, "%u");
                ImGui::Text("Priority : ");
                ImGui::SameLine();
                ImGui::InputScalar("##es_startapp_priority", ImGuiDataType_U16, &command->es_startappcmd.Payload.Priority, NULL, NULL, "%u");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->es_startappcmd, sizeof(CFE_ES_StartAppCmd_t));
                break;
            }
            default: {
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->es_noargscmd, sizeof(CFE_ES_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("SCH"))
        {
            ImGui::BeginChild("##cmdtitle_sch", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * this->CMDField), true, this->WindowFlag);
            if (this->gen_msgid != SCH_CMD_MID)
            {
                this->gen_msgid = SCH_CMD_MID;
                this->gen_fnccode = 0;
            }

            snprintf(ComboboxLabel, sizeof(this->ComboboxLabel), "##ComboCMDGEN%d", this->gen_msgid);
            if (ImGui::BeginCombo(ComboboxLabel, SCHCMDLabels[this->gen_fnccode]))
            {
                for (int i = 0; i < 128; i++)
                {
                    if (strlen(SCHCMDLabels[i]) == 0)
                        continue;
                    else
                    {
                        bool SelectedGen = (this->gen_fnccode == i);
                        if (ImGui::Selectable(SCHCMDLabels[i], SelectedGen))
                        {
                            if (this->gen_fnccode != i)
                            {
                                this->gen_fnccode = i;
                            }
                        }
                        if (SelectedGen)
                            ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            switch (this->gen_fnccode) {
            case(SCH_ENABLE_CC):
            {
                ImGui::Text("SlotNumber : ");
                ImGui::SameLine();
                ImGui::InputScalar("##sch_en_slotnumber", ImGuiDataType_U16, &command->sch_enablecmd.SlotNumber, NULL, NULL, "%d");
                ImGui::Text("Entry : ");
                ImGui::SameLine();
                ImGui::InputScalar("##sch_en_entrynumber", ImGuiDataType_U16, &command->sch_enablecmd.EntryNumber, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->sch_enablecmd, sizeof(SCH_EntryCmd_t));
                break;
            }
            case(SCH_DISABLE_CC):
            {
                ImGui::Text("SlotNumber : ");
                ImGui::SameLine();
                ImGui::InputScalar("##sch_dis_slotnumber", ImGuiDataType_U16, &command->sch_disablecmd.SlotNumber, NULL, NULL, "%d");
                ImGui::Text("Entry : ");
                ImGui::SameLine();
                ImGui::InputScalar("##sch_dis_entrynumber", ImGuiDataType_U16, &command->sch_disablecmd.EntryNumber, NULL, NULL, "%d");
                this->CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->sch_disablecmd, sizeof(SCH_EntryCmd_t));
                break;
            }
            default: {
                CMDDataGenerator(this->gen_msgid, this->gen_fnccode, &command->sch_noargscmd, sizeof(SCH_NoArgsCmd_t));
                break;
            }
            }
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();

    ImGui::BeginChild("PingRegion", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.3), true, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale * 2.5);
    if (ImGui::Button("Ping(AX100)", ImVec2(ImGui::GetContentRegionAvail().x * 0.333, ImGui::GetContentRegionAvail().y)))
    {
        //pthread_join(p_thread[4], NULL);
        //pthread_create(&p_thread[4], NULL, csp_ping_console, NULL);
        //csp_ping_custom(NULL);
    }
    ImGui::SameLine();
    if (ImGui::Button("Ping(OBC)", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        //pthread_join(p_thread[4], NULL);
        //FSWTle* MIM_TLEinfo = (FSWTle*)malloc(sizeof(FSWTle) + MIM_HAND_DATAFDSTART);
        //if (State.TLEnow < 0)
        //{
        //    console.AddLog("[ERROR]##You Tried Ping without Tracking. Please Check Again.");
        //}
        //else
        //{
        //    FSWTleConverter(State.Satellites[State.TLEref][State.TLEnow]->tle, MIM_TLEinfo);
        //    packetsign* TestPacket;
        //    TestPacket = PingInit(MIM_TLEinfo);
        //    pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void*)TestPacket);
        //}
    }
    ImGui::SameLine();
    if (ImGui::Button("UL Sig", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        //pthread_join(p_thread[4], NULL);
        //packetsign* TestPacket = (packetsign*)malloc(9);
        //TestPacket->Identifier = MIM_ID;
        //TestPacket->PacketType = MIM_PT_SIGNAL;
        //TestPacket->Length = 1;
        //memset(TestPacket->Data, 0, TestPacket->Length);
        //pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void*)TestPacket);
    }
    ImGui::SetWindowFontScale(this->FontScale);
    ImGui::EndChild();
    //if (ImGui::Button("CMD Manager", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    //{
    //    State.Display_CMD = true;
    //}
    //if (State.Display_CMD)
    //    State.Display_CMD = popup_cmd();
    //ImGui::SameLine();
    //if (ImGui::Button("Clear", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    //{
    //    NowCMD = 0;
    //    for (int i = 0; i < 256; i++)
    //    {
    //        if (SatCMD[i] != NULL)
    //        {
    //            delete SatCMD[i];
    //            SatCMD[i] = NULL;
    //        }
    //    }
    //}
    if (ImGui::BeginTable("##CMDTables", 5, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("FuncCode", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Scheduled", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("DataLength", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Order", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        for (int i = 0; i < 256; i++)
        {
            if (SatCMD[i] == NULL)
                continue;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            this->_msgidbuf[1] = ((uint8_t*)SatCMD[i]->CmdHeader)[0];
            this->_msgidbuf[0] = ((uint8_t*)SatCMD[i]->CmdHeader)[1];
            this->_msgidinfo = *((uint16_t*)this->_msgidbuf);
            ImGui::Text("%x", this->_msgidinfo);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%" PRIu16, SatCMD[i]->GetFncCode());
            ImGui::TableSetColumnIndex(2);
            if (SatCMD[i]->Scheduled)
                ImGui::Text("Yes");
            else
                ImGui::Text("No");
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%" PRIu16, SatCMD[i]->GetSize());
            ImGui::TableSetColumnIndex(4);
            sprintf_s(this->SendingLabelBuf, "Send##%d", i);
            if (ImGui::Button(this->SendingLabelBuf, ImVec2(ImGui::GetContentRegionAvail().x * 0.7, ImGui::GetFontSize() * 1.5)))
            {
                //pthread_join(p_thread[4], NULL);
                //if (SatCMD[i]->Checksum)
                //    SatCMD[i]->GenerateChecksum();
                //packetsign* SendingCMD = SatCMD[i]->GenerateCMDPacket();
                //pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void*)SendingCMD);
            }
            ImGui::SameLine();
            sprintf_s(this->DeleteLabelBuf, "X##%d", i);
            if (ImGui::Button(this->DeleteLabelBuf, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
            {
                delete SatCMD[i];
                SatCMD[i] = NULL;
            }
        }
    }
    ImGui::EndTable();
    ImGui::End();
}