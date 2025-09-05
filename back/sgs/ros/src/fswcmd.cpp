// // ===== fswcmd.cpp 헤더들 (표준 먼저) =====
// #include <cstdint>    // uint8_t, uint16_t, ...
// #include <cstddef>    // size_t
// #include <cstdio>     // std::printf
// #include <cstdlib>    // malloc, free (필요시)
// #include <cstring>    // memset, memcpy
// #include <string>
// #include <vector>
// #include <algorithm>  // std::transform, std::remove_if
// #include <cctype>     // std::tolower
// #include <climits>    // LLONG_MAX, INT32_MIN, INT32_MAX
// #include <arpa/inet.h> // htons, htonl, inet_pton 등 (엔디안/네트워크)
// #include <json/json.h> // Json::Value

// #include <ros/ros.h>


// // SGS_FSWCMD::SGS_FSWCMD(SGS_UDPHandler * udp, uint8_t node, uint8_t port)
// //     : SGS_PING(udp, node, port)
// // {
// //     memset(this->CMD, NULL, sizeof(this->CMD));
// // }

// // SGS_FSWCMD::~SGS_FSWCMD()
// // {
// //     this->ClearAll();
// // }

// // int SGS_FSWCMD::GenerateCMD(uint32_t MsgId, uint16_t FncCode, uint32_t Size, void* Data, sgs_cmd_schinfo_t * schinfo)
// // {
// //     uint32_t Headlen = sizeof(CFE_MSG_CommandHeader);
// //     uint32_t DataSize = Size - Headlen;



// //     CFE_MSG * msg = (CFE_MSG *)malloc(Size);
// //     memset(msg, 0, Size);

// //     this->GenerateCmdHeader(msg, MsgId, FncCode);

// //     if (this->SetSize(msg, Size) != 0)
// //     {
// //         this->callback_err(CMDTYPE_CMD_ERR_ALLOC);
// //         return CMDTYPE_CMD_ERR_ALLOC;
// //     }

// //     if(Data)
// //         this->GenerateCmdData(msg, Data, DataSize);

// //     if(schinfo)
// //         return this->GenerateCmdSchedule(msg, schinfo);
// //     else
// //     {
// //         int idx = this->AllocIdx(Size);
// //         memset(this->CMD[idx], 0, Size);
// //         memcpy(this->CMD[idx], msg, Size);
// //         return idx;
// //     }
// // }


// // ---------- utils ----------
// static inline void dump_hex(const uint8_t* b, size_t n){
//     for(size_t i=0;i<n;++i) std::printf("0x%x ", b[i]);
//     std::printf("\n");
// }
// static inline std::string norm(std::string s){
//     std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
//     s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
//     return s;
// }
// // 문자열/숫자 → u64/s64/f32/f64/bool
// static bool to_u64(const Json::Value& v, uint64_t& out){
//     if(v.isString()){ try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isUInt64()){ out=v.asUInt64(); return true; }
//     if(v.isUInt()){   out=v.asUInt();   return true; }
//     if(v.isInt64()){  auto x=v.asInt64(); if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isInt()){    auto x=v.asInt();  if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isDouble()){ double d=v.asDouble(); if(d<0) return false; out=(uint64_t)d; return true; }
//     return false;
// }
// static bool to_i64(const Json::Value& v, int64_t& out){
//     if(v.isString()){ try{ size_t i=0; long long x=std::stoll(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isInt64()){ out=v.asInt64(); return true; }
//     if(v.isInt()){   out=v.asInt();   return true; }
//     if(v.isUInt64()){ auto x=v.asUInt64(); if(x>(uint64_t)LLONG_MAX) return false; out=(int64_t)x; return true; }
//     if(v.isUInt()){  out=v.asUInt();  return true; }
//     if(v.isDouble()){ out=(int64_t)v.asDouble(); return true; }
//     return false;
// }
// static bool to_f32(const Json::Value& v, float& out){
//     if(v.isString()){ try{ out=std::stof(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=(float)v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(float)v.asInt(); return true; }
//     return false;
// }
// static bool to_f64(const Json::Value& v, double& out){
//     if(v.isString()){ try{ out=std::stod(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(double)v.asInt(); return true; }
//     return false;
// }
// static bool to_bool1(const Json::Value& v, uint8_t& out){
//     if(v.isBool()){ out = v.asBool()?1:0; return true; }
//     if(v.isString()){
//         auto s = norm(v.asString());
//         if(s=="true"||s=="1"||s=="yes"||s=="on") { out=1; return true; }
//         if(s=="false"||s=="0"||s=="no"||s=="off"){ out=0; return true; }
//         // 숫자 문자열도 허용
//         try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
//     }
//     if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
//     return false;
// }

// // ---------- one param → payload append ----------
// // 주의: 페이로드 엔디안은 네 기존 코드처럼 host 그대로 memcpy.
// // 장비가 BE 요구하면 여기서 htons/htonl/수동스왑 적용.
// static bool pack_one_by_type(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload){
//     const std::string t = norm(typeStr);

//     if(t=="u8"||t=="uint8"||t=="byte"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFF) return false;
//         payload.push_back((uint8_t)u); return true;
//     }
//     if(t=="u16"||t=="uint16"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFF) return false;
//         uint16_t w=(uint16_t)u; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="u32"||t=="uint32"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFFFFFFull) return false;
//         uint32_t w=(uint32_t)u; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="u64"||t=="uint64"){
//         uint64_t u; if(!to_u64(value,u)) return false;
//         uint8_t b[8]; std::memcpy(b,&u,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="s8"||t=="int8"){
//         int64_t s; if(!to_i64(value,s)||s<-128||s>127) return false;
//         int8_t w=(int8_t)s; payload.push_back(*(uint8_t*)&w); return true;
//     }
//     if(t=="s16"||t=="int16"){
//         int64_t s; if(!to_i64(value,s)||s<-32768||s>32767) return false;
//         int16_t w=(int16_t)s; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="s32"||t=="int32"){
//         int64_t s; if(!to_i64(value,s)||s<(int64_t)INT32_MIN||s>(int64_t)INT32_MAX) return false;
//         int32_t w=(int32_t)s; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="s64"||t=="int64"){
//         int64_t s; if(!to_i64(value,s)) return false;
//         int64_t w=s; uint8_t b[8]; std::memcpy(b,&w,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="float"||t=="f32"){
//         float f; if(!to_f32(value,f)) return false;
//         uint8_t b[4]; std::memcpy(b,&f,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="double"||t=="f64"){
//         double f; if(!to_f64(value,f)) return false;
//         uint8_t b[8]; std::memcpy(b,&f,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="bool"||t=="b1"||t=="boolean"){
//         uint8_t b; if(!to_bool1(value,b)) return false;
//         payload.push_back(b); return true;
//     }
//     return false; // 미지원 타입 문자열
// }

