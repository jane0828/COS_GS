#ifndef _SGS_CONSOLE_H_
#define _SGS_CONSOLE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <cstdio>
#include <cstdarg>
#include <jsoncpp/json/json.h>

typedef enum {
    SGS_DEBUG,
    SGS_INFO,
    SGS_WARNING,
    SGS_ERROR
} SGS_LogLevel_n;

typedef enum {
    SGS_NOT_CLASSIFIED,
    SGS_INTERNAL,
    SGS_IFC,
    SGS_DCS,
    SGS_ARS,
    SGS_GTS,
    SGS_HDS,
    SGS_NCC,
    SGS_RBS,
    SGS_TCS,
    SGS_UTIL
} SGS_LogCatalog_n;


class SGS_CONSOLE {
private:
    SGS_LogLevel_n level = SGS_DEBUG;
    SGS_LogCatalog_n catalog = SGS_NOT_CLASSIFIED;
    std::mutex mtx;

    // SGS_CONSOLE() = default;
    // ~SGS_CONSOLE() = default;

    std::string levelToString(SGS_LogLevel_n level) {
        switch (level) {
            case SGS_LogLevel_n::SGS_DEBUG: return "DEBUG";
            case SGS_LogLevel_n::SGS_INFO: return "INFO";
            case SGS_LogLevel_n::SGS_WARNING: return "WARNING";
            case SGS_LogLevel_n::SGS_ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }
    
public:
    SGS_CONSOLE();
    ~SGS_CONSOLE();

    void set_level(SGS_LogLevel_n _level);

    void set_catalog(SGS_LogCatalog_n _catalog);

    template<typename... Args>
    void log(SGS_LogLevel_n mode, const char* fmt, Args... args);
    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);
};

#endif