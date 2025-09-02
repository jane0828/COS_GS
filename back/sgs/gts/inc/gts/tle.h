#ifndef _GTS_TLE_H_
#define _GTS_TLE_H_

#include <inttypes.h>
#include <Observer.h>
#include <iostream>

class SGS_TLE
{
private:

public:
	SGS_CONSOLE* console = NULL;
	SGS_WebSocketClient* ws = NULL;

	std::string label;
	std::string local;
	std::string remote;
	bool webref = false;
	bool imported = false;
	bool sgp4on = false;
	
	uint32_t idx = 0;
	uint32_t trackidx = 0;
	int count = 0;

	std::vector<SatelliteObject*> Satellites;

	CoordGeodetic* geo;
	Observer * obs = new Observer(37.564021, 126.934600, 0.05);

	SGS_TLE(std::string labSCLel);
	SGS_TLE(std::string _local, std::string _remote);
	~SGS_TLE();
	int register_handle(SGS_WebSocketClient* _ws);
	int set_observer(double latitude, double longitude, double altitude);
	int set_observer_json(const Json::Value& data);

	int download_tle();
	int download_tle_json(const Json::Value& data);

	void * Import();

	int get_tle_lines(char* TLEFilename, bool fds, bool sgp4on);
	int get_tle_lines_json(const Json::Value& data);
};


#endif