// static bool pack_from_param_list(const Json::Value& params, std::vector<uint8_t>& payload){
//     if(!params.isArray()) return false;
//     payload.clear();
//     for(const auto& p : params){
//         if(!p.isObject()) return false;
//         const std::string typ = p.get("type","").asString();
//         const Json::Value& val = p["value"];
//         if(!pack_one_by_type(typ, val, payload)) return false;
//     }
//     return true;
// }

// // ---------- build header + checksum & print ----------
// static std::vector<uint8_t> build_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
//     std::vector<uint8_t> cmd(8 + payload.size(), 0);
//     uint16_t mid_be = htons(msgid);
//     std::memcpy(&cmd[0], &mid_be, 2);
//     cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
//     cmd[5]=(uint8_t)(0x01 + payload.size());
//     cmd[6]=cc;
//     if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());
//     uint16_t n=(uint16_t)cmd.size(); uint8_t cs=0xFF; const uint8_t* p=cmd.data();
//     while(n--) cs ^= *p++;
//     cmd[7]=cs;
//     return cmd;
// }

// void MakeTCData(const Json::Value& root){
//     const Json::Value& m = root.isMember("message") ? root["message"] : root;
//     uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
//     uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();
//     const Json::Value& params = m["parameters"]; // [{name,type,value}, …]

//     std::vector<uint8_t> payload;
//     if(!pack_from_param_list(params, payload)){
//         std::puts("parameter packing failed");
//         return;
//     }
//     auto cmd = build_cmd(msgid, cc, payload);

//     // print only (send 미구현)
//     dump_hex(cmd.data(), cmd.size());
// }



// // ===== fswcmd.cpp =====
// // 1) 내부 CMD(8바이트 헤더 + payload + LRC) 생성
// // 2) EncapsulateTC 규격으로 프레임 구성(PREAMBLE/TYPE/LEN + DATA + CRC32 + POSTAMBLE)
// // 3) JSON에서 IP/Port 읽어 TCP 전송

// #include <cstdint>
// #include <cstddef>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <cctype>
// #include <climits>

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// #include <json/json.h>

// // --- 프로젝트에서 이미 정의되어 있으면 이 블록은 무시됨 ---
// #ifndef PREAMBLE
// #define PREAMBLE   0xA1B2C3D4u
// #endif
// #ifndef POSTAMBLE
// #define POSTAMBLE  0x4DC3B2A1u   // 필요 시 프로젝트 값으로 교체
// #endif

// // ---------- utils ----------
// static inline void dump_hex(const uint8_t* b, size_t n){
//     for(size_t i=0;i<n;++i) std::printf("0x%x ", b[i]);
//     std::printf("\n");
// }
// static inline std::string norm(std::string s){
//     std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
//     s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
//     return s;
// }

// // 문자열/숫자 → 정수/실수 캐스팅
// static bool to_u64(const Json::Value& v, uint64_t& out){
//     if(v.isString()){ try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isUInt64()){ out=v.asUInt64(); return true; }
//     if(v.isUInt()){   out=v.asUInt();   return true; }
//     if(v.isInt64()){  auto x=v.asInt64(); if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isInt()){    auto x=v.asInt();  if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isDouble()){ double d=v.asDouble(); if(d<0) return false; out=(uint64_t)d; return true; }
//     return false;
// }
// static bool to_i64(const Json::Value& v, int64_t& out){
//     if(v.isString()){ try{ size_t i=0; long long x=std::stoll(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isInt64()){ out=v.asInt64(); return true; }
//     if(v.isInt()){   out=v.asInt();   return true; }
//     if(v.isUInt64()){ auto x=v.asUInt64(); if(x>(uint64_t)LLONG_MAX) return false; out=(int64_t)x; return true; }
//     if(v.isUInt()){  out=v.asUInt();  return true; }
//     if(v.isDouble()){ out=(int64_t)v.asDouble(); return true; }
//     return false;
// }
// static bool to_f32(const Json::Value& v, float& out){
//     if(v.isString()){ try{ out=std::stof(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=(float)v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(float)v.asInt(); return true; }
//     return false;
// }
// static bool to_f64(const Json::Value& v, double& out){
//     if(v.isString()){ try{ out=std::stod(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(double)v.asInt(); return true; }
//     return false;
// }
// static bool to_bool1(const Json::Value& v, uint8_t& out){
//     if(v.isBool()){ out = v.asBool()?1:0; return true; }
//     if(v.isString()){
//         auto s = norm(v.asString());
//         if(s=="true"||s=="1"||s=="yes"||s=="on") { out=1; return true; }
//         if(s=="false"||s=="0"||s=="no"||s=="off"){ out=0; return true; }
//         try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
//     }
//     if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
//     return false;
// }

// // ---------- (name,type,value) 리스트 → payload ----------
// static bool pack_one_by_type(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload){
//     const std::string t = norm(typeStr);

//     if(t=="u8"||t=="uint8"||t=="byte"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFF) return false;
//         payload.push_back((uint8_t)u); return true;
//     }
//     if(t=="u16"||t=="uint16"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFF) return false;
//         uint16_t w=(uint16_t)u; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="u32"||t=="uint32"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFFFFFFull) return false;
//         uint32_t w=(uint32_t)u; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="u64"||t=="uint64"){
//         uint64_t u; if(!to_u64(value,u)) return false;
//         uint8_t b[8]; std::memcpy(b,&u,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="s8"||t=="int8"){
//         int64_t s; if(!to_i64(value,s)||s<-128||s>127) return false;
//         int8_t w=(int8_t)s; payload.push_back(*(uint8_t*)&w); return true;
//     }
//     if(t=="s16"||t=="int16"){
//         int64_t s; if(!to_i64(value,s)||s<-32768||s>32767) return false;
//         int16_t w=(int16_t)s; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="s32"||t=="int32"){
//         int64_t s; if(!to_i64(value,s)||s<(int64_t)INT32_MIN||s>(int64_t)INT32_MAX) return false;
//         int32_t w=(int32_t)s; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="s64"||t=="int64"){
//         int64_t s; if(!to_i64(value,s)) return false;
//         int64_t w=s; uint8_t b[8]; std::memcpy(b,&w,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="float"||t=="f32"){
//         float f; if(!to_f32(value,f)) return false;
//         uint8_t b[4]; std::memcpy(b,&f,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="double"||t=="f64"){
//         double f; if(!to_f64(value,f)) return false;
//         uint8_t b[8]; std::memcpy(b,&f,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="bool"||t=="b1"||t=="boolean"){
//         uint8_t b; if(!to_bool1(value,b)) return false;
//         payload.push_back(b); return true;
//     }
//     return false;
// }
// static bool pack_from_param_list(const Json::Value& params, std::vector<uint8_t>& payload){
//     if(!params.isArray()) return false;
//     payload.clear();
//     for(const auto& p : params){
//         if(!p.isObject()) return false;
//         const std::string typ = p.get("type","").asString();
//         const Json::Value& val = p["value"];
//         if(!pack_one_by_type(typ, val, payload)) return false;
//     }
//     return true;
// }

