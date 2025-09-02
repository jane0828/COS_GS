#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif




void * Pilot(void * param)
{
    if(!param)
        return NULL;
    Autopilot * autop = (Autopilot *) param;

    this->callback(SGS_CONMOD_AUTO, "Start AutoPilot.");
    int AutoRes = autop->AutoPilot_task();
    if(AutoRes)
        this->callback(SGS_CONMOD_AUTOERR, "AutoPilot Stopped with Error Code : %d", AutoRes);
    if(AutoRes != -2)
    {
        State.Doppler = false;
        pthread_join(p_thread[7], NULL);
        this->callback(SGS_CONMOD_AUTO, "Stop Doppler Correction.");

        State.Engage = false;
        State.Doppler = false;
        pthread_join(p_thread[1], NULL);
        this->callback(SGS_CONMOD_AUTO, "Finish Engaging.");
        
        RotatorSetTo(0, 0);
        this->callback(SGS_CONMOD_AUTO, "Beacon Success : %d, Ping Success : %d CMD Success : %d", autop->BeaconCounter, autop->PingCounter, State.CMDCount);
        autop->BeaconCounter = 0;
        autop->PingCounter = 0;
    }
    State.Successed = 0;
    return NULL;
}

void * SatelliteInitialize(void * sgp4on)
{
    
    State.tleallindex = 32768;
    State.Display_TLE = false;
    State.Display_load = true;
    for(State.TLEref = 0; State.TLEref < sizeof(State.Satellites) / sizeof(SatelliteObject *); State.TLEref++)
    {
        
        if(State.Satellites[State.TLEref][State.TLEnow] != NULL)
        {
            if(strlen(State.Satellites[State.TLEref][State.TLEnow]->Name()) == 0)
                break;
            State.Satellites[State.TLEref][State.TLEnow]->~SatelliteObject();
        }
    }
    if((State.tleallindex = ReadTLELines(State.tleinfolistup[State.TLEref]->local, false, *((bool *)sgp4on))) <= 0)
        console.AddLog("[ERROR]##Invalid type of TLE File. Please check again.");
    
    
    State.Display_load = false;
    State.Display_TLE = true;
    State.TLEnow = true;
    State.TLEref = 0;
    return NULL;
}

int Autopilot::FindValuablePath()
{
    int Valuable = 0;
    for(int i = 0 ; i < 64; i++)
    {
        if(State.Satellites[State.TLEref][State.TLEnow]->MaxElevation(i) > 10.0f)
        {
            Valuable = i;
            break;
        }
    }
    return Valuable;
}

bool Autopilot::NowOn(int NextValuablePath)
{
    return (DateTime::Now(false) - State.Satellites[State.TLEref][State.TLEnow]->_nextlos[NextValuablePath]).TotalSeconds() < 0;
}

int Autopilot::BeaconCheck()
{
    if(this->BeaconCounter >= State.iteration)
    {
        BeaconAvailable = true;
        break;
    }
        
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTOERR, "Beacon Success is too hard. Beacon Success : %d", this->BeaconCounter);
        break;
    }
}

int Autopilot::PingTry()
{
    sleep(State.SpanTime);
            
    pthread_create(&p_thread[4], NULL, csp_ping_console, NULL);
    if(this->PingCounter >= 1)
    {
        PingAvailable = true;
        break;
    }
        
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTOERR, "Ping Success is too hard. Ping Success : %d", this->PingCounter);
        break;
    }
    pthread_join(p_thread[4], NULL);
}

int Autopilot::Watchdog()
{
    sleep(1);
    if(csp_transaction(CSP_PRIO_HIGH, setup->ax100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0)
    this->callback(SGS_CONMOD_AUTO, "Watchdog Control OK.(No Reply)");
    else
    this->callback(SGS_CONMOD_AUTOERR, "Watchdog Control Failed.(No Reply)");
    if(now_total_rx_bytes != get_rx_bytes())
    {
        this->callback(SGS_CONMOD_AUTO, "Watchdog Reset Success. Total RX Bytes Start :%u, End : %u", now_total_rx_bytes, get_rx_bytes());
        break;
    }
    else if (!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTOERR, "Watchdog Reset is too hard. Total RX Bytes Start :%u, End : %u", now_total_rx_bytes, get_rx_bytes());
        break;
    }
}

