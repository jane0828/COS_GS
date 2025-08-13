#pragma once
#ifndef _SCL_SETUPINFO_H_
#define _SCL_SETUPINFO_H_
#ifndef _SCL_INCLUDE_H_
#include <stdio.h>
#include <iostream>
#include <imgui.h>
#endif


class SGS_SetupInfoWindow
{
private:
    bool _open = false;
    float _x_pos, _y_pos, _width, _height = 0;
    char _Transciever_devname[256];
    char _Rotator_devname[256];
    char _Switch_devname[256];
    char _DebugFilePath[256];
    char _S_Username[64];
    char _S_Address[64];
    char _S_passwd[64];

    uint32_t _Transceiver_baud = 0;
    uint32_t _Rotator_baud = 0;
    uint32_t _Switch_baud = 0;

    uint8_t _gs100_node = 0;
    uint8_t _kiss_node = 0;
    uint8_t _obc_node = 0;
    uint8_t _ax100_node = 0;

    double _default_freq = 0;

    bool _SbandUse = false;

public:
    ImGuiWindowFlags WindowFlag;
    float FontScale;

    uint16_t _pingsize = 0;

    uint32_t _default_timeout = 0;
    uint32_t _guard_delay = 0;
    uint32_t _queue_delay = 0;

	SGS_SetupInfoWindow();                               
	~SGS_SetupInfoWindow();
    void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);

	void Render();
};
#endif