// // ---------- 내부 CMD(8+payload) 생성: [0..1]=msgid(BE), [2]=C0, [3]=00, [4]=00, [5]=len(1+payload), [6]=cc, [7]=LRC(FF xor all), [8..]=payload ----------
// static std::vector<uint8_t> build_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
//     std::vector<uint8_t> cmd(8 + payload.size(), 0);

//     // msgid은 big-endian(네 기존 규칙)
//     uint16_t mid_be = htons(msgid);
//     std::memcpy(&cmd[0], &mid_be, 2);

//     cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
//     cmd[5]=(uint8_t)(0x01 + payload.size());
//     cmd[6]=cc;

//     if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());

//     uint16_t n=(uint16_t)cmd.size(); 
//     uint8_t cs=0xFF; 
//     const uint8_t* p=cmd.data();
//     while(n--) cs ^= *p++;
//     cmd[7]=cs; // LRC

//     return cmd;
// }

// // ---------- CRC32 (상대 소스와 동일 동작: reflected poly, init=0xFFFFFFFF, final xor=0xFFFFFFFF) ----------
// static uint32_t crc32_reflected(const uint8_t* data, size_t len) {
//     uint32_t crc = 0xFFFFFFFFu;
//     for (size_t i = 0; i < len; ++i) {
//         crc ^= data[i];
//         for (int k = 0; k < 8; ++k) {
//             crc = (crc >> 1) ^ ((crc & 1u) ? 0xEDB88320u : 0u);
//         }
//     }
//     return ~crc;
// }

// // ---------- EncapsulateTC 호환 프레임 빌드 (host 엔디안 그대로 memcpy!) ----------
// static std::vector<uint8_t> build_tc_frame_compat(const std::vector<uint8_t>& telecmd_data) {
//     const uint32_t PRE  = PREAMBLE;
//     const uint32_t TYPE = 1u;
//     const uint32_t POST = POSTAMBLE;

//     // EncapsulateTC와 동일: MsgLen = sizeof(header 3*U32) + data + CRC(U32) + POST(U32)
//     const uint32_t total_len = (uint32_t)( sizeof(uint32_t)*3 + telecmd_data.size() + sizeof(uint32_t) + sizeof(uint32_t) );

//     std::vector<uint8_t> out;
//     out.reserve(total_len);

//     auto put_u32_host = [&](uint32_t v){
//         size_t off = out.size();
//         out.resize(off + 4);
//         std::memcpy(&out[off], &v, 4); // host 엔디안 그대로
//     };

//     // Header
//     put_u32_host(PRE);
//     put_u32_host(TYPE);
//     put_u32_host(total_len);

//     // Data
//     if (!telecmd_data.empty()) {
//         size_t off = out.size();
//         out.resize(off + telecmd_data.size());
//         std::memcpy(&out[off], telecmd_data.data(), telecmd_data.size());
//     }

//     // CRC32 over (Header + Data)
//     const uint32_t crc = crc32_reflected(out.data(), out.size());
//     put_u32_host(crc);

//     // Postamble
//     put_u32_host(POST);

//     return out;
// }

// // ---------- TCP 전송 ----------
// static bool send_tcp_bytes(const std::string& ip, uint16_t port, const uint8_t* buf, size_t len){
//     int fd = ::socket(AF_INET, SOCK_STREAM, 0);
//     if (fd < 0) { std::perror("socket"); return false; }

//     sockaddr_in sa{};
//     sa.sin_family = AF_INET;
//     sa.sin_port = htons(port);
//     if (::inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1) { std::perror("inet_pton"); ::close(fd); return false; }

//     if (::connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0) { std::perror("connect"); ::close(fd); return false; }

//     size_t sent = 0;
//     while (sent < len) {
//         ssize_t n = ::send(fd, buf + sent, len - sent, 0);
//         if (n <= 0) { std::perror("send"); ::close(fd); return false; }
//         sent += (size_t)n;
//     }

//     ::close(fd);
//     return true;
// }

// // ---------- 공개 함수 ----------
// void MakeTCData(const Json::Value& root){
//     // data 또는 data.message 둘 다 지원
//     const Json::Value& m = root.isMember("message") ? root["message"] : root;

//     // 내부 CMD 생성에 필요한 필드
//     const uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
//     const uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();

//     // 파라미터 페이로드 생성
//     const Json::Value& params = m["parameters"]; // [{name,type,value}, …]
//     std::vector<uint8_t> payload;
//     if(!pack_from_param_list(params, payload)){
//         std::puts("[MakeTCData] parameter packing failed");
//         return;
//     }

//     // 1) 내부 CMD (8바이트 헤더 + payload + LRC)
//     const auto inner_cmd = build_cmd(msgid, cc, payload);

//     // 2) EncapsulateTC 규격 프레임
//     const auto frame = build_tc_frame_compat(inner_cmd);

//     // 3) 디버그 출력
//     std::printf("[INNER CMD] %zu bytes:\n", inner_cmd.size());
//     dump_hex(inner_cmd.data(), inner_cmd.size());
//     std::printf("[TC FRAME ] %zu bytes:\n", frame.size());
//     dump_hex(frame.data(), frame.size());

//     // 4) JSON에서 IP/Port 읽어 전송
//     std::string ip;
//     int port = 0;
//     if (m.isMember("IP"))   ip = m["IP"].asString(); else if (m.isMember("ip")) ip = m["ip"].asString();
//     if (m.isMember("Port")) port = m["Port"].asInt(); else if (m.isMember("port")) port = m["port"].asInt();

//     if (!ip.empty() && port > 0) {
//         const bool ok = send_tcp_bytes(ip, (uint16_t)port, frame.data(), frame.size());
//         std::printf("[SEND] %s:%d -> %zu bytes %s\n", ip.c_str(), port, frame.size(), ok ? "OK" : "FAIL");
//     } else {
//         std::puts("[SEND] IP/Port not provided. Skipped sending.");
//     }
// // }
// #include <cstdint>
// #include <cstdio>
// #include <cstring>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <cctype>
// #include <climits>

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// #include <json/json.h>

// // ========== Expedite 상수 ==========
// static constexpr uint32_t EXP_PRE      = 0xA1B2C3D4u;     // Preamble
// static constexpr uint32_t EXP_POST     = ~0xA1B2C3D4u;    // Postamble (= ~Preamble)
// static constexpr uint32_t EXP_TYPE_TC  = 0x01u;           // Message Type: Telecommand