int Autopilot::UpSig()
{
    if((inittime - DateTime::Now()).TotalSeconds() > 30)
        this->callback(SGS_CONMOD_AUTO, "No Beacon for 30 seconds. Try Next Task.");
    this->callback(SGS_CONMOD_AUTO, "Try to Send UL Signal");
    
    DateTime BeaconStandard = DateTime::Now(false);
    for(int i = 0; i < 10; i++)
    {
        sleep(State.SpanTime);
        if((DateTime::Now(false) - State.Satellites[State.TLEref][State.TLEnow]->_nextlos[NextValuablePath]).TotalSeconds() > 0)
        {
            // this->callback(SGS_CONMOD_AUTOERR, "Failed to Send UL Signal.");
            break;
        }
        
        // MIM_TLEinfo = FSWTleConverter(TrackingTLE[NowSat]);
        packetsign* TestPacket = (packetsign *)malloc(9);
        TestPacket->Identifier = MIM_ID;
        TestPacket->PacketType = MIM_PT_SIGNAL;
        TestPacket->Length = 1;
        memset(TestPacket->Data, 0, TestPacket->Length);
        pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void *)TestPacket);
        pthread_join(p_thread[4], NULL);
    }

    this->callback(SGS_CONMOD_AUTO, "Try to config total rx bytes.");
    int trying = 0;
    while(this->state && NowOn(NextValuablePath))
    {
        if(gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK)
        {
            if((get_rx_bytes() - initial) % 16 != 0)
            {
                this->callback(SGS_CONMOD_AUTO, "UL Signal Send Done. Init : %u, Now : %u", initial, get_rx_bytes());
                ulsigdone = true;
                break;
            }
        }
        else if (trying == 5)
            break;
        else
        {
            trying ++;
            continue;
        }
            

    }
    
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTOERR, "Failed to Send UL Signal.");
        break;
    }
}

int Autopilot::FSWCMD()
{
    for(int i = 0; i < 256; i++)
    {
        if(SatCMD[i] == NULL)
            continue;
        if(State.Successed > State.CMDCount)
        {
            State.CMDCount++;
            continue;
        }
        while(this->state)
        {
            sleep(State.SpanTime);
            
            if(SatCMD[i]->Checksum)
                SatCMD[i]->GenerateChecksum();
            packetsign * SendingCMD = SatCMD[i]->GenerateCMDPacket();
            pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void *)SendingCMD);
            pthread_join(p_thread[4], NULL);
            if(State.Successed < State.CMDCount)
            {
                State.Successed = State.CMDCount;
                break;
            }
            if(!NowOn(NextValuablePath))
            {
                break;
            }
        }
        if(!NowOn(NextValuablePath))
        {
            break;
        }

    }
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTOERR, "Stop CMD after LOS. Success : %d", State.CMDCount);
        State.CMDCount = 0;
        break;
    }
}

int Autopilot::FTPDL()
{
    if(State.ftp_mode)
        continue;
    sleep(State.SpanTime);
    pthread_join(p_thread[8], NULL);
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTO, "Stop FTP Download after LOS.");
        break;
    }
    if(State.ftp_version == 1)
        pthread_create(&p_thread[8], NULL, ftp_downlink_force, &State.ftplistup[NowFTP]);
    else
        pthread_create(&p_thread[8], NULL, ftp_downlink_onorbit, &State.ftplistup[NowFTP]);
    
}

int Autopilot::FTPUL()
{
    if(State.ftp_mode)
        continue;
    sleep(State.SpanTime);
    pthread_join(p_thread[8], NULL);
    if(!NowOn(NextValuablePath))
    {
        this->callback(SGS_CONMOD_AUTO, "Stop FTP Upload after LOS.");
        break;
    }
    if(State.ftp_version == 1)
        pthread_create(&p_thread[8], NULL, ftp_uplink_force, &State.ftplistup[NowFTP]);
    else
        pthread_create(&p_thread[8], NULL, ftp_uplink_onorbit, &State.ftplistup[NowFTP]);
}

