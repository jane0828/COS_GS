#ifndef _GTS_SATELLITEOBJECT_H_
#define _GTS_SATELLITEOBJECT_H_

#include <utl/utl.h>

#include <SGP4.h>
#include <Observer.h>
#include <Tle.h>
#include <CoordTopocentric.h>
using namespace libsgp4;
#define RAD_TO_DEG      57.2958

void * Propagator(void* param);

class SatelliteObject {
private:
    char _name[30];
    DateTime start_date;
    DateTime end_date;

public:
    bool use, cal;
    SGS_CONSOLE* console = NULL;

    Observer* obs;
    Tle* tle;
    SGP4* sgp4 = nullptr;
    Eci* eci = nullptr;

    Eci* AOSeci[64];

    CoordTopocentric topo, _topo_1secfuture;
    CoordTopocentric AOStopo[64];
    CoordGeodetic geo;

    DateTime _nextaos[64];
    DateTime _nextlos[64];

    double _max_elevation[64];

    double path_az[1024];
    double path_el[1024];

    double futuretime = 3.0f;
    int CrossPath = 0;

    SatelliteObject(Tle& _tle, Observer& _InputObserver);
    ~SatelliteObject();
    void Refresh();

    void Initializer(bool sgp4on);
    std::string Name();
    int Azimuth();
    int Elevation();
    int Azimuth_1secfuture();
    int Elevation_1secfuture();
    int AOS_Azimuth(int i = 0);
    int AOS_Elevation(int i = 0);
    double MaxElevation(int i = 0);
    float DisplayPoint_Longitude();
    float DisplayPoint_Latitude();
    void Update();
    void GeneratePath();
    bool GeneratePassList(const int time_step);
    double FindMaxElevation(Observer obs, SGP4& sgp4, const DateTime& aos, const DateTime& los);
    DateTime FindCrossingPoint(Observer obs, SGP4& sgp4, const DateTime& initial_time1, const DateTime& initial_time2, bool finding_aos);
};

#endif