// // ========== Expedite 헤더(메뉴얼 구조체 그대로) ==========
// typedef struct __attribute__((packed)) _TC_HEADER {
//     uint32_t nPreamble;   // Packet preamble
//     uint32_t nMsglen;     // Total packet length
//     uint32_t nMsgType;    // Message type
// } TC_HEADER;

// // ========== 유틸 ==========
// static inline std::string norm(std::string s){
//     std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
//     s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
//     return s;
// }
// static inline void dump_hex(const uint8_t* b, size_t n){
//     for(size_t i=0;i<n;++i) std::printf("0x%02X ", b[i]); std::printf("\n");
// }

// // JSON → 숫자 변환들
// static bool to_u64(const Json::Value& v, uint64_t& out){
//     if(v.isString()){ try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isUInt64()){ out=v.asUInt64(); return true; }
//     if(v.isUInt()){   out=v.asUInt();   return true; }
//     if(v.isInt64()){  auto x=v.asInt64(); if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isInt()){    auto x=v.asInt();  if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isDouble()){ double d=v.asDouble(); if(d<0) return false; out=(uint64_t)d; return true; }
//     return false;
// }
// static bool to_i64(const Json::Value& v, int64_t& out){
//     if(v.isString()){ try{ size_t i=0; long long x=std::stoll(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isInt64()){ out=v.asInt64(); return true; }
//     if(v.isInt()){   out=v.asInt();   return true; }
//     if(v.isUInt64()){ auto x=v.asUInt64(); if(x>(uint64_t)LLONG_MAX) return false; out=(int64_t)x; return true; }
//     if(v.isUInt()){  out=v.asUInt();  return true; }
//     if(v.isDouble()){ out=(int64_t)v.asDouble(); return true; }
//     return false;
// }
// static bool to_f32(const Json::Value& v, float& out){
//     if(v.isString()){ try{ out=std::stof(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=(float)v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(float)v.asInt(); return true; }
//     return false;
// }
// static bool to_f64(const Json::Value& v, double& out){
//     if(v.isString()){ try{ out=std::stod(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(double)v.asInt(); return true; }
//     return false;
// }
// static bool to_bool1(const Json::Value& v, uint8_t& out){
//     if(v.isBool()){ out = v.asBool()?1:0; return true; }
//     if(v.isString()){
//         auto s = norm(v.asString());
//         if(s=="true"||s=="1"||s=="yes"||s=="on") { out=1; return true; }
//         if(s=="false"||s=="0"||s=="no"||s=="off"){ out=0; return true; }
//         try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
//     }
//     if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
//     return false;
// }

// // 파라미터 → payload packing
// static bool pack_one_by_type(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload){
//     const std::string t = norm(typeStr);

//     if(t=="u8"||t=="uint8"||t=="byte"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFF) return false;
//         payload.push_back((uint8_t)u); return true;
//     }
//     if(t=="u16"||t=="uint16"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFF) return false;
//         uint16_t w=(uint16_t)u; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="u32"||t=="uint32"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFFFFFFull) return false;
//         uint32_t w=(uint32_t)u; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="u64"||t=="uint64"){
//         uint64_t u; if(!to_u64(value,u)) return false;
//         uint8_t b[8]; std::memcpy(b,&u,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="s8"||t=="int8"){
//         int64_t s; if(!to_i64(value,s)||s<-128||s>127) return false;
//         int8_t w=(int8_t)s; payload.push_back(*(uint8_t*)&w); return true;
//     }
//     if(t=="s16"||t=="int16"){
//         int64_t s; if(!to_i64(value,s)||s<-32768||s>32767) return false;
//         int16_t w=(int16_t)s; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="s32"||t=="int32"){
//         int64_t s; if(!to_i64(value,s)||s<(int64_t)INT32_MIN||s>(int64_t)INT32_MAX) return false;
//         int32_t w=(int32_t)s; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="s64"||t=="int64"){
//         int64_t s; if(!to_i64(value,s)) return false;
//         int64_t w=s; uint8_t b[8]; std::memcpy(b,&w,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="float"||t=="f32"){
//         float f; if(!to_f32(value,f)) return false;
//         uint8_t b[4]; std::memcpy(b,&f,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="double"||t=="f64"){
//         double f; if(!to_f64(value,f)) return false;
//         uint8_t b[8]; std::memcpy(b,&f,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="bool"||t=="b1"||t=="boolean"){
//         uint8_t b; if(!to_bool1(value,b)) return false;
//         payload.push_back(b); return true;
//     }
//     return false;
// }
// static bool pack_from_param_list(const Json::Value& params, std::vector<uint8_t>& payload){
//     if(!params.isArray()) return false;
//     payload.clear();
//     for(const auto& p : params){
//         if(!p.isObject()) return false;
//         const std::string typ = p.get("type","").asString();
//         const Json::Value& val = p["value"];
//         if(!pack_one_by_type(typ, val, payload)) return false;
//     }
//     return true;
// }

// // 내부 Telecommand (네 CCSDS 포맷) 만들기
// // [0..1]=msgid(BE), [2]=0xC0, [3]=0x00, [4]=0x00, [5]=(1+payload), [6]=cc, [7]=LRC(0xFF xor all), [8..]=payload
// static std::vector<uint8_t> build_inner_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
//     std::vector<uint8_t> cmd(8 + payload.size(), 0);
//     uint16_t mid_be = htons(msgid);
//     std::memcpy(&cmd[0], &mid_be, 2);
//     cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
//     cmd[5]=(uint8_t)(0x01 + payload.size());
//     cmd[6]=cc;
//     if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());
//     uint16_t n=(uint16_t)cmd.size(); uint8_t cs=0xFF; const uint8_t* p=cmd.data();
//     while(n--) cs ^= *p++;
//     cmd[7]=cs;
//     return cmd;
// }

// // CSP 32-bit 헤더 (연구실 스택과 동일 비트배치, LE 저장)
// // LSB→MSB: flags(8) | sport(6) | dport(6) | dst(5) | src(5) | pri(2)
// static inline uint32_t build_csp_id_le(uint8_t pri, uint8_t src, uint8_t dst, uint8_t sport, uint8_t dport, uint8_t flags){
//     uint32_t id = 0;
//     id |= ((uint32_t)flags) & 0xFFu;                 // bits 0..7
//     id |= ((uint32_t)(sport & 0x3F)) << 8;           // bits 8..13
//     id |= ((uint32_t)(dport & 0x3F)) << 14;          // bits 14..19
//     id |= ((uint32_t)(dst   & 0x1F)) << 20;          // bits 20..24
//     id |= ((uint32_t)(src   & 0x1F)) << 25;          // bits 25..29
//     id |= ((uint32_t)(pri   & 0x03)) << 30;          // bits 30..31
//     return id;
// }
// static inline void put_u32_le(std::vector<uint8_t>& out, uint32_t v){
//     out.push_back((uint8_t)(v      & 0xFF));
//     out.push_back((uint8_t)((v>>8) & 0xFF));
//     out.push_back((uint8_t)((v>>16)& 0xFF));
//     out.push_back((uint8_t)((v>>24)& 0xFF));
// }


