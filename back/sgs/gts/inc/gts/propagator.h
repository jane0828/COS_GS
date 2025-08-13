#ifndef _GTS_PROPAGATOR_H_
#define _GTS_PROPAGATOR_H_

#include <pthread.h>
#include <SGP4.h>
#include <utl/utl.h>

class GTS_Propagator
{
private:
    bool HANDLE[16];
    pthread_t THREAD[16];
    SGS_CONSOLE* console = NULL;
    SGS_WebSocketClient* ws = NULL;
public:
    GTS_Propagator();
    ~GTS_Propagator();

    int set_observer(double latitude, double longitude, double altitude);
	int set_observer_json(const Json::Value& data);

    int start_tracking();
    int start_tracking_json(const Json::Value& data);

    int get_future_path();
    int get_future_path_json(const Json::Value& data);

    int get_footprint();
    int get_footprint_json(const Json::Value& data);

    int start_engage();
    int start_engage_json(const Json::Value& data);
};

#endif