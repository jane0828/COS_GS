#pragma once
#ifndef _SGS_PROPAGATOR_H_
#define _SGS_PROPAGATOR_H_
#ifndef _SGS_INCLUDE_H_
#include <inttypes.h>
#include <pthread.h>
#include <imgui.h>
#endif
void * TrackingSatellites(void * param);

class SGS_PropagatorWindow
{
private:
	bool _open;
	float _x_pos, _y_pos, _width, _height = 0;
	char _ButtonLabel_Info[64];
	char _ButtonLabel_Track[64];
public:
	uint32_t TLEref = 0;
	bool THREAD[16];
	pthread_t HANDLES[16];

	ImGuiWindowFlags WindowFlag;
	float FontScale;

	SGS_PropagatorWindow();
	~SGS_PropagatorWindow();
	
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
	bool popup_satinfo(uint32_t idx, uint32_t row);
};

#endif