// // BE로 패킹
// static inline uint32_t build_csp_id_be(uint8_t pri, uint8_t src, uint8_t dst,
//                                        uint8_t sport, uint8_t dport, uint8_t flags){
//     uint32_t ext =
//         ((uint32_t)(pri   & 0x03) << 30) |
//         ((uint32_t)(src   & 0x1F) << 25) |
//         ((uint32_t)(dst   & 0x1F) << 20) |
//         ((uint32_t)(dport & 0x3F) << 14) |
//         ((uint32_t)(sport & 0x3F) <<  8) |
//         ((uint32_t)(flags & 0xFF)      );
//     return htonl(ext);
// }

// static inline void put_u32_be(std::vector<uint8_t>& out, uint32_t v){
//     out.push_back((uint8_t)((v >> 24) & 0xFF));
//     out.push_back((uint8_t)((v >> 16) & 0xFF));
//     out.push_back((uint8_t)((v >>  8) & 0xFF));
//     out.push_back((uint8_t)( v        & 0xFF));
// }


// // CRC32 (reflected) — 메뉴얼의 GetCRC32 동등 구현
// static uint32_t crc32_reflected(const uint8_t* data, size_t len){
//     uint32_t crc = 0xFFFFFFFFu;
//     for(size_t i=0;i<len;++i){
//         crc ^= data[i];
//         for(int k=0;k<8;++k) crc = (crc >> 1) ^ ((crc & 1u)? 0xEDB88320u : 0u);
//     }
//     return ~crc;
// }

// // Expedite 캡슐화 (메뉴얼 EncapsulateTC 로직 동일 동작)
// static std::vector<uint8_t> encapsulate_expedite_tc(const std::vector<uint8_t>& telecmd_data){
//     // 총길이 = 헤더(12) + 데이터 + CRC(4) + POST(4)
//     const uint32_t nMsglen = (uint32_t)(sizeof(TC_HEADER) + telecmd_data.size() + 2 * sizeof(uint32_t));

//     // 1) 헤더 LE 직렬화
//     std::vector<uint8_t> out; out.reserve(nMsglen);
//     TC_HEADER hdr{};
//     hdr.nPreamble = EXP_PRE;
//     hdr.nMsgType  = EXP_TYPE_TC;
//     hdr.nMsglen   = nMsglen;
//     out.resize(out.size() + sizeof(TC_HEADER));
//     std::memcpy(&out[out.size() - sizeof(TC_HEADER)], &hdr, sizeof(TC_HEADER));

//     // 2) 데이터
//     out.insert(out.end(), telecmd_data.begin(), telecmd_data.end());

//     // 3) CRC(헤더+데이터)
//     uint32_t crc = crc32_reflected(out.data(), out.size());
//     out.resize(out.size() + sizeof(crc));
//     std::memcpy(&out[out.size() - sizeof(crc)], &crc, sizeof(crc));

//     // 4) Postamble
//     uint32_t post = EXP_POST;
//     out.resize(out.size() + sizeof(post));
//     std::memcpy(&out[out.size() - sizeof(post)], &post, sizeof(post));
//     return out;
// }

// // TCP 전송
// static bool send_tcp_bytes(const std::string& ip, uint16_t port, const uint8_t* buf, size_t len){
//     int fd = ::socket(AF_INET, SOCK_STREAM, 0);
//     if(fd < 0){ std::perror("socket"); return false; }
//     sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_port = htons(port);
//     if(::inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1){ std::perror("inet_pton"); ::close(fd); return false; }
//     if(::connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0){ std::perror("connect"); ::close(fd); return false; }
//     size_t sent=0;
//     while(sent < len){
//         ssize_t n = ::send(fd, buf+sent, len-sent, 0);
//         if(n <= 0){ std::perror("send"); ::close(fd); return false; }
//         sent += (size_t)n;
//     }
//     ::close(fd);
//     return true;
// }

// // ======================== 공개 함수 ========================
// // JSON → (CCSDS+ARG) → 앞에 CSP 4B → Expedite 캡슐화 → TCP 전송
// void MakeTCData(const Json::Value& root){
//     const Json::Value& m = root.isMember("message") ? root["message"] : root;

//     // 1) payload 생성
//     const uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
//     const uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();

//     std::vector<uint8_t> payload;
//     if(!pack_from_param_list(m["parameters"], payload)){
//         std::puts("[MakeTCData] parameter packing failed");
//         return;
//     }

//     // 2) 내부(CCSDS+arg) 생성
//     const auto inner_cmd = build_inner_cmd(msgid, cc, payload);

//     // 3) CSP 4B (연구실 스택과 동일 비트배치) — 기본값도 허용
//     uint8_t pri   = (uint8_t)m["csp"].get("pri",   1).asUInt();
//     uint8_t src   = (uint8_t)m["csp"].get("src",  20).asUInt();
//     uint8_t dst   = (uint8_t)m["csp"].get("dst",   3).asUInt();
//     uint8_t sport = (uint8_t)m["csp"].get("sport",10).asUInt();
//     uint8_t dport = (uint8_t)m["csp"].get("dport",13).asUInt();
//     uint8_t flags = (uint8_t)m["csp"].get("flags", 0).asUInt();

//     const uint32_t csp_id = build_csp_id_be(pri, src, dst, sport, dport, flags);

//     std::vector<uint8_t> tc_data;
//     tc_data.reserve(4 + inner_cmd.size());
//     // CSP ID(BE) 추가
//     put_u32_be(tc_data, csp_id);
//     // 내부 CMD 붙임
//     tc_data.insert(tc_data.end(), inner_cmd.begin(), inner_cmd.end());

//     // 4) Expedite 캡슐화
//     const auto frame = encapsulate_expedite_tc(tc_data);

//     // 5) TCP 전송
//     std::string ip; int port = 0;
//     if(m.isMember("IP"))   ip = m["IP"].asString();   else if(m.isMember("ip"))   ip = m["ip"].asString();
//     if(m.isMember("Port")) port = m["Port"].asInt();  else if(m.isMember("port")) port = m["port"].asInt();

//     if(!ip.empty() && port > 0){
//         bool ok = send_tcp_bytes(ip, (uint16_t)port, frame.data(), frame.size());
//         std::printf("[SEND] %s:%d -> %zu bytes %s\n", ip.c_str(), port, frame.size(), ok?"OK":"FAIL");
//     }else{
//         std::puts("[SEND] IP/Port not provided. Skipped sending.");
//     }

