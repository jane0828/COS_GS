// fswcmd.cpp — JSON → (CCSDS+ARG) → CSP v1 헤더 4B(BE) → (옵션) CSP CRC32 tail(LE) → Expedite 캡슐화(제조사 방식) → TCP 전송

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <climits>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <json/json.h>

// ======================= Expedite 상수(제조사 명칭) =======================
static constexpr uint32_t PREAMBLE   = 0xA1B2C3D4u;       // 프리앰블
static constexpr uint32_t POSTAMBLE  = ~PREAMBLE;         // 포스트앰블(비트반전)
static constexpr uint32_t TYPE_TC    = 0x01u;             // Telecommand
static constexpr uint8_t CSP_F_CRC32 = 0x01u;             // crc 쓸래말래

// =================== Expedite 헤더 (제조사 구조체) ===================
typedef struct __attribute__((packed)) _TC_HEADER {
    uint32_t nPreamble;   // Packet preamble
    uint32_t nMsglen;     // Total packet length
    uint32_t nMsgType;    // Message type
} TC_HEADER;
static_assert(sizeof(TC_HEADER) == 12, "TC_HEADER must be 12 bytes");

// ========================= 유틸 =========================
static inline std::string norm(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
    s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
    return s;
}

static inline void dump_hex(const uint8_t* b, size_t n){
    for(size_t i=0;i<n;++i){
        std::printf("%02X%s", b[i], ((i+1)%16==0 || i+1==n) ? "\n" : " ");
    }
}