int Autopilot::DoTask()
{
    while(this->state)
    {
        printf("Start AutoPilot.\n");
        int NextValuablePath = FindValuablePath();
        this->callback(SGS_CONMOD_AUTO, "Start Autopilot. Next Valuable AOS : %d/%d %d:%d:%d",
                                                        State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath].AddHours(9).Month(), 
                                                        State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath].AddHours(9).Day(),
                                                        State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath].AddHours(9).Hour(),
                                                        State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath].AddHours(9).Minute(),
                                                        State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath].AddHours(9).Second());
        this->callback(SGS_CONMOD_AUTO, "Max Elevation : %.2lf° Please Keep Devices On.", State.Satellites[State.TLEref][State.TLEnow]->_max_elevation[NextValuablePath] * RAD_TO_DEG);
        // printf("%f\n", (State.Satellites[SatIndex]->_nextaos[NextValuablePath] -  DateTime::Now(true)).TotalMinutes());
        while(this->state)
        {
            if((State.Satellites[State.TLEref][State.TLEnow]->_nextaos[NextValuablePath] -  DateTime::Now(true)).TotalMinutes() < 5.0f)
                break;
        }
        // PathGenerator(State.Satellites[State.TLEref][State.TLEnow]);

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        State.Engage = true;
        pthread_create(&p_thread[1], NULL, Sattracker, NULL);
        this->callback(SGS_CONMOD_AUTO, "Start Engaging.");

        State.Doppler = true;
        pthread_create(&p_thread[7], NULL, Doppler, NULL);
        this->callback(SGS_CONMOD_AUTO, "Start Doppler Correction.");

        //Task
        //Check Beacon Received
        while(this->state && State.DoBeaconCheck)
        {
            this->BeaconCheck();
        }

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        if(State.AMPmode)
        {
            while((State.Satellites[State.TLEref][State.TLEnow]->_nextlos[NextValuablePath] - DateTime::Now(false)).TotalMinutes() > 15.0f)
                continue;
            if(State.AmpTime > 0)
            {
                this->callback(SGS_CONMOD_AUTOERR, "Cannot Turn on AMP Now. Wati until the timer goes on.");
                this->callback(SGS_CONMOD_AUTOERR, "If you don't want it, restart the software.");
                this->callback(SGS_CONMOD_AUTOERR, "Remind the AMP would be turned off after the timer goes on");
                this->callback(SGS_CONMOD_AUTOERR, "even if you restart the software.");
                while(State.AmpTime > 0)
                    continue;
            }
                
            pid_t pid = fork();
            if (pid == -1) {
                this->callback(SGS_CONMOD_AUTOERR, "Failed to fork process");
            }
            else if (pid == 0) {
                // Child process
                execl("./amp/ampcontrol", "ampcontrol", NULL);
                this->callback(SGS_CONMOD_AUTO, "Forked Safe Amplifier Process.");
            }
            else
            {
                this->callback(SGS_CONMOD_AUTO, "Forked Safe Amplifier Process.(ON, Timer)");
                pthread_create(&p_thread[6], NULL, AmpTimer, NULL);
            }
        }
       

        //Ping Try
        while(this->state && BeaconAvailable && State.DoPing && NowOn(NextValuablePath))
        {
            this->PingTry();
        }

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        uint32_t now_total_rx_bytes = get_rx_bytes();

        //Send Watchdog
        if(this->state && BeaconAvailable && NowOn(NextValuablePath))
        {
            this->callback(SGS_CONMOD_AUTO, "Synchronizing Watchdog Counter GS100 - AX100");
            csp_transaction(CSP_PRIO_HIGH, setup->gs100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0;
            while(this->state)
            {
                this->Watchdog();
            }
        }

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        bool ulsigdone = false;
        //UL SIGNAL
        if(State.DoFTPDL || State.DoFTPUL && NowOn(NextValuablePath))
        {
            int NowBecCount = this->BeaconCounter;
            while(NowBecCount == this->BeaconCounter && this->state && NowOn(NextValuablePath))
                continue;
            uint32_t initial = get_rx_bytes();
            DateTime inittime = DateTime::Now(false);
            
            while(this->state && !ulsigdone)
            {
                this->UpSig();
            }
        }

        

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        //CMD
        if(this->state && State.DoCMD && NowOn(NextValuablePath))
        {
            this->FSWCMD();
        }

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        //FTP DL
        //Temporary condition : max_el > 30.0deg
        while(this->state && State.DoFTPDL && NowOn(NextValuablePath))
        {
            this->FTPDL();
        }

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        //FTP UL
        //Temporary condition : max_el > 30.0deg
        while(this->state && State.DoFTPUL && NowOn(NextValuablePath))
        {
            this->FTPUL();
        }
        

        if(!this->state)
        {
            this->callback(SGS_CONMOD_AUTO, "Autopilot Stopped by User.");
            return 0;
        }

        //Wait Until LOS and return multhreads.
        pthread_join(p_thread[6], NULL);
        pthread_join(p_thread[8], NULL);
        pthread_join(p_thread[4], NULL);

        while(this->state && NowOn(NextValuablePath))
            continue;
        State.Doppler = false;
        pthread_join(p_thread[7], NULL);
        this->callback(SGS_CONMOD_AUTO, "Stop Doppler Correction.");

        State.Engage = false;
        State.Doppler = false;
        pthread_join(p_thread[1], NULL);
        this->callback(SGS_CONMOD_AUTO, "Finish Engaging.");
        
        RotatorSetTo(0, 0);
        this->callback(SGS_CONMOD_AUTO, "Beacon Success : %d, Ping Success : %d CMD Success : %d", this->BeaconCounter, this->PingCounter, State.CMDCount);
        this->BeaconCounter = 0;
        this->PingCounter = 0;
        if(TLE_Autoupdate() < 0)
            this->callback(SGS_CONMOD_AUTOERR, "Failed to Update TLE. Continue without Update.");
    }
    return 0;
}

