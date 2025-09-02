#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif


SGS_FSWCMD::SGS_FSWCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
    : SGS_PING(udp, node, port)
{
    memset(this->CMD, NULL, sizeof(this->CMD));
}

SGS_FSWCMD::~SGS_FSWCMD()
{
    this->ClearAll();
}

int SGS_FSWCMD::AllocIdx(int len)
{
    int i;
    for(i = 0; i < sizeof(CMD) / sizeof(CFE_MSG*); i++)
    {
        if(CMD[i] == NULL)
            break;
    }
    this->CMD[i] = (CFE_MSG *)malloc(len);
    return i;
}

void SGS_FSWCMD::ClearIdx(int idx)
{
    if(!this->CMD[idx])
        return;
    delete this->CMD[idx];
    this->CMD[idx] = NULL;
    return;
}

void SGS_FSWCMD::ClearAll()
{
    for(int i = 0; i < sizeof(CMD) / sizeof(CFE_MSG*); i++)
        this->ClearIdx(i);
}

void SGS_FSWCMD::SetHeaderWord(uint8_t* Word16, uint16_t Value, uint16_t Mask) {
    Word16[0] = (Word16[0] & ~(Mask >> 8)) | ((Value & Mask) >> 8);
    Word16[1] = ((Word16[1] & ~Mask) | (Value & Mask)) & 0xFF;
}

void SGS_FSWCMD::SetHeaderByte(uint8_t* Byte, uint8_t Value, uint8_t Mask) {
    Byte[0] = (Byte[0] & ~Mask) | (Value & Mask);
}

int SGS_FSWCMD::SetHasSecondaryHeader(CFE_MSG * msg, bool HasSec) {
    SetHeaderByte(&msg->header.Msg.CCSDS.Pri.StreamId[0], HasSec, 0x08);
    return 0;
}

int SGS_FSWCMD::SetMsgId(CFE_MSG * msg, uint16_t MsgId) {
    SetHeaderWord(msg->header.Msg.CCSDS.Pri.StreamId, MsgId, 0xFFFF);
    return 0;
}

int SGS_FSWCMD::SetSize(CFE_MSG * msg, uint16_t Size) {
    if (Size < CFE_MSG_SIZE_OFFSET || Size >(0xFFFF + CFE_MSG_SIZE_OFFSET)) {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }
    Size -= CFE_MSG_SIZE_OFFSET;
    SetHeaderWord(msg->header.Msg.CCSDS.Pri.Length, Size, 0xFFFF);
    return 0;
}

int SGS_FSWCMD::SetSegmentationFlag(CFE_MSG * msg, uint16_t SegFlag) {
    SetHeaderWord(msg->header.Msg.CCSDS.Pri.Sequence, SegFlag, CFE_MSG_SEGFLG_MASK);
    return 0;
}

int SGS_FSWCMD::SetFncCode(CFE_MSG * msg, uint16_t FncCode) {
    if (!this->HasSecondaryHeader(msg)) {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }
    msg->header.Sec.FunctionCode = FncCode;
    return 0;
}

void SGS_FSWCMD::GetHeaderWord(const uint8_t* ByteField, uint16_t& Value, uint16_t Mask) {
    Value = (ByteField[0] << 8 | ByteField[1]) & Mask;
}

uint16_t SGS_FSWCMD::GetSize(CFE_MSG * msg) {
    uint16_t RetVal;
    GetHeaderWord(msg->header.Msg.CCSDS.Pri.Length, RetVal, 0xFFFF);
    return RetVal + CFE_MSG_SIZE_OFFSET;
}

uint16_t SGS_FSWCMD::GetFncCode(CFE_MSG * msg)
{
    return msg->header.Sec.FunctionCode;
}

void SGS_FSWCMD::CopyCmdHeaderToBuffer(CFE_MSG * msg, uint8_t* Buffer) {
    memcpy(Buffer, msg->header.Msg.Byte, this->GetSize(msg));
}

uint32_t SGS_FSWCMD::ComputeCheckSum(CFE_MSG * msg) {
    uint16_t Len = this->GetSize(msg);
    const uint8_t* BytePtr = msg->header.Msg.Byte;
    uint32_t chksum = 0xFF;

    while (Len--) {
        chksum ^= *(BytePtr++);
    }
    return chksum;
}

bool SGS_FSWCMD::HasSecondaryHeader(CFE_MSG * msg) const {
    return (msg->header.Msg.CCSDS.Pri.StreamId[0] & (CFE_MSG_SHDR_MASK >> 8)) != 0;
}

int SGS_FSWCMD::GenerateChecksum(CFE_MSG * msg) {
    msg->header.Sec.Checksum = 0;
    msg->header.Sec.Checksum = this->ComputeCheckSum(msg);
    return 0;
}



int SGS_FSWCMD::GenerateCmdHeader(CFE_MSG * msg, uint32_t MsgId, uint16_t FncCode) {
    if (this->SetHasSecondaryHeader(msg, true) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }
    if (this->SetSegmentationFlag(msg, CFE_MSG_SEGFLG_UNSEG) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }
    if (this->SetMsgId(msg, MsgId) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }    

    if (this->SetFncCode(msg, FncCode) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }    
    if (this->GenerateChecksum(msg) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }    
    return CMDTYPE_CMD_OK;
}

int SGS_FSWCMD::GenerateCmdData(CFE_MSG * msg, void * Data, uint32_t datasize)
{
    memcpy(msg->data, Data, datasize);
}

int SGS_FSWCMD::GenerateCmdSchedule(CFE_MSG * msg, sgs_cmd_schinfo_t * schinfo)
{
    uint32_t wholesz = this->GetSize(msg) + sizeof(sgs_cmd_schinfo_t);
    sgs_cmd_sch_t * data = (sgs_cmd_sch_t *)malloc(wholesz);
    memcpy(&data->sch, schinfo, sizeof(sgs_cmd_schinfo_t));
    memcpy(&data->msg, msg, this->GetSize(msg));
    return GenerateCMD(TS_CMD_MID, TS_INSERT_SCHEDULE_ENTRY_CC, wholesz, data, NULL);
}

int SGS_FSWCMD::GenerateCMD(uint32_t MsgId, uint16_t FncCode, uint32_t Size, void* Data, sgs_cmd_schinfo_t * schinfo)
{
    uint32_t Headlen = sizeof(CFE_MSG_CommandHeader);
    uint32_t DataSize = Size - Headlen;



    CFE_MSG * msg = (CFE_MSG *)malloc(Size);
    memset(msg, 0, Size);

    this->GenerateCmdHeader(msg, MsgId, FncCode);

    if (this->SetSize(msg, Size) != 0)
    {
        this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
        return CMDTYPE_CMD_ERR_ALLOC;
    }

    if(Data)
        this->GenerateCmdData(msg, Data, DataSize);

    if(schinfo)
        return this->GenerateCmdSchedule(msg, schinfo);
    else
    {
        int idx = this->AllocIdx(Size);
        memset(this->CMD[idx], 0, Size);
        memcpy(this->CMD[idx], msg, Size);
        return idx;
    }
}