// JSON → 숫자 변환들
static bool to_u64(const Json::Value& v, uint64_t& out){
    if(v.isString()){ try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
    if(v.isUInt64()){ out=v.asUInt64(); return true; }
    if(v.isUInt()){   out=v.asUInt();   return true; }
    if(v.isInt64()){  auto x=v.asInt64(); if(x<0) return false; out=(uint64_t)x; return true; }
    if(v.isInt()){    auto x=v.asInt();  if(x<0) return false; out=(uint64_t)x; return true; }
    if(v.isDouble()){ double d=v.asDouble(); if(d<0) return false; out=(uint64_t)d; return true; }
    return false;
}
static bool to_i64(const Json::Value& v, int64_t& out){
    if(v.isString()){ try{ size_t i=0; long long x=std::stoll(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
    if(v.isInt64()){ out=v.asInt64(); return true; }
    if(v.isInt()){   out=v.asInt();   return true; }
    if(v.isUInt64()){ auto x=v.asUInt64(); if(x>(uint64_t)LLONG_MAX) return false; out=(int64_t)x; return true; }
    if(v.isUInt()){  out=v.asUInt();  return true; }
    if(v.isDouble()){ out=(int64_t)v.asDouble(); return true; }
    return false;
}
static bool to_f32(const Json::Value& v, float& out){
    if(v.isString()){ try{ out=std::stof(v.asString()); }catch(...){ return false; } return true; }
    if(v.isDouble()){ out=(float)v.asDouble(); return true; }
    if(v.isInt()||v.isUInt()){ out=(float)v.asInt(); return true; }
    return false;
}
static bool to_f64(const Json::Value& v, double& out){
    if(v.isString()){ try{ out=std::stod(v.asString()); }catch(...){ return false; } return true; }
    if(v.isDouble()){ out=v.asDouble(); return true; }
    if(v.isInt()||v.isUInt()){ out=(double)v.asInt(); return true; }
    return false;
}
static bool to_bool1(const Json::Value& v, uint8_t& out){
    if(v.isBool()){ out = v.asBool()?1:0; return true; }
    if(v.isString()){
        auto s = norm(v.asString());
        if(s=="true"||s=="1"||s=="yes"||s=="on") { out=1; return true; }
        if(s=="false"||s=="0"||s=="no"||s=="off"){ out=0; return true; }
        try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
    }
    if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
    return false;
}

// ========================= Payload pack =========================
static bool pack_one_by_type(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload){
    const std::string t = norm(typeStr);

    if(t=="u8"||t=="uint8"||t=="byte"){
        uint64_t u; if(!to_u64(value,u)||u>0xFF) return false;
        payload.push_back((uint8_t)u); return true;
    }
    if(t=="u16"||t=="uint16"){
        uint64_t u; if(!to_u64(value,u)||u>0xFFFF) return false;
        uint16_t w=(uint16_t)u; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
    }
    if(t=="u32"||t=="uint32"){
        uint64_t u; if(!to_u64(value,u)||u>0xFFFFFFFFull) return false;
        uint32_t w=(uint32_t)u; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
    }
    if(t=="u64"||t=="uint64"){
        uint64_t u; if(!to_u64(value,u)) return false;
        uint8_t b[8]; std::memcpy(b,&u,8); payload.insert(payload.end(), b,b+8); return true;
    }
    if(t=="s8"||t=="int8"){
        int64_t s; if(!to_i64(value,s)||s<-128||s>127) return false;
        int8_t w=(int8_t)s; payload.push_back(*(uint8_t*)&w); return true;
    }
    if(t=="s16"||t=="int16"){
        int64_t s; if(!to_i64(value,s)||s<-32768||s>32767) return false;
        int16_t w=(int16_t)s; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
    }
    if(t=="s32"||t=="int32"){
        int64_t s; if(!to_i64(value,s)||s<(int64_t)INT32_MIN||s>(int64_t)INT32_MAX) return false;
        int32_t w=(int32_t)s; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
    }
    if(t=="s64"||t=="int64"){
        int64_t s; if(!to_i64(value,s)) return false;
        int64_t w=s; uint8_t b[8]; std::memcpy(b,&w,8); payload.insert(payload.end(), b,b+8); return true;
    }
    if(t=="float"||t=="f32"){
        float f; if(!to_f32(value,f)) return false;
        uint8_t b[4]; std::memcpy(b,&f,4); payload.insert(payload.end(), b,b+4); return true;
    }
    if(t=="double"||t=="f64"){
        double f; if(!to_f64(value,f)) return false;
        uint8_t b[8]; std::memcpy(b,&f,8); payload.insert(payload.end(), b,b+8); return true;
    }
    if(t=="bool"||t=="b1"||t=="boolean"){
        uint8_t b; if(!to_bool1(value,b)) return false;
        payload.push_back(b); return true;
    }
    if (t.rfind("str", 0) == 0) {
        int len = std::stoi(t.substr(3));
        if (len <= 0) return false;
        std::string s = value.asString();
        if ((int)s.size() > len) return false;

        std::vector<uint8_t> buf(len, 0);
        std::memcpy(buf.data(), s.data(), s.size());

        payload.insert(payload.end(), buf.begin(), buf.end());
        return true;
    }

    return false;
}

static bool pack_from_param_list(const Json::Value& params, std::vector<uint8_t>& payload){
    if(!params.isArray()) return false;
    payload.clear();
    for(const auto& p : params){
        if(!p.isObject()) return false;
        const std::string typ = p.get("type","").asString();
        const Json::Value& val = p["value"];
        if(!pack_one_by_type(typ, val, payload)) return false;
    }
    return true;
}

// ============ 내부 Telecommand ============
static std::vector<uint8_t> build_inner_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
    std::vector<uint8_t> cmd(8 + payload.size(), 0);
    uint16_t mid_be = htons(msgid);
    std::memcpy(&cmd[0], &mid_be, 2);
    cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
    if (msgid == 6262) {
        cmd[5] = 0x4D;
    }
    else cmd[5]=(uint8_t)(0x01 + payload.size());
    cmd[6]=cc;
    if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());
    uint16_t n=(uint16_t)cmd.size(); uint8_t cs=0xFF; const uint8_t* p=cmd.data();
    while(n--) cs ^= *p++;
    cmd[7]=cs;
    return cmd;
}

// ============ CSP v1 32-bit 헤더 ============
static inline uint32_t build_csp_id_be(uint8_t pri, uint8_t src, uint8_t dst,
                                       uint8_t sport, uint8_t dport, uint8_t flags){
    uint32_t ext =
        ((uint32_t)(pri   & 0x03) << 30) |
        ((uint32_t)(src   & 0x1F) << 25) |
        ((uint32_t)(dst   & 0x1F) << 20) |
        ((uint32_t)(dport & 0x3F) << 14) |
        ((uint32_t)(sport & 0x3F) <<  8) |
        ((uint32_t)(flags & 0xFF)      );
    return ext; // put_u32_be가 BE
}
static inline void put_u32_be(std::vector<uint8_t>& out, uint32_t v){
    out.push_back((uint8_t)((v >> 24) & 0xFF));
    out.push_back((uint8_t)((v >> 16) & 0xFF));
    out.push_back((uint8_t)((v >>  8) & 0xFF));
    out.push_back((uint8_t)( v        & 0xFF));
}

// ============ CRC32 (reflected) ============
static uint32_t GetCRC32(const std::vector<uint8_t>& u8vIn){
    // const uint32_t POLY = 0xEDB88320u;  // 원래 이게 맞는데
    const uint32_t POLY =0x04C11DB7 ;
    uint32_t crc = 0xFFFFFFFFu;
    for(uint8_t i : u8vIn){
        crc ^= i;
        for (int j = 0; j < 8; ++j)
            crc = (crc & 1u) ? ((crc >> 1) ^ POLY) : (crc >> 1);
    }
    return crc ^ 0xFFFFFFFFu;
}


// CSP CRC32 계산
static const uint32_t crc_tab[256] = {
    0x00000000, 0xF26B8303, 0xE13B70F7, 0x1350F3F4, 0xC79A971F, 0x35F1141C, 0x26A1E7E8, 0xD4CA64EB,
    0x8AD958CF, 0x78B2DBCC, 0x6BE22838, 0x9989AB3B, 0x4D43CFD0, 0xBF284CD3, 0xAC78BF27, 0x5E133C24,
    0x105EC76F, 0xE235446C, 0xF165B798, 0x030E349B, 0xD7C45070, 0x25AFD373, 0x36FF2087, 0xC494A384,
    0x9A879FA0, 0x68EC1CA3, 0x7BBCEF57, 0x89D76C54, 0x5D1D08BF, 0xAF768BBC, 0xBC267848, 0x4E4DFB4B,
    0x20BD8EDE, 0xD2D60DDD, 0xC186FE29, 0x33ED7D2A, 0xE72719C1, 0x154C9AC2, 0x061C6936, 0xF477EA35,
    0xAA64D611, 0x580F5512, 0x4B5FA6E6, 0xB93425E5, 0x6DFE410E, 0x9F95C20D, 0x8CC531F9, 0x7EAEB2FA,
    0x30E349B1, 0xC288CAB2, 0xD1D83946, 0x23B3BA45, 0xF779DEAE, 0x05125DAD, 0x1642AE59, 0xE4292D5A,
    0xBA3A117E, 0x4851927D, 0x5B016189, 0xA96AE28A, 0x7DA08661, 0x8FCB0562, 0x9C9BF696, 0x6EF07595,
    0x417B1DBC, 0xB3109EBF, 0xA0406D4B, 0x522BEE48, 0x86E18AA3, 0x748A09A0, 0x67DAFA54, 0x95B17957,
    0xCBA24573, 0x39C9C670, 0x2A993584, 0xD8F2B687, 0x0C38D26C, 0xFE53516F, 0xED03A29B, 0x1F682198,
    0x5125DAD3, 0xA34E59D0, 0xB01EAA24, 0x42752927, 0x96BF4DCC, 0x64D4CECF, 0x77843D3B, 0x85EFBE38,
    0xDBFC821C, 0x2997011F, 0x3AC7F2EB, 0xC8AC71E8, 0x1C661503, 0xEE0D9600, 0xFD5D65F4, 0x0F36E6F7,
    0x61C69362, 0x93AD1061, 0x80FDE395, 0x72966096, 0xA65C047D, 0x5437877E, 0x4767748A, 0xB50CF789,
    0xEB1FCBAD, 0x197448AE, 0x0A24BB5A, 0xF84F3859, 0x2C855CB2, 0xDEEEDFB1, 0xCDBE2C45, 0x3FD5AF46,
    0x7198540D, 0x83F3D70E, 0x90A324FA, 0x62C8A7F9, 0xB602C312, 0x44694011, 0x5739B3E5, 0xA55230E6,
    0xFB410CC2, 0x092A8FC1, 0x1A7A7C35, 0xE811FF36, 0x3CDB9BDD, 0xCEB018DE, 0xDDE0EB2A, 0x2F8B6829,
    0x82F63B78, 0x709DB87B, 0x63CD4B8F, 0x91A6C88C, 0x456CAC67, 0xB7072F64, 0xA457DC90, 0x563C5F93,
    0x082F63B7, 0xFA44E0B4, 0xE9141340, 0x1B7F9043, 0xCFB5F4A8, 0x3DDE77AB, 0x2E8E845F, 0xDCE5075C,
    0x92A8FC17, 0x60C37F14, 0x73938CE0, 0x81F80FE3, 0x55326B08, 0xA759E80B, 0xB4091BFF, 0x466298FC,
    0x1871A4D8, 0xEA1A27DB, 0xF94AD42F, 0x0B21572C, 0xDFEB33C7, 0x2D80B0C4, 0x3ED04330, 0xCCBBC033,
    0xA24BB5A6, 0x502036A5, 0x4370C551, 0xB11B4652, 0x65D122B9, 0x97BAA1BA, 0x84EA524E, 0x7681D14D,
    0x2892ED69, 0xDAF96E6A, 0xC9A99D9E, 0x3BC21E9D, 0xEF087A76, 0x1D63F975, 0x0E330A81, 0xFC588982,
    0xB21572C9, 0x407EF1CA, 0x532E023E, 0xA145813D, 0x758FE5D6, 0x87E466D5, 0x94B49521, 0x66DF1622,
    0x38CC2A06, 0xCAA7A905, 0xD9F75AF1, 0x2B9CD9F2, 0xFF56BD19, 0x0D3D3E1A, 0x1E6DCDEE, 0xEC064EED,
    0xC38D26C4, 0x31E6A5C7, 0x22B65633, 0xD0DDD530, 0x0417B1DB, 0xF67C32D8, 0xE52CC12C, 0x1747422F,
    0x49547E0B, 0xBB3FFD08, 0xA86F0EFC, 0x5A048DFF, 0x8ECEE914, 0x7CA56A17, 0x6FF599E3, 0x9D9E1AE0,
    0xD3D3E1AB, 0x21B862A8, 0x32E8915C, 0xC083125F, 0x144976B4, 0xE622F5B7, 0xF5720643, 0x07198540,
    0x590AB964, 0xAB613A67, 0xB831C993, 0x4A5A4A90, 0x9E902E7B, 0x6CFBAD78, 0x7FAB5E8C, 0x8DC0DD8F,
    0xE330A81A, 0x115B2B19, 0x020BD8ED, 0xF0605BEE, 0x24AA3F05, 0xD6C1BC06, 0xC5914FF2, 0x37FACCF1,
    0x69E9F0D5, 0x9B8273D6, 0x88D28022, 0x7AB90321, 0xAE7367CA, 0x5C18E4C9, 0x4F48173D, 0xBD23943E,
    0xF36E6F75, 0x0105EC76, 0x12551F82, 0xE03E9C81, 0x34F4F86A, 0xC69F7B69, 0xD5CF889D, 0x27A40B9E,
    0x79B737BA, 0x8BDCB4B9, 0x988C474D, 0x6AE7C44E, 0xBE2DA0A5, 0x4C4623A6, 0x5F16D052, 0xAD7D5351 };

// uint32_t csp_crc32_memory(const uint8_t * data, uint32_t length) {
// uint32_t crc;

// crc = 0xFFFFFFFF;
// while (length--)
//    crc = crc_tab[(crc ^ *data++) & 0xFFL] ^ (crc >> 8);
// return (crc ^ 0xFFFFFFFF);
// }


static uint32_t GetCSP_CRC32(const std::vector<uint8_t>& u8vIn) {
    constexpr size_t SKIP = 4;
    if (u8vIn.size() <= SKIP) {
        // 앞 4바이트를 건너뛰면 남는 데이터가 없음
        return 0u; // 필요에 맞게 반환값 조정 가능
    }

    uint32_t crc = 0xFFFFFFFFu;
    const uint8_t* begin = u8vIn.data() + SKIP;          // 4바이트 건너뜀
    const uint8_t* end   = u8vIn.data() + u8vIn.size();  // 끝

    for (const uint8_t* p = begin; p < end; ++p) {
        crc = crc_tab[(crc ^ *p) & 0xFFu] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}


// 빅엔디안 변환 (CRC)
static inline void append_u32_be(std::vector<uint8_t>& v, uint32_t x) {
    v.push_back((uint8_t)((x >>24) & 0xFF));
    v.push_back((uint8_t)((x >>16) & 0xFF));
    v.push_back((uint8_t)((x >> 8) & 0xFF));
    v.push_back((uint8_t)( x       & 0xFF));
}



// static uint32_t GetCSP_CRC32(const std::vector<uint8_t>& u8vIn){
//     const uint32_t POLY = 0xEDB88320u;  // 원래 이게 맞는데
//     // const uint32_t POLY =0x04C11DB7 ;
//     uint32_t crc = 0xFFFFFFFFu;
//     for(uint8_t i : u8vIn){
//         crc ^= i;
//         for (int j = 0; j < 8; ++j)
//             crc = (crc & 1u) ? ((crc >> 1) ^ POLY) : (crc >> 1);
//     }
//     return crc ^ 0xFFFFFFFFu;
// }




// ============ CSP CRC tail (옵션) 붙이기 ============
// static inline void maybe_append_csp_crc32_tail(std::vector<uint8_t>& tc_data, uint8_t flags){
//     if ((flags & 0x01u) == 0) return; // CSP CRC 사용 안 함
//     const uint32_t ccrc = GetCSP_CRC32(tc_data);          // 헤더(4B)+INNER
//     // tail은 LE
//     tc_data.push_back((uint8_t)( ccrc        & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >>  8) & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >> 16) & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >> 24) & 0xFF));
// }

// CRC LE
static inline void append_u32_le(std::vector<uint8_t>& v, uint32_t x) {
    v.push_back(static_cast<uint8_t>(x        & 0xFF));
    v.push_back(static_cast<uint8_t>((x >> 8) & 0xFF));
    v.push_back(static_cast<uint8_t>((x >>16) & 0xFF));
    v.push_back(static_cast<uint8_t>((x >>24) & 0xFF));
}

// CSP CRC tail (옵션) 붙이기: flags의 bit0(CSP_F_CRC32)가 켜져 있을 때만 tail 추가
static inline void maybe_append_csp_crc32_tail(std::vector<uint8_t>& tc_data, uint8_t flags){
    if ((flags & CSP_F_CRC32) == 0) return;         // 사용 안 함
    const uint32_t ccrc = GetCSP_CRC32(tc_data);    // 대상: "CSP 헤더(4B, BE) + INNER"
    append_u32_be(tc_data, ccrc);                   // tail은 Little-Endian
}


// ============ Expedite 캡슐화 (제조사 방식) ============
static std::vector<uint8_t> EncapsulateTC(const std::vector<uint8_t>& pnData){
    const uint32_t nMsglen = (uint32_t)(sizeof(TC_HEADER) + pnData.size() + 2 * sizeof(uint32_t));
    std::vector<uint8_t> pnOut; pnOut.clear();

    // 헤더(struct 그대로 memcpy)
    TC_HEADER sTCheader{};
    sTCheader.nPreamble = PREAMBLE;
    sTCheader.nMsgType  = TYPE_TC;
    sTCheader.nMsglen   = nMsglen;

    pnOut.resize(pnOut.size() + sizeof(TC_HEADER));
    std::memcpy(&pnOut[pnOut.size() - sizeof(TC_HEADER)], &sTCheader, sizeof(TC_HEADER));

    // 데이터 부착
    pnOut.resize(pnOut.size() + pnData.size());
    std::memcpy(&pnOut[pnOut.size() - pnData.size()], pnData.data(), pnData.size());

    // Expedite CRC32(헤더+데이터) — LE
    const uint32_t m_nCRC32 = GetCRC32(pnOut);
    pnOut.resize(pnOut.size() + sizeof(m_nCRC32));
    std::memcpy(&pnOut[pnOut.size() - sizeof(m_nCRC32)], &m_nCRC32, sizeof(m_nCRC32));

    // Postamble — LE
    const uint32_t m_nPostamble = POSTAMBLE;
    pnOut.resize(pnOut.size() + sizeof(m_nPostamble));
    std::memcpy(&pnOut[pnOut.size() - sizeof(m_nPostamble)], &m_nPostamble, sizeof(m_nPostamble));

    return pnOut;
}

// ============ TCP 전송 ============
static bool send_tcp_bytes(const std::string& ip, uint16_t port, const uint8_t* buf, size_t len){
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){ std::perror("socket"); return false; }
    sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_port = htons(port);
    if(::inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1){ std::perror("inet_pton"); ::close(fd); return false; }
    if(::connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0){ std::perror("connect"); ::close(fd); return false; }
    size_t sent=0;
    while(sent < len){
        ssize_t n = ::send(fd, buf+sent, len-sent, 0);
        if(n <= 0){ std::perror("send"); ::close(fd); return false; }
        sent += (size_t)n;
    }
    ::close(fd);
    return true;
}

// ======================== 공개 함수 ========================
// JSON → (CCSDS+ARG) → CSP4(BE) → (옵션) CSP CRC32 tail(LE) → Expedite(제조사) → TCP
void MakeTCData(const Json::Value& root){
    const Json::Value& m = root.isMember("message") ? root["message"] : root;

    // 1) payload 생성
    const uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
    const uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();

    std::vector<uint8_t> payload;
    if(!pack_from_param_list(m["parameters"], payload)){
        std::puts("[MakeTCData] parameter packing failed");
        return;
    }

    // 2) 내부(CCSDS+arg) 생성
    const auto inner_cmd = build_inner_cmd(msgid, cc, payload);

    // 3) CSP 4B 입력값
    uint8_t pri   = (uint8_t)m["csp"].get("pri",   1).asUInt();  // P1=HIGH
    uint8_t src   = (uint8_t)m["csp"].get("src",  28).asUInt();
    uint8_t dst   = (uint8_t)m["csp"].get("dst",   3).asUInt();
    uint8_t sport = (uint8_t)m["csp"].get("sport",10).asUInt();  // 필요 시 내부 에페메랄 증가 로직 가능
    uint8_t dport = (uint8_t)m["csp"].get("dport",13).asUInt();
    uint8_t flags = (uint8_t)m["csp"].get("flags", CSP_F_CRC32).asUInt();  // 1: CSP CRC32 tail 사용

    // 3-1) CSP 헤더 4B(BE) 생성
    const uint32_t csp_id_be = build_csp_id_be(pri, src, dst, sport, dport, flags);

    // 3-2) TC 데이터 = CSP4(BE) + INNER
    std::vector<uint8_t> tc_data;
    tc_data.reserve(4 + inner_cmd.size() + 4 /*crc opt*/);
    // CSP ID(BE)
    put_u32_be(tc_data, csp_id_be);
    // INNER CMD
    tc_data.insert(tc_data.end(), inner_cmd.begin(), inner_cmd.end());

    // // 3-3) (옵션) CSP CRC32 tail(LE)
    maybe_append_csp_crc32_tail(tc_data, flags);

    // 4) Expedite 캡슐화 (제조사 EncapsulateTC와 동일)
    const auto frame = EncapsulateTC(tc_data);

    // ===== debug =====
    std::printf("[CSP-HDR(BE)] %02X %02X %02X %02X\n",
        (uint8_t)((csp_id_be>>24)&0xFF),
        (uint8_t)((csp_id_be>>16)&0xFF),
        (uint8_t)((csp_id_be>> 8)&0xFF),
        (uint8_t)((csp_id_be>> 0)&0xFF));

    std::printf("[INNER CMD] %zu bytes\n", inner_cmd.size());
    dump_hex(inner_cmd.data(), inner_cmd.size());

    std::printf("[TC DATA  ] %zu bytes (CSP4 + INNER%s)\n",
                tc_data.size(), (flags&0x01) ? " + CRC32" : "");
    dump_hex(tc_data.data(), tc_data.size());

    // ★ 전송할 최종 프레임 원본 그대로 덤프
    std::printf("[TX/TCP] (Expedite) %zu bytes\n", frame.size());
    dump_hex(frame.data(), frame.size());
    std::fflush(stdout);

    // 5) TCP 전송
    std::string ip; int port = 0;
    if(m.isMember("IP"))   ip = m["IP"].asString();   else if(m.isMember("ip"))   ip = m["ip"].asString();
    if(m.isMember("Port")) port = m["Port"].asInt();  else if(m.isMember("port")) port = m["port"].asInt();

    if(!ip.empty() && port > 0){
        bool ok = send_tcp_bytes(ip, (uint16_t)port, frame.data(), frame.size());
        std::printf("[SEND] %s:%d -> %zu bytes %s\n", ip.c_str(), port, frame.size(), ok?"OK":"FAIL");
    }else{
        std::puts("[SEND] IP/Port not provided. Skipped sending.");
    }

    std::printf("[EXP.FRAME] %zu bytes\n", frame.size());
}
