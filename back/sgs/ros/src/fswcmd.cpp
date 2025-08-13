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