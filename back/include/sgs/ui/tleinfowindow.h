#pragma once
#ifndef _SGS_TLEINFO_H_
#define _SGS_TLEINFO_H_
#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#endif

class SGS_TLEInfoWindow
{
private:
	bool _open;
	float _x_pos, _y_pos, _width, _height = 0;

	char SearchBuf[256];
	char SelectButtonTextBuf[64];
public:
	ImGuiWindowFlags WindowFlag;
	float FontScale;

	SGS_TLEInfoWindow();
	~SGS_TLEInfoWindow();
	
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
	bool popup_load();
};
#endif