//     // debug
//     std::printf("[INNER CMD] %zu bytes\n", inner_cmd.size());  dump_hex(inner_cmd.data(), inner_cmd.size());
//     std::printf("[TC DATA  ] %zu bytes (CSP4 + INNER)\n", tc_data.size()); dump_hex(tc_data.data(), tc_data.size());
//     std::printf("[EXP.FRAME] %zu bytes\n", frame.size());
// }


// // fswcmd.cpp
// // JSON → (CCSDS+ARG) → CSP 헤더 4B(BE) → (옵션) CSP CRC32 tail(LE) → Expedite 캡슐화 → TCP 전송

// #include <cstdint>
// #include <cstdio>
// #include <cstring>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <cctype>
// #include <climits>

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>

// #include <json/json.h>

// // ======================= Expedite 상수 =======================
// static constexpr uint32_t EXP_PRE      = 0xA1B2C3D4u;     // Preamble  (LE 직렬화 가정)
// static constexpr uint32_t EXP_POST     = ~0xA1B2C3D4u;    // Postamble (= ~Preamble)
// static constexpr uint32_t EXP_TYPE_TC  = 0x01u;           // Message Type: Telecommand

// // =================== Expedite 헤더(메뉴얼 구조체) ===================
// typedef struct __attribute__((packed)) _TC_HEADER {
//     uint32_t nPreamble;   // Packet preamble
//     uint32_t nMsglen;     // Total packet length
//     uint32_t nMsgType;    // Message type
// } TC_HEADER;

// // ========================= 유틸 =========================
// static inline std::string norm(std::string s){
//     std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)std::tolower(c); });
//     s.erase(std::remove_if(s.begin(), s.end(), [](char c){ return c==' ' || c=='_'; }), s.end());
//     return s;
// }

// static inline void dump_hex(const uint8_t* b, size_t n){
//     for(size_t i=0;i<n;++i){
//         std::printf("%02X%s", b[i], ((i+1)%16==0 || i+1==n) ? "\n" : " ");
//     }
// }

// // JSON → 숫자 변환들
// static bool to_u64(const Json::Value& v, uint64_t& out){
//     if(v.isString()){ try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isUInt64()){ out=v.asUInt64(); return true; }
//     if(v.isUInt()){   out=v.asUInt();   return true; }
//     if(v.isInt64()){  auto x=v.asInt64(); if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isInt()){    auto x=v.asInt();  if(x<0) return false; out=(uint64_t)x; return true; }
//     if(v.isDouble()){ double d=v.asDouble(); if(d<0) return false; out=(uint64_t)d; return true; }
//     return false;
// }
// static bool to_i64(const Json::Value& v, int64_t& out){
//     if(v.isString()){ try{ size_t i=0; long long x=std::stoll(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out=x; return true; }catch(...){ return false; } }
//     if(v.isInt64()){ out=v.asInt64(); return true; }
//     if(v.isInt()){   out=v.asInt();   return true; }
//     if(v.isUInt64()){ auto x=v.asUInt64(); if(x>(uint64_t)LLONG_MAX) return false; out=(int64_t)x; return true; }
//     if(v.isUInt()){  out=v.asUInt();  return true; }
//     if(v.isDouble()){ out=(int64_t)v.asDouble(); return true; }
//     return false;
// }
// static bool to_f32(const Json::Value& v, float& out){
//     if(v.isString()){ try{ out=std::stof(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=(float)v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(float)v.asInt(); return true; }
//     return false;
// }
// static bool to_f64(const Json::Value& v, double& out){
//     if(v.isString()){ try{ out=std::stod(v.asString()); }catch(...){ return false; } return true; }
//     if(v.isDouble()){ out=v.asDouble(); return true; }
//     if(v.isInt()||v.isUInt()){ out=(double)v.asInt(); return true; }
//     return false;
// }
// static bool to_bool1(const Json::Value& v, uint8_t& out){
//     if(v.isBool()){ out = v.asBool()?1:0; return true; }
//     if(v.isString()){
//         auto s = norm(v.asString());
//         if(s=="true"||s=="1"||s=="yes"||s=="on") { out=1; return true; }
//         if(s=="false"||s=="0"||s=="no"||s=="off"){ out=0; return true; }
//         try{ size_t i=0; unsigned long long x=std::stoull(v.asString(), &i, 0); if(i!=v.asString().size()) return false; out = (x!=0); return true; }catch(...){ return false; }
//     }
//     if(v.isInt()||v.isUInt()){ out = (v.asInt()!=0); return true; }
//     return false;
// }

// // ========================= Payload pack =========================
// static bool pack_one_by_type(const std::string& typeStr, const Json::Value& value, std::vector<uint8_t>& payload){
//     const std::string t = norm(typeStr);

//     if(t=="u8"||t=="uint8"||t=="byte"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFF) return false;
//         payload.push_back((uint8_t)u); return true;
//     }
//     if(t=="u16"||t=="uint16"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFF) return false;
//         uint16_t w=(uint16_t)u; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="u32"||t=="uint32"){
//         uint64_t u; if(!to_u64(value,u)||u>0xFFFFFFFFull) return false;
//         uint32_t w=(uint32_t)u; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="u64"||t=="uint64"){
//         uint64_t u; if(!to_u64(value,u)) return false;
//         uint8_t b[8]; std::memcpy(b,&u,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="s8"||t=="int8"){
//         int64_t s; if(!to_i64(value,s)||s<-128||s>127) return false;
//         int8_t w=(int8_t)s; payload.push_back(*(uint8_t*)&w); return true;
//     }
//     if(t=="s16"||t=="int16"){
//         int64_t s; if(!to_i64(value,s)||s<-32768||s>32767) return false;
//         int16_t w=(int16_t)s; uint8_t b[2]; std::memcpy(b,&w,2); payload.insert(payload.end(), b,b+2); return true;
//     }
//     if(t=="s32"||t=="int32"){
//         int64_t s; if(!to_i64(value,s)||s<(int64_t)INT32_MIN||s>(int64_t)INT32_MAX) return false;
//         int32_t w=(int32_t)s; uint8_t b[4]; std::memcpy(b,&w,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="s64"||t=="int64"){
//         int64_t s; if(!to_i64(value,s)) return false;
//         int64_t w=s; uint8_t b[8]; std::memcpy(b,&w,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="float"||t=="f32"){
//         float f; if(!to_f32(value,f)) return false;
//         uint8_t b[4]; std::memcpy(b,&f,4); payload.insert(payload.end(), b,b+4); return true;
//     }
//     if(t=="double"||t=="f64"){
//         double f; if(!to_f64(value,f)) return false;
//         uint8_t b[8]; std::memcpy(b,&f,8); payload.insert(payload.end(), b,b+8); return true;
//     }
//     if(t=="bool"||t=="b1"||t=="boolean"){
//         uint8_t b; if(!to_bool1(value,b)) return false;
//         payload.push_back(b); return true;
//     }
//     return false;
// }
// static bool pack_from_param_list(const Json::Value& params, std::vector<uint8_t>& payload){
//     if(!params.isArray()) return false;
//     payload.clear();
//     for(const auto& p : params){
//         if(!p.isObject()) return false;
//         const std::string typ = p.get("type","").asString();
//         const Json::Value& val = p["value"];
//         if(!pack_one_by_type(typ, val, payload)) return false;
//     }
//     return true;
// }

