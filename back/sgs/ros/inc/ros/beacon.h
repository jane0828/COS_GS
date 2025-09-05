#pragma once
#ifndef _SGS_BEACON_H_
#define _SGS_BEACON_H_

#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <mysql/mysql.h>
#endif
#include <string>

#ifndef BEACON_NET_ENDIAN
#define BEACON_NET_ENDIAN 1
#endif

#ifndef BEACON_CC_ID
#define BEACON_CC_ID 0x081A
#endif

typedef struct {
    // ===== Header (12 B) =====
    uint16_t CCMessage_ID;        // 2
    uint16_t CCSequence;          // 2
    uint16_t CCLength;            // 2
    uint8_t  CCTime_code[6];      // 6

    // ===== FSW.SRL (17 B) =====
    uint8_t  SRL_CommandCounter;      // 1
    uint8_t  SRL_CommandErrorCounter; // 1
    uint8_t  SRL_I2C0_Status;         // 1
    uint8_t  SRL_I2C1_Status;         // 1
    uint8_t  SRL_I2C2_Status;         // 1
    uint8_t  SRL_UART_Status;         // 1
    uint8_t  SRL_RS422_Status;        // 1
    uint16_t SRL_I2C0_TxCount;        // 2
    uint16_t SRL_I2C1_TxCount;        // 2
    uint16_t SRL_I2C2_TxCount;        // 2
    uint16_t SRL_UART_TxCount;        // 2
    uint16_t SRL_RS422_TxCount;       // 2

    // ===== FSW.RPT (19 B) =====
    uint8_t  RPT_CmdCounter;          // 1
    uint8_t  RPT_ErrCounter;          // 1
    uint8_t  RPT_ReportQueueCount;    // 1
    uint8_t  RPT_CriticalQueueCount;  // 1
    uint16_t RPT_BootCount;           // 2
    uint32_t RPT_SpacecraftTimeSec;   // 4
    uint32_t RPT_SpacecraftTimeSubsec;// 4
    uint32_t RPT_Sequence;            // 4
    uint8_t  RPT_ResetCause;          // 1
    // FSW subtotal = 17 + 19 = 36

    // ===== COMS (25 B) =====
    // SANT
    uint8_t  SANT_status;             // 1
    // STRX (AX2150)
    int16_t  STRX_last_rssi;          // 2
    uint16_t STRX_bootcount;          // 2
    uint32_t STRX_bootcause;          // 4
    uint8_t  STRX_rx_mode;            // 1
    uint16_t STRX_gnd_wdt_cnt;        // 2
    uint32_t STRX_gnd_wdt_left;       // 4
    // UANT (ISIS)
    uint16_t UANT_report_deploy;      // 2
    // UTX (AX100)
    uint8_t  UTX_active_conf;         // 1
    uint16_t UTX_boot_count;          // 2
    uint32_t UTX_boot_cause;          // 4

    // ===== EPS (44 B) =====
    uint16_t EPS_vbatt;               // 2
    uint8_t  EPS_output[8];           // 8
    uint16_t EPS_curout[6];           // 12
    uint16_t EPS_curin[3];            // 6
    uint16_t EPS_cursys;              // 2
    uint16_t EPS_counter_boot;        // 2
    uint32_t EPS_wdt_gnd_time_left;   // 4
    uint8_t  EPS_bootcause;           // 1
    uint8_t  EPS_battmode;            // 1
    uint8_t  EPS_batt_heater_mode;    // 1
    int16_t  EPS_BP4_Temperature[2];  // 4
    uint8_t  EPS_solar_panel_deploy;  // 1

    // ===== ADCS (14 B) =====
    uint8_t  ADCS_power_state;        // 1
    uint8_t  ADCS_control_mode;       // 1
    float    ADCS_gyro0_cal_rate_x;   // 4
    float    ADCS_gyro0_cal_rate_y;   // 4
    float    ADCS_gyro0_cal_rate_z;   // 4

    // ===== PAY UZURO (17 B) =====
    uint8_t  PAY_cam_mode;            // 1
    uint8_t  PAY_cam_memory_status[6];// 6
    uint8_t  PAY_cam_image_number[6]; // 6
    uint16_t PAY_deorbit_adc[2];      // 4
}__attribute__((packed)) Beacon;

static_assert(sizeof(Beacon) == 148, "Beacon packed size must be 148 bytes");

class BeaconHandler {
private:
    int fp = 0;
    char BeaconFileName[64];
public:
    Beacon * bec = nullptr;
    MYSQL  * sql = nullptr;

    int Save2File();
    int Save2DB();

    int File2DB();
	void parseBeaconData();

    BeaconHandler();
    ~BeaconHandler();
};

std::string beacon_summary(const Beacon& b);

#endif