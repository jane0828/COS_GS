#include <stdio.h>
#include <utl/utl.h>
#include <ifc/ifc.h>
#include <dcs/dcs.h>
#include <gts/gts.h>
// #include <ncs/ncs.h>
// #include <ros/ros.h>

int main(int argc, char* argv[])
{
    MySQLDatabase mysqldb("localhost", "miman", "qwe123", "HYVRID");
    mysqldb.connect();

    SGS_WebSocketClient wsclient("gs634", "client634");
    wsclient.Connect("ws://localhost:4443");

    SGS_CSP cspclient(8);
    SGS_GS100 uhftrx(&cspclient, "/dev/GS102", 500000);
    uhftrx.register_handle(&wsclient);
    cspclient.set_route_table(20, CSP_NO_VIA_ADDRESS);
    cspclient.set_route_table(7, 20);
    cspclient.set_route_table(28, 20);

    // GS232B rotator;
    SGS_KTA223* switchbox = new SGS_KTA223("/dev/KTA223", 115200);
    // SGS_Q5Signal amp;

    std::vector<SGS_TLE *> TleSource;
    TleSource.push_back(new SGS_TLE("active"));
    TleSource.push_back(new SGS_TLE("weather"));

    while(1);

    return 0;
}