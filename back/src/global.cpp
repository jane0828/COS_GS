
#include <sgs/include.h>

pthread_t p_thread[16];
int NowFTP;
bool sgp4check;

Rotator * rotator;

Render * render;
SGS_TLE * tle[64];
Autopilot * autopilot;


SGS_PropagatorWindow * PropagatorWindow;
SGS_TLEInfoWindow * TLEInfoWindow;
SGS_ConsoleWindow * console;
SGS_SetupInfoWindow * SetupWindow;
SGS_RotatorWindow * ROTContWindow;
SGS_TLMWindow * TLMWindow;
SGS_TRXWindow * TRXWindow;
SGS_TRXParamWindow * TRXParamWindow;
SGS_FTPWindow * FTPWindow;