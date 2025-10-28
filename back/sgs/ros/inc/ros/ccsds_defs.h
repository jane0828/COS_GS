#pragma once
#include <cstdint>
#include <arpa/inet.h>

#pragma pack(push,1)
struct CcsdsPrimaryHdr {
    uint16_t ver_type_sec_apid; // [3b ver][1b type][1b sec flag][11b APID]
    uint16_t seq_flags_seq;     // [2b flags][14b seq]
    uint16_t pkt_len;           // bytes_after_this_field - 1
};
#pragma pack(pop)

static constexpr uint16_t IMAGE_CC_ID = 0x0878;

inline uint16_t ccsds_apid(const CcsdsPrimaryHdr* h){
    return ntohs(h->ver_type_sec_apid) & 0x07FFu;
}
inline bool ccsds_sec_hdr_flag(const CcsdsPrimaryHdr* h){
    return (ntohs(h->ver_type_sec_apid) >> 11) & 0x1;
}
inline uint16_t ccsds_seq(const CcsdsPrimaryHdr* h){
    return ntohs(h->seq_flags_seq) & 0x3FFFu;
}
inline uint16_t ccsds_total_len_bytes(const CcsdsPrimaryHdr* h){
    return (uint16_t)(ntohs(h->pkt_len) + 1u + 6u);
}

// 선택: 이미지 CCMessage_ID 상수

