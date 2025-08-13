#pragma once
#ifndef _SGS_BEACON_H_
#define _SGS_BEACON_H_
#ifndef _SGS_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <mysql/mysql.h>
#endif


typedef struct {
	//Callsign
	char CallSign[5];

	//CCSDS Header
	uint16_t CCMessage_ID;
	uint16_t CCSequence;
	uint16_t CCLength;
	uint8_t CCTime_code[6];
	uint8_t CCPadding[4];

	//FM
	uint32_t FMSpaceTime;
	uint16_t FMRebootCount;
	uint8_t FMCurMode;
	uint8_t FmCurSubMode;
	uint8_t FMPreviousMode;
	uint8_t FMPreviousSubMode;

	//EPS->System
	uint8_t SYMode;
	uint8_t SYResetCause;
	uint16_t SYResetcount;

	//EPS->Battery
	uint16_t BAtteryStatus;
	int16_t BAtteryInV;
	int16_t BAtteryInI;
	int16_t BAtteryTemp;

	//EPS->Motherboard
	int16_t MOMPPTInV[3];
	int16_t MOMPPTInI[3];
	int16_t MOChancur3V[4];
	int16_t MOChancur5V[4];
	int16_t MOBoardSupV;
	int16_t MOBoardTemp;

	//EPS->Daughterboard
	int16_t DAMPPTInV[2];
	int16_t DAMPPTInI[2];
	int16_t DAChancur5V;
	int16_t DAChancur12V;

	//SNSR->IMU
	float IMTemp;
	float IMAngV[3];
	uint16_t IMRWErrcount;

	//UTRX
	uint32_t UXTotRXByte;
	uint16_t UXBootCount;
	uint8_t UXActiveConf;
}__attribute__((packed)) Beacon;

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