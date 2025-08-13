#pragma once
#ifndef _SGS_CONSOLE_H_
#define _SGS_CONSOLE_H_

#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#include <fstream>
#endif

#define SGS_CONMOD_LOG          0
#define SGS_CONMOD_ERR          1
#define SGS_CONMOD_WRN          2
#define SGS_CONMOD_DBG          3
#define SGS_CONMOD_OK           4
#define SGS_CONMOD_AUTO         5
#define SGS_CONMOD_AUTOERR      6

class SGS_ConsoleWindow
{
private:
    bool _open = false;
    float _x_pos, _y_pos, _width, _height = 0;
    ImVector<char*>     _items;
    std::fstream        _history_fs;
    std::fstream        _debug_fs;
    time_t              _ntt;
    tm                  _ntm;
    bool                _push_to_bottom;

    char _buf[1000];
    char _now[24];
    char _msg[1024];
    char _date[32];

public:
    ImGuiWindowFlags WindowFlag;
    float FontScale;
    const char* title = "Console Window";

    
    SGS_ConsoleWindow();
    ~SGS_ConsoleWindow();

    int Stricmp(const char* str1, const char* str2);
    int Strnicmp(const char* str1, const char* str2, int n);
    char* Strdup(const char* str);
    void Strtrim(char* str);

    void ClearLog();
    void AddLog(int mode, const char* fmt, ...);
    void DelStartingCharLog(const char* fmt);
    void DelPrefixLog(const char* fmt);
    void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
    void Render();
};
#endif
