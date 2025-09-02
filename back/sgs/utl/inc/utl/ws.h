#ifndef _UTL_WS_H_
#define _UTL_WS_H_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <thread>
#include <functional>

#define REGISTER_JSON_HANDLER(ws, obj, handler) \
    (ws)->RegisterHandler(#handler, std::bind(&std::remove_reference<decltype(*obj)>::type::handler, std::ref(*obj), std::placeholders::_1));

// 기본 매크로 정의
#define JSONVAL2(typec, type, key) \
    typec key; \
    if (data.isMember(#key)) { \
        key = static_cast<typec>(data[#key].as##type()); \
    } else { \
        console->error((std::string(#key) + " is missing in the JSON data").c_str()); \
        return SGS_ERROR_INVALID_FORMAT; \
    }

// std::string
#define JSONVAL2STRING(key)  \
    std::string key; \
    if (data.isMember(#key)) { \
        key = data[#key].asString(); \
    } else { \
        console->error((std::string(#key) + " is missing in the JSON data").c_str()); \
        return SGS_ERROR_INVALID_FORMAT; \
    }

// const char*
#define JSONVAL2CSTR(key)  \
    const char* key; \
    if (data.isMember(#key)) { \
        key = data[#key].asString().c_str(); \
    } else { \
        console->error((std::string(#key) + " is missing in the JSON data").c_str()); \
        return SGS_ERROR_INVALID_FORMAT; \
    }

#define JSONVAL2BIN(ptr_name, data_len_name) \
    uint8_t* ptr_name = nullptr; \
    uint16_t data_len_name = 0; \
    if (data.isMember(#ptr_name) && data[#ptr_name].isObject()) { \
        const Json::Value& bin_data = data[#ptr_name]; \
        if (!bin_data.isMember("length") || !bin_data.isMember("data")) { \
            console->error((std::string(#ptr_name) + " must have 'length' and 'data'").c_str()); \
            return SGS_ERROR_INVALID_FORMAT; \
        } \
        data_len_name = static_cast<uint16_t>(bin_data["length"].asUInt()); \
        std::string hex_str = bin_data["data"].asString(); \
        if (hex_str.length() != data_len_name * 2) { \
            console->error("Binary data length mismatch with hex string length"); \
            return SGS_ERROR_INVALID_FORMAT; \
        } \
        ptr_name = (uint8_t*)malloc(data_len_name); \
        if (!ptr_name) { \
            console->error("Memory allocation failed for binary data"); \
            return SGS_ERROR_NULL; \
        } \
        for (size_t i = 0; i < data_len_name; ++i) { \
            std::string byte_str = hex_str.substr(i * 2, 2); \
            ptr_name[i] = static_cast<uint8_t>(std::stoi(byte_str, nullptr, 16)); \
        } \
    } else { \
        console->error((std::string(#ptr_name) + " binary object is missing").c_str()); \
        return SGS_ERROR_INVALID_FORMAT; \
    }


// 자료형별 매크로 정의
// uint8_t
#define JSONVAL2UINT8(key)   JSONVAL2(uint8_t, UInt, key)  // uint8_t
// uint16_t
#define JSONVAL2UINT16(key)  JSONVAL2(uint16_t, UInt, key)  // uint16_t
// uint32_t
#define JSONVAL2UINT32(key)  JSONVAL2(uint32_t, UInt, key)  // uint32_t
// uint64_t
#define JSONVAL2UINT64(key)  JSONVAL2(uint64_t, UInt64, key)  // uint64_t
// int8_t
#define JSONVAL2INT8(key)    JSONVAL2(int8_t, Int, key)   // int8_t
// int16_t
#define JSONVAL2INT16(key)   JSONVAL2(int16_t, Int, key)   // int16_t
// int32_t
#define JSONVAL2INT32(key)   JSONVAL2(int32_t, Int, key)   // int32_t
// int64_t
#define JSONVAL2INT64(key)   JSONVAL2(int64_t, Int64, key)   // int64_t
// float
#define JSONVAL2FLOAT(key)   JSONVAL2(float, Float, key) // float
// double
#define JSONVAL2DOUBLE(key)  JSONVAL2(double, Double, key) // double


typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_ptr;
typedef websocketpp::lib::error_code error_code;

class SGS_WebSocketClient {
private:
    client wsClient;
    client::connection_ptr connection;
    std::string myname, service_provider;
    thread_ptr clientThread;
    bool running;

    // datatype 후킹을 위한 함수 맵
    std::unordered_map<std::string, std::function<void(const Json::Value&)>> datatypeHandlers;

public:
    SGS_WebSocketClient(const std::string& _myname, const std::string& _service_provider);
    ~SGS_WebSocketClient();

    void Connect(const std::string& uri);
    void Stop();

    void RegisterHandler(const std::string& datatype, std::function<void(const Json::Value&)> handler);

    Json::Value MakeHexAsciiJson(size_t length, uint8_t* ptr);
    std::pair<size_t, uint8_t*> ParseHexAsciiJson(const Json::Value& json);

    void ReadMessage(const Json::Value& message);
    
    int CheckJSONError(const Json::Value& data);
    Json::Value CStringtoHexJsonData(const char* cstr, size_t length);

    
    void SendMessage(const std::string& client, const std::string& reqtype, const Json::Value& data);
    void SendMessage2Provider(const std::string& reqtype, const Json::Value& data);
    void SendRequest(const std::string& client, const std::string& reqtype);
    void SendRequest2Provider(const std::string& reqtype);
    void SendReport(const std::string& client, int retcode);
    void SendReport2Provider(int retcode);
};

#endif