int Autopilot::TLE_Autoupdate()
{
    char NowTrackingNameBuffer[32];
    sprintf(NowTrackingNameBuffer, State.Satellites[State.TLEref][State.TLEref]->Name());
    this->callback(SGS_CONMOD_AUTO, "Update TLE...");
    for(int i = 0; i < sizeof(State.Satellites) / sizeof(SatelliteObject *); i++)
    {
        State.Satellites[State.TLEref][i]->use = false;
    }
    if(DownloadTLE(State.tleinfolistup[State.TLEref]))
    {
        State.TLEnow = false;
        
        this->callback(SGS_CONMOD_AUTO, "TLE \"%s\" Download Completed.", State.tleinfolistup[State.TLEref]->label);

        // pthread_create(&p_thread[15], NULL, &SatelliteInitialize,  &sgp4check);
        // SatelliteInitialize(NULL);
        
    }
    else
    {
        this->callback(SGS_CONMOD_AUTOERR, "TLE Download failed. Please check again.");
        return -1;
    }
    // pthread_join(p_thread[15], NULL);
    printf("Updated TLE.\n");
    sleep(3);
    pid_t pid_off = fork();

    if (pid_off == -1) {
        this->callback(SGS_CONMOD_AUTOERR, "Failed to fork process");
    }
    else if (pid_off == 0) {
        // Child process
        execl("./amp/ampoff", "ampoff", NULL);
    }
    else
    {
        this->callback(SGS_CONMOD_AUTO, "Forked Safe Amplifier Process. (OFF)");
    }
    State.AMPON = false;

    csp_transaction(CSP_PRIO_HIGH, setup->gs100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0;
    this->callback(SGS_CONMOD_AUTO, "Reset GS100 watchdog.");
    printf("Reset GS100 Watchdog.\n");
    sleep(3);
    int tlepopupindex = 0;
    
    printf("Search Samename Satellite.\n");

    State.tleallindex = sizeof(State.Satellites) / sizeof(SatelliteObject *);
    if((State.tleallindex = ReadTLELines_Errorhandling(State.tleinfolistup[State.TLEref]->local, NowTrackingNameBuffer,false, false)) <= 0)
        this->callback(SGS_CONMOD_AUTOERR, "Invalid type of TLE File. Please check again.");
    State.Display_TLE = true;
    
    sleep(3);
    State.Satellites[State.TLEref][State.TLEnow]->use = true;
    if(State.Satellites[State.TLEref][State.TLEnow]->cal == false)
        State.Satellites[State.TLEref][State.TLEnow]->Refresh(State.Satellites[State.TLEref][State.TLEnow]->tle, State.Satellites[State.TLEref][State.TLEnow]->obs, true, true);
    printf("Updated Propagator.\n");
    sleep(3);
    State.TLEnow = true;
    State.ModelRefreshRequired = true;
    printf("Model Refresh.\n");
    sleep(3);
    // PathGenerator(State.Satellites[State.TLEref][State.TLEnow]);
    State.Display_load = false;
    State.Display_TLE = false;
    printf("Finish Autopilot.\n");
    
}

