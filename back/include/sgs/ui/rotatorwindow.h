#pragma once
#ifndef _SGS_ROTCONT_H_
#define _SGS_ROTCONT_H_
#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#endif

class SGS_RotatorWindow
{
private:
	bool _open = false;

	float _x_pos = 0;
	float _y_pos = 0;
	float _width = 0;
	float _height = 0;
	int _BeaconCounter = 0;
	int _Pingcounter = 0;

public:
	ImGuiWindowFlags WindowFlag = 0;
	float FontScale = 1.0f;
	bool Engage = false;

	SGS_RotatorWindow();
	~SGS_RotatorWindow();
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
};

#endif