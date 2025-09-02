#pragma once
#ifndef _SGS_TRXPARAM_H_
#define _SGS_TRXPARAM_H_
#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#include <time.h>
#include <inttypes.h>
#endif

class SGS_TRXParamWindow
{
private:
	bool _open = false;

	float _x_pos = 0;
	float _y_pos = 0;
	float _width = 0;
	float _height = 0;

public:
    Ptable0* param0 = nullptr;
    Ptable1* param1 = nullptr;
    Ptable5* param5 = nullptr;
	ImGuiWindowFlags WindowFlag = 0;
	float FontScale = 1.0f;

	SGS_TRXParamWindow();
	~SGS_TRXParamWindow();

	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
};

#endif