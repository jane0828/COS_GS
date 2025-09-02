#include <dcs/dcs.h>
#include <utl/utl.h>

void * RotatorReadInfo(void * param)
{
    GS232B * rot = (GS232B *)param;
    if(!rot)
        return nullptr;
    rot->RotatorReading = true;

    while(rot->RotatorReading)
    {
        // rot->RotatorAngle();
        if(rot->RotatorReading)
            rot->RotatorReading = false;
    }
    return nullptr;
}


GS232B::GS232B(const char* _devname, uint32_t _baud)
    : SGS_RS232(_devname, _baud)
{
    memset(this->ControlBuf, 0, sizeof(this->ControlBuf));
    memset(this->ReadBuf, 0, sizeof(this->ReadBuf));

    pthread_create(&THREAD[0], NULL, RotatorReadInfo, NULL);

    this->console = new SGS_CONSOLE();
    this->console->set_catalog(SGS_DCS);
}

GS232B::~GS232B()
{
    this->RotatorReading = false;
    pthread_join(this->THREAD[0], NULL);
    this->RotatorReady = false;
    this->Close();
}

int GS232B::register_handle(SGS_WebSocketClient* _ws)
{
    if (!_ws)
        return SGS_ERROR_NULL;

    this->ws = _ws;

	// Register JSON handler functions using the macro
	REGISTER_JSON_HANDLER(this->ws, this, set_angle_json)
	REGISTER_JSON_HANDLER(this->ws, this, get_angle_json)

    return SGS_OK;
}

int GS232B::set_angle(int azimuth, int elevation)
{   
    this->RotatorReady = false;
    sprintf(this->ControlBuf, "W%03d %03d\r\n", azimuth, elevation);
    this->az_target = azimuth;
    this->el_target = elevation;
    int res = this->Write(this->ControlBuf, strlen(this->ControlBuf));
    if(res > 0)
    {
        this->console->info("Tracking : Az %d, El %d.", azimuth, elevation);
    }
    else
        this->console->error("Write to rotator failed.");
    
    this->RotatorReady = true;
    return res;
}

int GS232B::set_angle_json(const Json::Value& data)
{
    if (!this->ws)
    return SGS_ERROR_NULL;

    int res = ws->CheckJSONError(data);
    if (res != SGS_OK)
        return res;

    JSONVAL2INT32(az)
    JSONVAL2INT32(el)
    res = this->set_angle(az, el);

    ws->SendReport2Provider(res);
    return res;
}

int GS232B::get_angle()
{
    this->byte = 0;
    this->read_count = 0;
    memset(this->ReadBuf, 0, strlen(this->ReadBuf));
    if(this->Write("C2\r\n", 4) > 0)
    {
        this->RotatorReady = true;
    }
    else
    {
        this->console->error("Rotator Read Failed. Please Check Again.");
        this->RotatorReading = false;
        return SGS_ERROR_BUSY;
    }

    this->RotatorReading = true;
    
    sleep(1);
    while(this->RotatorReading && this->RotatorReady)
    {
        byte = this->Read(&this->ReadBuf[read_count], 1);
        if (byte < 1 && read_count > 3 && this->ReadBuf[read_count - 1] == '\n')
        {
            char azi[5] = {0}, ele[5] = {0};

            if (this->ReadBuf[3] == '-')	// AZ=-XXX
            {
                // Sometime azi / ele can be negative value
                // in that case, we need calibration

                strncpy(azi, &this->ReadBuf[3], 4);
                this->az_actual= atoi(azi);

                this->console->error("Rotator azimuth calibration is necessary.");

                if (this->ReadBuf[12] == '-')		// AZ=-XXX  EL=-XXX
                {
                    strncpy(ele, &this->ReadBuf[12], 4);
                    this->el_actual = atoi(ele);

                    this->console->error("Rotator elevation calibration is necessary.");
                }
                else								// AZ=-XXX  EL=XXX
                {
                    strncpy(ele, &this->ReadBuf[12], 3);
                    this->el_actual = atoi(ele);
                }
            }
            else
            {
                strncpy(azi, &this->ReadBuf[3], 3);
                this->az_actual = atoi(azi);

                if (this->ReadBuf[11] == '-')		// AZ=XXX  EL=-XXX
                {
                    strncpy(ele, &this->ReadBuf[11], 4);
                    this->el_actual = atoi(ele);

                    this->console->error("Rotator elevation calibration is necessary.");
                }
                else								// AZ=XXX  EL=XXX
                {
                    strncpy(ele, &this->ReadBuf[11], 3);
                    this->el_actual = atoi(ele);
                }
            }
            read_count = 0;
            sleep(1);
            this->RotatorReady = false;
        }
        else if (byte > 0)
        {
            read_count++;
        }
        else if (byte < 1)
        {
            this->console->error("Rotator Read Failed.");
            return SGS_ERROR_NOT_CONNECTED;
        }
    }
    return SGS_OK;
}

int GS232B::get_angle_json(const Json::Value& data)
{
    if (!this->ws)
	return SGS_ERROR_NULL;

	int res = ws->CheckJSONError(data);
	if (res != SGS_OK)
		return res;

    res = this->get_angle();

    Json::Value reply;
    reply["az"] = this->az_actual;
    reply["el"] = this->el_actual;
    ws->SendMessage2Provider("gs232b_get_angle", reply);

    return res;
}