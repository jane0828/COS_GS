#pragma once
#pragma once
#ifndef _SGS_TLM_H_
#define _SGS_TLM_H_
#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#endif
#pragma pack(push, 1)

class SGS_TLMWindow
{
private:
	bool _open = false;

	float _x_pos = 0;
	float _y_pos = 0;
	float _width = 0;
	float _height = 0;
    char callsignbuf[64];

    void StateWindowColumnManager(const char * info);
public:
	ImGuiWindowFlags WindowFlag = 0;
	float FontScale = 1.0f;

	SGS_TLMWindow();
	~SGS_TLMWindow();
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
    void Render_Beacon(float fontscale);
};

#pragma(pop)
#endif