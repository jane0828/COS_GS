#pragma once
#ifndef _SGS_TRX_H_
#define _SGS_TRX_H_
#ifndef _SGS_INCLUDE_H_
#include <imgui.h>
#include <time.h>
#endif

/* Time Settings*/
#define DOPPLER_TIMESTEP            500



/* Node & Port Configurations */
// Node Configuration
#define TX_PORT                     13

// Port Configuration
#define RPARAM_PORT                 7
#define TEST_PORT		            10
#define	BCN_PORT                    31
#define TRX_PORT                    13
#define FTPFCD_PORT                 15
#define FTPRDP_PORT                 16
#define TASK_SYN	                0x01
#define TASK_ACK                    0x02
#define TASK_EAK                    0x04
#define TASK_RST	                0x08


class SGS_TRXWindow
{
private:
	bool _open = false;

	float _x_pos = 0;
	float _y_pos = 0;
	float _width = 0;
	float _height = 0;

	double _default_freq = 436.5;
    char AmpTimeBuf[64];
    
public:
	ImGuiWindowFlags WindowFlag = 0;
	float FontScale = 1.0f;

	bool Doppler = false;
	bool GS100_Connection = false;

	SGS_TRXWindow();
	~SGS_TRXWindow();

	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
};

void * Doppler(void *);
void * AmpTimer(void *);

#endif