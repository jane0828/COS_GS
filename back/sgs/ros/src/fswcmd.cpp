#include <ros/ros.h>


// SGS_FSWCMD::SGS_FSWCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
//     : SGS_PING(udp, node, port)
// {
//     memset(this->CMD, NULL, sizeof(this->CMD));
// }

// SGS_FSWCMD::~SGS_FSWCMD()
// {
//     this->ClearAll();
// }

// int SGS_FSWCMD::GenerateCMD(uint32_t MsgId, uint16_t FncCode, uint32_t Size, void* Data, sgs_cmd_schinfo_t * schinfo)
// {
//     uint32_t Headlen = sizeof(CFE_MSG_CommandHeader);
//     uint32_t DataSize = Size - Headlen;



//     CFE_MSG * msg = (CFE_MSG *)malloc(Size);
//     memset(msg, 0, Size);

//     this->GenerateCmdHeader(msg, MsgId, FncCode);

//     if (this->SetSize(msg, Size) != 0)
//     {
//         this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
//         return CMDTYPE_CMD_ERR_ALLOC;
//     }

//     if(Data)
//         this->GenerateCmdData(msg, Data, DataSize);

//     if(schinfo)
//         return this->GenerateCmdSchedule(msg, schinfo);
//     else
//     {
//         int idx = this->AllocIdx(Size);
//         memset(this->CMD[idx], 0, Size);
//         memcpy(this->CMD[idx], msg, Size);
//         return idx;
//     }
// }


// ---------- utils ----------
static inline void dump_hex(const uint8_t* b, size_t n){
    for(size_t i=0;i<n;++i) std::printf("0x%x ", b[i]);
    std::printf("\n");
}
static inline std::string norm(std::string s){
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
    s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
    return s;
}
// 문자열/숫자 → u64/s64/f32/f64/bool
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
        // 숫자 문자열도 허용
        try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
    }
    if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
    return false;
}

// ---------- one param → payload append ----------
// 주의: 페이로드 엔디안은 네 기존 코드처럼 host 그대로 memcpy.
// 장비가 BE 요구하면 여기서 htons/htonl/수동스왑 적용.
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
    return false; // 미지원 타입 문자열
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

// ---------- build header + checksum & print ----------
static std::vector<uint8_t> build_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
    std::vector<uint8_t> cmd(8 + payload.size(), 0);
    uint16_t mid_be = htons(msgid);
    std::memcpy(&cmd[0], &mid_be, 2);
    cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
    cmd[5]=(uint8_t)(0x01 + payload.size());
    cmd[6]=cc;
    if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());
    uint16_t n=(uint16_t)cmd.size(); uint8_t cs=0xFF; const uint8_t* p=cmd.data();
    while(n--) cs ^= *p++;
    cmd[7]=cs;
    return cmd;
}

void MakeTCData(const Json::Value& root){
    const Json::Value& m = root.isMember("message") ? root["message"] : root;
    uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
    uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();
    const Json::Value& params = m["parameters"]; // [{name,type,value}, …]

    std::vector<uint8_t> payload;
    if(!pack_from_param_list(params, payload)){
        std::puts("parameter packing failed");
        return;
    }
    auto cmd = build_cmd(msgid, cc, payload);

    // print only (send 미구현)
    dump_hex(cmd.data(), cmd.size());
}