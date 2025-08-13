#include <gts/gts.h>

void * RotatorSattracker(void * param)
{
    // GS232B * rot = (GS232B *)param;
    // rot->RotatorEngage = true;

    // while(rot->RotatorEngage)
    // {
    //     if(rot->Sat == NULL)
    //     {
    //         rot->console->error("Cannot Detect Tracking rot->Satellites.");
    //         break;
    //     }
    
    //     if(rot->Sat->Elevation() < 0)
    //     {
    //         rot->console->debug("Change to AOS.");
    //         rot->az_tag = rot->Sat->AOS_Azimuth();
    //         rot->el_tag = rot->Sat->AOS_Elevation();
    //     }
    //     else
    //     {
    //         rot->az_tag = rot->Sat->Azimuth_1secfuture();
    //         rot->el_tag = rot->Sat->Elevation_1secfuture();
    //     }
    //     if(rot->el > 80)
    //     {
    //         rot->el_tag = 80;
    //         rot->Sat->futuretime = 6.0f;
    //     }
    //     else
    //         rot->Sat->futuretime = 3.0f;
        
    //     if(rot->Sat->CrossPath)
    //     {
    //         switch(rot->Sat->CrossPath)
    //         {
    //             case(1): {
    //                 if(rot->az_tag < 90)
    //                     rot->az_tag += 360;
    //                 break;
    //             }
    //             case(-1): {
    //                 if(rot->az_tag < 90)
    //                     rot->az_tag += 360;
    //                 break;
    //             }
    //         }
    //     }
    //     if ((abs(rot->az_tag - rot->az) >= rot->tolrnc_az || abs(rot->el_tag - rot->el) >= rot->tolrnc_el))
    //     {
            

    //         rot->console->info("Azimuth : %d, Elevation : %d\n", rot->az_tag, rot->el_tag);
    //         rot->RotatorSetTo(rot->az_tag, rot->el_tag);
    //     }
    //     else
    //         rot->console->debug("Tolerance Filtering...");
    //     sleep(1);
    // }
    return NULL;
}

int GTS_Propagator::set_observer(double latitude, double longitude, double altitude)
{
    // if(!this->obs)
    //     return SGS_ERROR_NULL;
    
    // this->geo->latitude = latitude;
    // this->geo->longitude = longitude;
    // this->geo->altitude = altitude;
    // obs->SetLocation(*(this->geo));
    // return SGS_OK;
}
