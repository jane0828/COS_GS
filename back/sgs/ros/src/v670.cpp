#include <ros/ros.h>
#include <inttypes.h>

uint16_t CFE_V670_Msg::ReadBits(uint16_t word, uint16_t mask, uint8_t shift) {
    return (word & mask) >> shift;
}

void CFE_V670_Msg::WriteBits(uint16_t &word, uint16_t mask, uint8_t shift, uint16_t value) {
    word = (uint16_t)((word & ~mask) | ((value & (mask >> shift)) << shift));
}

// Functions to read and write primary header fields
uint16_t CFE_V670_Msg::ReadStreamId() {
    return (Pri.StreamId[0] << 8) + Pri.StreamId[1];
}

void CFE_V670_Msg::WriteStreamId(uint16_t value) {
    Pri.StreamId[0] = value >> 8;
    Pri.StreamId[1] = value & 0xFF;
}

uint16_t CFE_V670_Msg::ReadAPID() {
    return ReadStreamId() & 0x07FF;
}

void CFE_V670_Msg::WriteAPID(uint16_t value) {
    Pri.StreamId[0] = (Pri.StreamId[0] & 0xF8) | (value >> 8 & 0x07);
    Pri.StreamId[1] = value & 0xFF;
}

uint8_t CFE_V670_Msg::ReadSHDR() {
    return (Pri.StreamId[0] & 0x08) >> 3;
}

void CFE_V670_Msg::WriteSHDR(uint8_t value) {
    Pri.StreamId[0] = (Pri.StreamId[0] & 0xF7) | ((value << 3) & 0x08);
}

uint8_t CFE_V670_Msg::ReadType() {
    return (Pri.StreamId[0] & 0x10) >> 4;
}

void CFE_V670_Msg::WriteType(uint8_t value) {
    Pri.StreamId[0] = (Pri.StreamId[0] & 0xEF) | ((value << 4) & 0x10);
}

uint8_t CFE_V670_Msg::ReadVersion() {
    return (Pri.StreamId[0] & 0xE0) >> 5;
}

void CFE_V670_Msg::WriteVersion(uint8_t value) {
    Pri.StreamId[0] = (Pri.StreamId[0] & 0x1F) | ((value << 5) & 0xE0);
}

uint16_t CFE_V670_Msg::ReadSequence() {
    return ((Pri.Sequence[0] & 0x3F) << 8) + Pri.Sequence[1];
}

void CFE_V670_Msg::WriteSequence(uint16_t value) {
    Pri.Sequence[0] = (Pri.Sequence[0] & 0xC0) | (value >> 8 & 0x3F);
    Pri.Sequence[1] = value & 0xFF;
}

uint8_t CFE_V670_Msg::ReadSeqFlag() {
    return (Pri.Sequence[0] & 0xC0) >> 6;
}

void CFE_V670_Msg::WriteSeqFlag(uint8_t value) {
    Pri.Sequence[0] = (Pri.Sequence[0] & 0x3F) | ((value << 6) & 0xC0);
}

uint16_t CFE_V670_Msg::ReadLength() {
    return (Pri.Length[0] << 8) + Pri.Length[1] + 7;
}

void CFE_V670_Msg::WriteLength(uint16_t value) {
    Pri.Length[0] = (value - 7) >> 8;
    Pri.Length[1] = (value - 7) & 0xFF;
}

// Clearing functions
void CFE_V670_Msg::ClearPrimaryHeader() {
    Pri.StreamId[0] = 0;
    Pri.StreamId[1] = 0;
    Pri.Sequence[0] = (CCSDS_INIT_SEQFLG_V670 << 6);
    Pri.Sequence[1] = 0;
    Pri.Length[0] = 0;
    Pri.Length[1] = 0;
}
public:
void CFE_V670_Msg::SetMsgId(uint16_t MsgId) {
    return this->WriteStreamId(MsgId);
}

void CFE_V670_Msg::SetLength(uint16_t Length) {
    return this->WriteLength(Length);
}

uint16_t CFE_V670_Msg::GetMsgId() {
    return this->ReadStreamId();
}

uint16_t CFE_V670_Msg::GetLength() {
    return this->ReadLength();
}

void CFE_V670_Msg::InitPriHdr(uint16_t MsgId, uint16_t Length, bool Clear){
    uint16_t           SeqCount;
    /* Save the sequence count in case it must be preserved. */
    SeqCount = this->ReadSequence();
    
    this->ClearPrimaryHeader();
    
    /* Set the length fields in the primary header. */
    this->WriteLength(Length);
    
    /* Always set the secondary header flag as CFS applications are required use it */
    this->WriteSHDR(1);
    
    this->WriteStreamId(MsgId);
    
    /* Restore the sequence count if needed. */
    if (!Clear)  
        this->WriteSequence(SeqCount);
    else
        this->WriteSeqFlag(CCSDS_INIT_SEQFLG_V670);
}


uint8_t CFE_V670_CmdMsg::ReadFunctionCode() {
    return ReadBits(Sec.Command, 0x7F00, 8);
}

void CFE_V670_CmdMsg::WriteFunctionCode(uint8_t value) {
    WriteBits(Sec.Command, 0x7F00, 8, value);
}

uint8_t CFE_V670_CmdMsg::ReadChecksum() {
    return ReadBits(Sec.Command, 0x00FF, 0);
}

