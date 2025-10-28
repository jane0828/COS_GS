#ifndef _ROS_CFE_V670_H_
#define _ROS_CFE_V670_H_

#include <inttypes.h>
#include <utl/utl.h>

/**********************************************************************
** Constant values.
**********************************************************************/

/* Value of packet type for a telemetry packet. */
#define CCSDS_TLM_V670              0
/* Value of packet type for a command packet. */
#define CCSDS_CMD_V670              1

/* Value of secondary header flag if secondary header not present. */
#define CCSDS_NO_SEC_HDR_V670       0
/* Value of secondary header flag if secondary header exists. */
#define CCSDS_HAS_SEC_HDR_V670      1

#define NUM_CCSDS_APIDS_V670        2048
#define NUM_CCSDS_PKT_TYPES_V670    2


/**********************************************************************
** Initial values for CCSDS header fields.
**********************************************************************/

/* Initial value of the sequence count. */
#define CCSDS_INIT_SEQ_V670         0
/* Initial value of the sequence flags. */
#define CCSDS_INIT_SEQFLG_V670      3
/* Initial value of the command function code. */
#define CCSDS_INIT_FC_V670          0
/* Initial value of the command checksum. */
#define CCSDS_INIT_CHECKSUM_V670    0

#define CCSDS_TIME_SZ_V670          6

typedef struct {
    uint8_t StreamId[2];  /* packet identifier word (stream ID) */
    /*  bits  shift   ------------ description ---------------- */
    /* 0x07FF    0  : application ID                            */
    /* 0x0800   11  : secondary header: 0 = absent, 1 = present */
    /* 0x1000   12  : packet type:      0 = TLM, 1 = CMD        */
    /* 0xE000   13  : CCSDS version:    0 = ver 1, 1 = ver 2    */

    uint8_t Sequence[2];  /* packet sequence word */
    /*  bits  shift   ------------ description ---------------- */
    /* 0x3FFF    0  : sequence count                            */
    /* 0xC000   14  : segmentation flags:  3 = complete packet  */

    uint8_t Length[2];     /* packet length word */
    /*  bits  shift   ------------ description ---------------- */
    /* 0xFFFF    0  : (total packet length) - 7                 */

}__attribute__((__packed__)) CCSDS_PriHdr_V670_t;

typedef struct {
    uint16_t Command;      /* command secondary header */
    /*  bits  shift   ------------ description ---------------- */
    /* 0x00FF    0  : checksum, calculated by ground system     */
    /* 0x7F00    8  : command function code                     */
    /* 0x8000   15  : reserved, set to 0                        */
}__attribute__((__packed__)) CCSDS_CmdSecHdr_V670_t;

typedef struct {
    uint8_t Time[CCSDS_TIME_SZ_V670];
}__attribute__((__packed__)) CCSDS_TlmSecHdr_V670_t;

typedef struct {
    CCSDS_PriHdr_V670_t Pri;
    CCSDS_CmdSecHdr_V670_t Sec;
}__attribute__((__packed__)) CCSDS_CmdHdr_V670_t;

typedef struct {
    CCSDS_CmdHdr_V670_t Hdr;
    uint8_t Data[];
}__attribute__((__packed__)) CCSDS_CmdMsg_V670_t;

typedef struct {
    CCSDS_PriHdr_V670_t Pri;
    CCSDS_TlmSecHdr_V670_t Sec;
}__attribute__((__packed__)) CCSDS_TlmHdr_V670_t;

typedef struct {
    CCSDS_TlmHdr_V670_t Hdr;
    uint8_t Data[];
}__attribute__((__packed__)) CCSDS_TlmMsg_V670_t;

class CFE_V670_Msg
{
protected:
    CCSDS_PriHdr_V670_t Pri;
    // Function for reading specific bit fields
    uint16_t ReadBits(uint16_t word, uint16_t mask, uint8_t shift);
    void WriteBits(uint16_t &word, uint16_t mask, uint8_t shift, uint16_t value);

    // Functions to read and write primary header fields
    uint16_t ReadStreamId();
    void WriteStreamId(uint16_t value);
    uint16_t ReadAPID();
    void WriteAPID(uint16_t value);
    uint8_t ReadSHDR();
    void WriteSHDR(uint8_t value);
    uint8_t ReadType();
    void WriteType(uint8_t value);
    uint8_t ReadVersion();
    void WriteVersion(uint8_t value);
    uint16_t ReadSequence();
    void WriteSequence(uint16_t value);
    uint8_t ReadSeqFlag();
    void WriteSeqFlag(uint8_t value);
    uint16_t ReadLength();
    void WriteLength(uint16_t value);

    // Clearing functions
    void ClearPrimaryHeader();

public:
    void SetMsgId(uint16_t MsgId);
    void SetLength(uint16_t Length);
    uint16_t GetMsgId();
    uint16_t GetLength();

    void InitPriHdr(uint16_t MsgId, uint16_t Length, bool Clear);
};
    
    

class CFE_V670_CmdMsg : public CFE_V670_Msg
{
private:
    CCSDS_CmdSecHdr_V670_t Sec;
    CCSDS_CmdMsg_V670_t* MsgPtr = nullptr;
    // Command Header Fields
    uint8_t ReadFunctionCode();
    void WriteFunctionCode(uint8_t value);
    uint8_t ReadChecksum();
    void WriteChecksum(uint8_t value);
    void ClearCommandHeader();
    uint8_t CCSDS_ComputeCheckSum();
    void UpdateCksumSync();
    void UpdateCmdHdr(uint16_t MsgId, uint16_t CmdCode, uint16_t Length);

public:
    void SetCmdCode(uint16_t CmdCode);
    uint16_t GetCmdCode();
    CFE_V670_CmdMsg(uint16_t MsgId, uint16_t CmdCode, void* UserData, uint16_t UserDataLen);
    ~CFE_V670_CmdMsg();
};

class CFE_V670_TlmMsg : public CFE_V670_Msg
{
private:
    CCSDS_TlmSecHdr_V670_t Sec;
    CCSDS_TlmMsg_V670_t* MsgPtr = nullptr;
    void ClearTelemetrySecondaryHeader();
    void SetTimeStamp();

public:
    CFE_V670_TlmMsg(uint16_t MsgId, void* UserData, uint16_t UserDataLen);
    CFE_V670_TlmMsg(CCSDS_TlmMsg_V670_t* _MsgPtr);
    ~CFE_V670_TlmMsg();
};

#endif