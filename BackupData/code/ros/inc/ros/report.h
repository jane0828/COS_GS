#pragma once
#ifndef _SGS_REPORT_H_
#define _SGS_REPORT_H_

#include <cstdint>
#include <string>

#define REPORT_CC_ID 0x0825   // Report 메시지 식별자

#pragma pack(push,1)
struct Report {
    // ===== CCSDS Header (16 B) =====
    uint16_t CCMessage_ID;        
    uint16_t CCSequence;          
    uint16_t CCLength;            
    uint8_t  CCTime_code[6];      
    uint32_t CCPadding;           

    // ===== Report Body (136 B) =====
    uint16_t msg_id;              
    uint8_t  cc;                  
    uint8_t  ret_type;            
    int32_t  ret_code;            
    uint16_t ret_val_size;        
    uint8_t  ret_val[126];        
};
#pragma pack(pop)

static_assert(sizeof(Report) == 152, "Report size must be 152 bytes");

class ReportHandler {
private:
    int fp = 0;
    char ReportFileName[64];
public:
    Report* rep = nullptr;

    int Save2File();
    int Save2DB();   // 지금은 not implemented
    int File2DB();   // 지금은 not implemented
    void parseReportData();

    ReportHandler();
    ~ReportHandler();
};

std::string report_summary(const Report& r);

#endif
