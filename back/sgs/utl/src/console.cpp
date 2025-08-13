#include <utl/utl.h>

SGS_CONSOLE::SGS_CONSOLE()
{

}

SGS_CONSOLE::~SGS_CONSOLE()
{

}

void SGS_CONSOLE::set_level(SGS_LogLevel_n _level) {
    this->level = _level;
}

void SGS_CONSOLE::set_catalog(SGS_LogCatalog_n _catalog) {
    this->catalog = _catalog;
}

template<typename... Args>
void SGS_CONSOLE::log(SGS_LogLevel_n mode, const char* fmt, Args... args) {
    if (mode < this->level) return;

    char buffer[256];
    std::snprintf(buffer, sizeof(buffer), fmt, args...);

    Json::Value logJson;
    logJson["level"] = levelToString(mode);
    logJson["catalog"] = this->catalog;
    logJson["log"] = buffer;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "";

    std::lock_guard<std::mutex> lock(mtx);
    std::cout << Json::writeString(writer, logJson) << std::endl;
}


void SGS_CONSOLE::debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    this->log(SGS_LogLevel_n::SGS_DEBUG, fmt, args);
    va_end(args);
}

void SGS_CONSOLE::info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    this->log(SGS_LogLevel_n::SGS_INFO, fmt, args);
    va_end(args);
}

void SGS_CONSOLE::warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    this->log(SGS_LogLevel_n::SGS_WARNING, fmt, args);
    va_end(args);
}

void SGS_CONSOLE::error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    this->log(SGS_LogLevel_n::SGS_ERROR, fmt, args);
    va_end(args);
}