// // ============ 내부 Telecommand (네 CCSDS 포맷) ============
// // [0..1]=msgid(BE), [2]=0xC0, [3]=0x00, [4]=0x00, [5]=(1+payload), [6]=cc, [7]=LRC(0xFF xor all), [8..]=payload
// static std::vector<uint8_t> build_inner_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
//     std::vector<uint8_t> cmd(8 + payload.size(), 0);
//     uint16_t mid_be = htons(msgid);
//     std::memcpy(&cmd[0], &mid_be, 2);
//     cmd[2]=0xC0; cmd[3]=0x00; cmd[4]=0x00;
//     cmd[5]=(uint8_t)(0x01 + payload.size());
//     cmd[6]=cc;
//     if(!payload.empty()) std::memcpy(&cmd[8], payload.data(), payload.size());
//     uint16_t n=(uint16_t)cmd.size(); uint8_t cs=0xFF; const uint8_t* p=cmd.data();
//     while(n--) cs ^= *p++;
//     cmd[7]=cs;
//     return cmd;
// }

// // ============ CSP v1 32-bit 헤더 pack (네트워크 바이트순, BE) ============
// static inline uint32_t build_csp_id_be(uint8_t pri, uint8_t src, uint8_t dst,
//                                        uint8_t sport, uint8_t dport, uint8_t flags){
//     uint32_t ext =
//         ((uint32_t)(pri   & 0x03) << 30) |
//         ((uint32_t)(src   & 0x1F) << 25) |
//         ((uint32_t)(dst   & 0x1F) << 20) |
//         ((uint32_t)(dport & 0x3F) << 14) |
//         ((uint32_t)(sport & 0x3F) <<  8) |
//         ((uint32_t)(flags & 0xFF)      );
//     // 온와이어는 BE
//     return htonl(ext);
// }

// static inline void put_u32_be(std::vector<uint8_t>& out, uint32_t v){
//     out.push_back((uint8_t)((v >> 24) & 0xFF));
//     out.push_back((uint8_t)((v >> 16) & 0xFF));
//     out.push_back((uint8_t)((v >>  8) & 0xFF));
//     out.push_back((uint8_t)( v        & 0xFF));
// }

// // ============ CRC32 (reflected / IEEE 802.3) ============
// static uint32_t crc32_reflected(const uint8_t* data, size_t len){
//     uint32_t crc = 0xFFFFFFFFu;
//     for(size_t i=0;i<len;++i){
//         crc ^= data[i];
//         for(int k=0;k<8;++k)
//             crc = (crc >> 1) ^ ((crc & 1u)? 0xEDB88320u : 0u);
//     }
//     return ~crc;
// }

// // ============ CSP CRC tail (옵션) 붙이기 ============
// // flags LSB(0x01)가 켜져 있으면 헤더(4B)+INNER 전체에 대해 CRC32를 계산, 결과 4바이트를 **LE**로 붙인다.
// static inline void maybe_append_csp_crc32_tail(std::vector<uint8_t>& tc_data, uint8_t flags){
//     if ((flags & 0x01u) == 0) return; // CSP CRC 사용 안 함
//     const uint32_t ccrc = crc32_reflected(tc_data.data(), tc_data.size());
//     // tail은 LE로 부착 (KISS 캡처와 일치)
//     tc_data.push_back((uint8_t)( ccrc        & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >>  8) & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >> 16) & 0xFF));
//     tc_data.push_back((uint8_t)((ccrc >> 24) & 0xFF));
// }

// // ============ Expedite 캡슐화 ============
// static std::vector<uint8_t> encapsulate_expedite_tc(const std::vector<uint8_t>& telecmd_data){
//     // 총길이 = 헤더(12) + 데이터 + CRC(4) + POST(4)  ← 메뉴얼과 동일
//     const uint32_t nMsglen = (uint32_t)(sizeof(TC_HEADER) + telecmd_data.size() + 2 * sizeof(uint32_t));

//     std::vector<uint8_t> out; out.reserve(nMsglen);

//     // 1) 헤더 (LE 직렬화 가정: struct 그대로 memcpy)
//     TC_HEADER hdr{};
//     hdr.nPreamble = EXP_PRE;
//     hdr.nMsgType  = EXP_TYPE_TC;
//     hdr.nMsglen   = nMsglen;
//     size_t off = out.size();
//     out.resize(off + sizeof(TC_HEADER));
//     std::memcpy(&out[off], &hdr, sizeof(TC_HEADER));

//     // 2) 데이터
//     out.insert(out.end(), telecmd_data.begin(), telecmd_data.end());

//     // 3) CRC(헤더+데이터)
//     const uint32_t crc = crc32_reflected(out.data(), out.size());
//     off = out.size(); out.resize(off + sizeof(crc));
//     std::memcpy(&out[off], &crc, sizeof(crc)); // LE

//     // 4) Postamble (LE)
//     const uint32_t post = EXP_POST;
//     off = out.size(); out.resize(off + sizeof(post));
//     std::memcpy(&out[off], &post, sizeof(post));

//     return out;
// }

// // ============ TCP 전송 ============
// static bool send_tcp_bytes(const std::string& ip, uint16_t port, const uint8_t* buf, size_t len){
//     int fd = ::socket(AF_INET, SOCK_STREAM, 0);
//     if(fd < 0){ std::perror("socket"); return false; }
//     sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_port = htons(port);
//     if(::inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) != 1){ std::perror("inet_pton"); ::close(fd); return false; }
//     if(::connect(fd, (sockaddr*)&sa, sizeof(sa)) < 0){ std::perror("connect"); ::close(fd); return false; }
//     size_t sent=0;
//     while(sent < len){
//         ssize_t n = ::send(fd, buf+sent, len-sent, 0);
//         if(n <= 0){ std::perror("send"); ::close(fd); return false; }
//         sent += (size_t)n;
//     }
//     ::close(fd);
//     return true;
// }

