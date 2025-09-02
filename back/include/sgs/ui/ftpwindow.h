#pragma once
#ifndef _SGS_FTPWINDOW_H_
#define _SGS_FTPWINDOW_H_
#ifndef _SGS_INCLUDE_H_
#include <inttypes.h>
#include <imgui.h>
#endif

typedef struct {
    char name[30];
    char local_path[256];
    char remote_path[256];
}ftpinfo;

class SGS_FTPWindow
{
private:
	bool _open = false;

	float _x_pos = 0;
	float _y_pos = 0;
	float _width = 0;
	float _height = 0;

	int _temptarget = 0;
	int _tempstatus = 0;
	int _tempfilenum = 1;
	int _tempstep = 0;
	int _tempoffset = 0;

	int _ftp_version = 2;
    int _ftp_task = FTP_UPLOAD_REQUEST;
	uint32_t _chunk_sz = 200;
	bool _ftp_mode = false;
	char _FTPWindowBuffer[64];
    char _temppath[256];
public:
    int NowFTP = 0;
    ftpinfo ftplistup[64];
	ImGuiWindowFlags WindowFlag = 0;
	float FontScale = 1.0f;

	SGS_FTPWindow();
	~SGS_FTPWindow();
	void Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight);
	void Render();
};

#endif