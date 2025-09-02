#include <utl/utl.h>

SGS_WebSocketClient::SGS_WebSocketClient(const std::string& _myname, const std::string& _service_provider) : running(false) {
    this->myname = _myname;
    this->service_provider = _service_provider;
    wsClient.init_asio();
    wsClient.set_open_handler([this](connection_hdl hdl) {
        std::cout << "Connected to server" << std::endl;
    });
    wsClient.set_message_handler([this](connection_hdl hdl, client::message_ptr msg) {
        std::cout << "Received: " << msg->get_payload() << std::endl;
        
        Json::CharReaderBuilder builder;
        Json::Value root;
        std::string errs;

        std::istringstream s(msg->get_payload());
        if (Json::parseFromStream(builder, s, &root, &errs)) {
            this->ReadMessage(root);  // ✅ 핵심: 파싱 후 실제 처리 함수 호출
        } else {
            std::cerr << "JSON parse error: " << errs << std::endl;
        }
    });

    wsClient.set_close_handler([this](connection_hdl hdl) {
        std::cout << "Disconnected from server" << std::endl;
    });
}

SGS_WebSocketClient::~SGS_WebSocketClient() {
    Stop();
}

void SGS_WebSocketClient::Connect(const std::string& uri) {
    if (running) return;
    running = true;
    error_code ec;
    connection = wsClient.get_connection(uri, ec);
    if (ec) {
        std::cerr << "Connection failed: " << ec.message() << std::endl;
        return;
    }
    wsClient.connect(connection);
    clientThread = thread_ptr(new websocketpp::lib::thread([this]() { wsClient.run(); }));
}

void SGS_WebSocketClient::Stop() {
    if (running) {
        running = false;
        error_code ec;
        wsClient.stop();
        if (clientThread && clientThread->joinable()) {
            clientThread->join();
        }
    }
}

void SGS_WebSocketClient::RegisterHandler(const std::string& reqtype, std::function<void(const Json::Value&)> handler) {
    datatypeHandlers[reqtype] = handler;
}

int SGS_WebSocketClient::CheckJSONError(const Json::Value& data)
{
    if (data.isObject() && data.isMember("error")) {
		if (data["error"].isString()) {
			printf("Error: %s\n", data["error"].asCString());
		} else {
			// error 값이 문자열이 아닌 경우도 문자열로 출력
			Json::StreamWriterBuilder writer;
			std::string errorStr = Json::writeString(writer, data["error"]);
			printf("Error: %s\n", errorStr.c_str());
		}
        return SGS_ERROR_INVALID_PARAMETER;
	}
    else
        return SGS_OK;
}

Json::Value SGS_WebSocketClient::MakeHexAsciiJson(size_t length, uint8_t* ptr) {
    Json::Value result;
    std::ostringstream oss;

    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ptr[i]);
    }

    result["length"] = static_cast<Json::UInt>(length); // Json::UInt 형식으로 캐스팅
    result["data"] = oss.str(); // HEX로 된 ASCII 문자열

    return result;
}

std::pair<size_t, uint8_t*> SGS_WebSocketClient::ParseHexAsciiJson(const Json::Value& json) {
    if (!json.isMember("length") || !json.isMember("data")) {
        throw std::invalid_argument("Invalid JSON: missing 'length' or 'data'");
    }

    size_t length = json["length"].asUInt();
    std::string hexStr = json["data"].asString();

    if (hexStr.length() != length * 2) {
        throw std::invalid_argument("Mismatch between length and hex string size");
    }

    uint8_t* buffer = new uint8_t[length];

    for (size_t i = 0; i < length; ++i) {
        std::string byteStr = hexStr.substr(i * 2, 2);
        buffer[i] = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
    }

    return { length, buffer };
}



// JSON 데이터 처리
void SGS_WebSocketClient::ReadMessage(const Json::Value& message) {
    // 1) 포맷 A: { "client": "...", "reqtype": "...", "data": {...} }
    if (message.isMember("client") && message.isMember("reqtype")) {
        std::string client  = message["client"].asString();
        std::string reqtype = message["reqtype"].asString();

        // 내 대상이 아니면 무시
        if (client != this->myname) return;

        Json::Value data;
        if (message.isMember("data")) data = message["data"];

        auto it = datatypeHandlers.find(reqtype);
        if (it != datatypeHandlers.end()) {
            it->second(data);
        } else {
            std::cerr << "No handler registered for reqtype: " << reqtype << std::endl;
        }
        return;
    }

    // 2) 포맷 B: { "type": "...", "to": "...", "message": {...} }
    else if (message.isMember("type") && message.isMember("to") && message.isMember("message")) {
        std::string to      = message["to"].asString();
        std::string reqtype = message["type"].asString(); // 예: "private"

        if (to != this->myname) return; // 내 대상이 아니면 무시

        Json::Value data = message["message"]; // payload: { msgid, cc, parameters }

        auto it = datatypeHandlers.find(reqtype);
        if (it != datatypeHandlers.end()) {
            it->second(data);
        } else {
            std::cerr << "No handler registered for type: " << reqtype << std::endl;
        }
        return;
    }

    // 3) 둘 다 아니면 에러
    else {
        std::cerr << "Invalid JSON format" << std::endl;
    }
}

void SGS_WebSocketClient::SendMessage(const std::string& client, const std::string& reqtype, const Json::Value& data) {
    Json::Value message;
    message["client"] = this->service_provider;
    message["reqtype"] = reqtype;
    message["data"] = data; // data는 Json::Value 형태로 받음

    Json::StreamWriterBuilder writer;
    std::string jsonString = Json::writeString(writer, message);
    if (!running || !connection) return;
    error_code ec;
    wsClient.send(connection->get_handle(), jsonString, websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cerr << "Send failed: " << ec.message() << std::endl;
    }
}

void SGS_WebSocketClient::SendMessage2Provider(const std::string& reqtype, const Json::Value& data)
{
    return SendMessage(this->service_provider, reqtype, data);
}

void SGS_WebSocketClient::SendRequest(const std::string& client, const std::string& reqtype)
{
    Json::Value data;
    return this->SendMessage(client, reqtype, data);
}

void SGS_WebSocketClient::SendRequest2Provider(const std::string& reqtype)
{
    return this->SendRequest(this->service_provider, reqtype);
}

void SGS_WebSocketClient::SendReport(const std::string& client, int retcode)
{
    Json::Value result;
    result["retcode"] = retcode;
    return this->SendMessage(client, "report", result);
}

void SGS_WebSocketClient::SendReport2Provider(int retcode)
{
    return this->SendReport(this->service_provider, retcode);
}