// // ======================== 공개 함수 ========================
// // JSON → (CCSDS+ARG) → 앞에 CSP 4B(BE) → (옵션) CSP CRC32 tail(LE) → Expedite 캡슐화 → TCP 전송
// void MakeTCData(const Json::Value& root){
//     const Json::Value& m = root.isMember("message") ? root["message"] : root;

//     // 1) payload 생성
//     const uint16_t msgid = (uint16_t)m.get("msgid",0).asUInt();
//     const uint8_t  cc    = (uint8_t )m.get("cc",0).asUInt();

//     std::vector<uint8_t> payload;
//     if(!pack_from_param_list(m["parameters"], payload)){
//         std::puts("[MakeTCData] parameter packing failed");
//         return;
//     }

//     // 2) 내부(CCSDS+arg) 생성
//     const auto inner_cmd = build_inner_cmd(msgid, cc, payload);

//     // 3) CSP 4B 입력값
//     uint8_t pri   = (uint8_t)m["csp"].get("pri",   1).asUInt();  // P1=HIGH
//     uint8_t src   = (uint8_t)m["csp"].get("src",  28).asUInt();
//     uint8_t dst   = (uint8_t)m["csp"].get("dst",   3).asUInt();
//     uint8_t sport = (uint8_t)m["csp"].get("sport",10).asUInt();  // 필요 시 내부에서 에페메랄 증가 구현 가능
//     uint8_t dport = (uint8_t)m["csp"].get("dport",13).asUInt();
//     uint8_t flags = (uint8_t)m["csp"].get("flags", 1).asUInt();  // 기본 1: CSP CRC32 tail 사용

//     // 3-1) CSP 헤더 4B(BE) 생성
//     const uint32_t csp_id_be = build_csp_id_be(pri, src, dst, sport, dport, flags);

//     // 3-2) TC 데이터 = CSP4(BE) + INNER
//     std::vector<uint8_t> tc_data;
//     tc_data.reserve(4 + inner_cmd.size() + 4 /*crc opt*/);
//     // CSP ID(BE) push
//     put_u32_be(tc_data, csp_id_be);
//     // INNER CMD append
//     tc_data.insert(tc_data.end(), inner_cmd.begin(), inner_cmd.end());

//     // 3-3) (옵션) CSP CRC32 tail(LE)
//     maybe_append_csp_crc32_tail(tc_data, flags);

//     // 4) Expedite 캡슐화
//     const auto frame = encapsulate_expedite_tc(tc_data);

//     // 5) TCP 전송
//     std::string ip; int port = 0;
//     if(m.isMember("IP"))   ip = m["IP"].asString();   else if(m.isMember("ip"))   ip = m["ip"].asString();
//     if(m.isMember("Port")) port = m["Port"].asInt();  else if(m.isMember("port")) port = m["port"].asInt();

//     if(!ip.empty() && port > 0){
//         bool ok = send_tcp_bytes(ip, (uint16_t)port, frame.data(), frame.size());
//         std::printf("[SEND] %s:%d -> %zu bytes %s\n", ip.c_str(), port, frame.size(), ok?"OK":"FAIL");
//     }else{
//         std::puts("[SEND] IP/Port not provided. Skipped sending.");
//     }

//     // ===== debug =====
//     std::printf("[CSP-HDR(BE)] ");
//     uint8_t hdr4[4] = {
//         (uint8_t)((csp_id_be>>24)&0xFF),
//         (uint8_t)((csp_id_be>>16)&0xFF),
//         (uint8_t)((csp_id_be>> 8)&0xFF),
//         (uint8_t)((csp_id_be>> 0)&0xFF),
//     };
//     dump_hex(hdr4, 4);

//     std::printf("[INNER CMD] %zu bytes\n", inner_cmd.size());
//     dump_hex(inner_cmd.data(), inner_cmd.size());

//     std::printf("[TC DATA  ] %zu bytes (CSP4 + INNER%s)\n",
//                 tc_data.size(), (flags&0x01) ? " + CRC32" : "");
//     dump_hex(tc_data.data(), tc_data.size());

//     std::printf("[EXP.FRAME] %zu bytes\n", frame.size());
// }


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

// ============ 내부 Telecommand (CCSDS 스타일) ============
static std::vector<uint8_t> build_inner_cmd(uint16_t msgid, uint8_t cc, const std::vector<uint8_t>& payload){
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

// ============ CSP v1 32-bit 헤더 pack (온와이어 BE) ============
static inline uint32_t build_csp_id_be(uint8_t pri, uint8_t src, uint8_t dst,
                                       uint8_t sport, uint8_t dport, uint8_t flags){
    uint32_t ext =
        ((uint32_t)(pri   & 0x03) << 30) |
        ((uint32_t)(src   & 0x1F) << 25) |
        ((uint32_t)(dst   & 0x1F) << 20) |
        ((uint32_t)(dport & 0x3F) << 14) |
        ((uint32_t)(sport & 0x3F) <<  8) |
        ((uint32_t)(flags & 0xFF)      );
    return ext; // put_u32_be가 BE로 씀
}
static inline void put_u32_be(std::vector<uint8_t>& out, uint32_t v){
    out.push_back((uint8_t)((v >> 24) & 0xFF));
    out.push_back((uint8_t)((v >> 16) & 0xFF));
    out.push_back((uint8_t)((v >>  8) & 0xFF));
    out.push_back((uint8_t)( v        & 0xFF));
}

// ============ CRC32 (reflected / IEEE 802.3) ============
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

// ============ CSP CRC tail (옵션) 붙이기 ============
static inline void maybe_append_csp_crc32_tail(std::vector<uint8_t>& tc_data, uint8_t flags){
    if ((flags & 0x01u) == 0) return; // CSP CRC 사용 안 함
    const uint32_t ccrc = GetCRC32(tc_data);          // 헤더(4B)+INNER
    // tail은 LE
    tc_data.push_back((uint8_t)( ccrc        & 0xFF));
    tc_data.push_back((uint8_t)((ccrc >>  8) & 0xFF));
    tc_data.push_back((uint8_t)((ccrc >> 16) & 0xFF));
    tc_data.push_back((uint8_t)((ccrc >> 24) & 0xFF));
}

// ============ Expedite 캡슐화 (제조사 방식) ============
static std::vector<uint8_t> EncapsulateTC(const std::vector<uint8_t>& pnData){
    const uint32_t nMsglen = (uint32_t)(sizeof(TC_HEADER) + pnData.size() + 2 * sizeof(uint32_t));
    std::vector<uint8_t> pnOut; pnOut.clear();

    // 헤더(struct 그대로 memcpy) — x86/리눅스에서는 LE 직렬화
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
    uint8_t flags = (uint8_t)m["csp"].get("flags", 1).asUInt();  // 1: CSP CRC32 tail 사용

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
    // maybe_append_csp_crc32_tail(tc_data, flags);

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
