#pragma once
#ifndef _SGS_BEACON_H_
#define _SGS_BEACON_H_
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <mysql/mysql.h>
#endif


// typedef struct {
// 	//Callsign
// 	char CallSign[5];

// 	//CCSDS Header
// 	uint16_t CCMessage_ID;
// 	uint16_t CCSequence;
// 	uint16_t CCLength;
// 	uint8_t CCTime_code[6];
// 	uint8_t CCPadding[4];

// 	//FM
// 	uint32_t FMSpaceTime;
// 	uint16_t FMRebootCount;
// 	uint8_t FMCurMode;
// 	uint8_t FmCurSubMode;
// 	uint8_t FMPreviousMode;
// 	uint8_t FMPreviousSubMode;

// 	//EPS->System
// 	uint8_t SYMode;
// 	uint8_t SYResetCause;
// 	uint16_t SYResetcount;

// 	//EPS->Battery
// 	uint16_t BAtteryStatus;
// 	int16_t BAtteryInV;
// 	int16_t BAtteryInI;
// 	int16_t BAtteryTemp;

// 	//EPS->Motherboard
// 	int16_t MOMPPTInV[3];
// 	int16_t MOMPPTInI[3];
// 	int16_t MOChancur3V[4];
// 	int16_t MOChancur5V[4];
// 	int16_t MOBoardSupV;
// 	int16_t MOBoardTemp;

// 	//EPS->Daughterboard
// 	int16_t DAMPPTInV[2];
// 	int16_t DAMPPTInI[2];
// 	int16_t DAChancur5V;
// 	int16_t DAChancur12V;

// 	//SNSR->IMU
// 	float IMTemp;
// 	float IMAngV[3];
// 	uint16_t IMRWErrcount;

// 	//UTRX
// 	uint32_t UXTotRXByte;
// 	uint16_t UXBootCount;
// 	uint8_t UXActiveConf;
// }__attribute__((packed)) Beacon;


/*****************************/
/*                           */
/*          COSMIC           */
/*                           */
/*****************************/

typedef struct {

    uint8_t  CCMessage_ID;
    uint8_t  CCSequence;
    uint8_t  CCLength;
    uint8_t  CCTime_code[6];

    uint8_t  SRL_CommandCounter;
    uint8_t  SRL_CommandErrorCounter;
    uint8_t  SRL_I2C0_Status;
    uint16_t SRL_I2C0_TxCount;
    uint8_t  SRL_I2C1_Status;
    uint16_t SRL_I2C1_TxCount;
    uint8_t  SRL_I2C2_Status;
    uint16_t SRL_I2C2_TxCount;
    uint8_t  SRL_UART_Status;
    uint16_t SRL_UART_TxCount;
    uint8_t  SRL_RS422_Status;
    uint16_t SRL_RS422_TxCount;

    uint8_t  RPT_ReportQueueCount;
    uint8_t  RPT_CriticalQueueCount;
    uint16_t RPT_BootCount;
    uint32_t RPT_SpacecraftTime_s;
    uint32_t RPT_SpacecraftTime_subs;
    uint32_t RPT_Sequence;
    uint8_t  RPT_ResetCause;
    uint16_t RPT_CurrentMode;
    uint16_t RPT_PreviousMode;

    uint8_t  STRX_status;
    int16_t  STRX_last_rssi;
    uint16_t STRX_bootcount;
    uint32_t STRX_bootcause;
    uint8_t  STRX_rx_mode;
    uint16_t STRX_gnd_wdt_cnt;
    uint32_t STRX_gnd_wdt_left;

    uint16_t UANT_ReportDeploymentStatus;

    uint8_t  UTX_active_conf;      /* addr 0x0018 */
    uint16_t UTX_boot_count;       /* addr 0x0020 */
    uint32_t UTX_boot_cause;       /* addr 0x0024 */

    uint16_t EPS_vbatt;
    uint8_t  EPS_output[8];
    uint16_t EPS_curout[6];
    uint16_t EPS_curin[3];
    uint16_t EPS_cursys;
    uint16_t EPS_counter_boot;
    uint32_t EPS_wdt_gnd_time_left;
    uint8_t  EPS_bootcause;
    uint8_t  EPS_battmode;

    uint8_t  Solar_DeployStatus;

    uint8_t  ADCS_PowerState;
    uint8_t  ADCS_ControlMode;
    float    ADCS_GyroCalRateX;
    float    ADCS_GyroCalRateY;
    float    ADCS_GyroCalRateZ;

    uint8_t  PAY_CAM_Mode;
    uint8_t  PAY_CAM_MemoryStatus;
    uint8_t  PAY_CAM_ImageNumber;

    uint16_t PAY_DEORBIT_ADCvalue;
} __attribute__((packed)) Beacon;



class BeaconHandler {
private:
    int fp = 0;
    char BeaconFileName[64];
public:
    Beacon * bec;
    MYSQL * sql;

    int Save2File();
    int Save2DB();

    int File2DB();

    BeaconHandler();
    ~BeaconHandler();

};

#endif