void CFE_V670_CmdMsg::WriteChecksum(uint8_t value) {
    WriteBits(Sec.Command, 0x00FF, 0, value);
}

void CFE_V670_CmdMsg::ClearCommandHeader() {
    Sec.Command = (CCSDS_INIT_CHECKSUM_V670 << 0) | (CCSDS_INIT_FC_V670 << 8);
}

uint8_t CFE_V670_CmdMsg::CCSDS_ComputeCheckSum ()
{
    uint16_t   PktLen   = this->ReadLength();
    uint8_t   *BytePtr  = (uint8_t *)this->MsgPtr;
    uint8_t    CheckSum = 0xFF;
    while (PktLen--)
        CheckSum ^= *(BytePtr++);

    return CheckSum;
} /* END CCSDS_ComputeCheckSum() */

void CFE_V670_CmdMsg::UpdateCksumSync() {
    /* if msg type is telemetry or there is no secondary hdr... */
    if((this->ReadType() == CCSDS_TLM_V670)||(this->ReadSHDR() == 0)){
        return;
    }/* end if */
    
    memset(this->MsgPtr, 0, sizeof(CCSDS_CmdHdr_V670_t));
    memcpy(&this->MsgPtr->Hdr.Pri, &this->Pri, sizeof(CCSDS_PriHdr_V670_t));
    memcpy(&this->MsgPtr->Hdr.Sec, &this->Sec, sizeof(CCSDS_CmdSecHdr_V670_t));

    this->WriteChecksum(CCSDS_ComputeCheckSum());
}

void CFE_V670_CmdMsg::UpdateCmdHdr(uint16_t MsgId, uint16_t CmdCode, uint16_t Length) {
    if(this->MsgPtr == nullptr)
    {
        this->MsgPtr = (CCSDS_CmdMsg_V670_t*)malloc(Length);
    }
    this->InitPriHdr(MsgId, Length, true);
    this->SetCmdCode(CmdCode);
    this->WriteType(CCSDS_CMD_V670);
    this->UpdateCksumSync();
}

void CFE_V670_CmdMsg::SetCmdCode(uint16_t CmdCode) {

    /* if msg type is telemetry or there is no secondary hdr... */
    if((this->ReadType() == CCSDS_TLM_V670)||(this->ReadSHDR() == 0)){
        return;
    }/* end if */

    this->WriteFunctionCode(CmdCode);
    this->UpdateCksumSync();
    return;
}

uint16_t CFE_V670_CmdMsg::GetCmdCode() {
    /* if msg type is telemetry or there is no secondary hdr, return 0 */
    if((this->ReadType() == CCSDS_TLM_V670)||(this->ReadSHDR() == 0)){
        return 0;
    }/* end if */

    return this->ReadFunctionCode();
}

CFE_V670_CmdMsg::CFE_V670_CmdMsg(uint16_t MsgId, uint16_t CmdCode, void* UserData, uint16_t UserDataLen) {
    uint16_t TotalLength = sizeof(this->Pri) + sizeof(this->Sec) + UserDataLen;
    this->MsgPtr = (CCSDS_CmdMsg_V670_t*)malloc(TotalLength);
    if(UserData && UserDataLen)
        memcpy(this->MsgPtr->Data, UserData, UserDataLen);
    this->UpdateCmdHdr(MsgId, CmdCode, TotalLength);
}

CFE_V670_CmdMsg::~CFE_V670_CmdMsg() {
    free(this->MsgPtr);
    this->MsgPtr = nullptr;
}

void CFE_V670_TlmMsg::ClearTelemetrySecondaryHeader() {
    memset(&Sec.Time, 0, sizeof(Sec.Time));
}
void CFE_V670_TlmMsg::SetTimeStamp() {

}

CFE_V670_TlmMsg::CFE_V670_TlmMsg(uint16_t MsgId, void* UserData, uint16_t UserDataLen) {
    uint16_t TotalLength = sizeof(this->Pri) + sizeof(this->Sec) + UserDataLen;
    this->MsgPtr = (CCSDS_TlmMsg_V670_t*)malloc(TotalLength);
    if(UserData && UserDataLen)
        memcpy(this->MsgPtr->Data, UserData, UserDataLen);
    this->InitPriHdr(MsgId, TotalLength, true);
    this->WriteType(CCSDS_TLM_V670);
    this->SetTimeStamp();
    memcpy(&this->MsgPtr->Hdr.Pri, &this->Pri, sizeof(CCSDS_PriHdr_V670_t));
    memcpy(&this->MsgPtr->Hdr.Sec, &this->Sec, sizeof(CCSDS_TlmSecHdr_V670_t));
}

CFE_V670_TlmMsg::CFE_V670_TlmMsg(CCSDS_TlmMsg_V670_t* _MsgPtr)
{
    if(_MsgPtr)
    {
        memcpy(&this->Pri, &_MsgPtr->Hdr.Pri, sizeof(CCSDS_PriHdr_V670_t));
        memcpy(&this->Sec, &_MsgPtr->Hdr.Sec, sizeof(CCSDS_TlmSecHdr_V670_t));
        uint16_t TotalLength = this->GetLength();
        this->MsgPtr = (CCSDS_TlmMsg_V670_t*)malloc(TotalLength);
        memcpy(this->MsgPtr, _MsgPtr, TotalLength);
    }
}

CFE_V670_TlmMsg::~CFE_V670_TlmMsg() {
    free(this->MsgPtr);
    this->MsgPtr = nullptr;
}