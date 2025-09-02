#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif


SGS_TRXWindow::SGS_TRXWindow()
{
    this->_open = true;
    this->shellcmd = new cmd_packet_t;
}

SGS_TRXWindow::~SGS_TRXWindow()
{
    this->_open = false;
}

void SGS_TRXWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{

    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}


void SGS_TRXWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("SGS_TRXWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);
    ImGui::Text("Center Frequency");
    ImGui::InputDouble("##centerfrequency(Hz)", &this->_default_freq);
    ImGui::SameLine();
    if (ImGui::Button("Set##SetDefaultFrequency", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        this->_default_freq = 436.5;

        TRXParamWindow->param1->freq = this->_default_freq;
        TRXParamWindow->param5->freq = this->_default_freq;
        TRXParamWindow->param1->baud = 4800;
        TRXParamWindow->param5->baud = 4800;
    }
    if (ImGui::BeginTable("##TRXspectable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.8)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("RX", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("TX", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%f MHz", ((float)TRXParamWindow->param1->freq) / 1000000);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%f MHz", ((float)TRXParamWindow->param5->freq) / 1000000);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%" PRIu32 " bps", TRXParamWindow->param1->baud);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%" PRIu32 " bps", TRXParamWindow->param5->baud);
        ImGui::EndTable();
    }
    ImGui::Text("Last RSSI(dBm) : ");
    //ImGui::SameLine();
    //ImGui::Text("%" PRId16 "", Return_RSSI());
    if (ImGui::Button("Set to RX", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        //switch_to_rx(setup->ax100_node);
    }
    ImGui::SameLine();
    if (ImGui::Button("Set to TX", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        //switch_to_tx(setup->ax100_node);
    }

    if (this->Doppler == false)
    {

        if (ImGui::Button("Start Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if (this->GS100_Connection)
            {
                this->Doppler = true;
                //pthread_create(&p_thread[7], NULL, Doppler, NULL);
                console->AddLog(SGS_CONMOD_OK, "Start Doppler Correction.");
            }
            else
            {
                console->AddLog(SGS_CONMOD_ERR, "GS100 Connection Error - Cannot Start Doppler Correction.");
            }

        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Doppler Correction", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            this->Doppler = false;
            //pthread_join(p_thread[7], NULL);
        }
        ImGui::PopStyleColor();
    }
    ImGui::Text("Ping Size  ");
    ImGui::SameLine();
    ImGui::InputScalar("##pingsize", ImGuiDataType_U16, &SetupWindow->_pingsize, NULL, NULL, "%u");
    ImGui::BeginChild("##DelaySettings", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, this->WindowFlag);
    ImGui::Text("Default Delay");
    ImGui::SameLine();
    ImGui::InputScalar("##defaulttimeout", ImGuiDataType_U32, &SetupWindow->_default_timeout, NULL, NULL, "%u");
    ImGui::Text("Guard Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##guarddelay", ImGuiDataType_U32, &SetupWindow->_guard_delay, NULL, NULL, "%u");
    ImGui::Text("Queue Delay  ");
    ImGui::SameLine();
    ImGui::InputScalar("##queuedelay", ImGuiDataType_U32, &SetupWindow->_queue_delay, NULL, NULL, "%u");
    if (ImGui::Button("Apply##apply", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        //set_serial_spec_micsec(setup->Transceiver_baud, setup->Switch_baud, 1000000, setup->queue_delay, setup->gs100_node, setup->default_timeout, setup->guard_delay);
        //console.AddLog("Set Delay Spec : Default : %u, Guard : %u, Queue : %u", setup->default_timeout, setup->guard_delay, setup->queue_delay);
    }
    ImGui::EndChild();

    if(!State.AMPON)
    {
        if(State.AmpTime > 0)
        {
            strftime(this->AmpTimeBuf, sizeof(this->AmpTimeBuf), "Disabled  %M:%S", State.AmpTM);
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::Button(this->AmpTimeBuf, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5));
            ImGui::PopItemFlag();
        }
            
        else
        {
            sprintf(this->AmpTimeBuf, "AMP ON");
            if (ImGui::Button(this->AmpTimeBuf, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
            {
                pthread_join(p_thread[6], NULL);
                pid_t pid_on = fork();

                if (pid_on == -1) {
                    console->AddLog(SGS_CONMOD_ERR, "Failed to fork process");
                }
                else if (pid_on == 0) {
                    // Child process
                    execl("./amp/ampcontrol", "ampcontrol", NULL);
                    
                }
                else
                {
                    console->AddLog(SGS_CONMOD_OK, "Forked Safe Amplifier Process.(ON, Timer)");
                    pthread_create(&p_thread[6], NULL, AmpTimer, NULL);
                }
            }
        }
            
        
    }
    else
    {
        if(State.AmpTime > 0)
            strftime(this->AmpTimeBuf, sizeof(this->AmpTimeBuf), "AMP OFF %M:%S", State.AmpTM);
        else
            sprintf(this->AmpTimeBuf, "AMP OFF");
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button(this->AmpTimeBuf, ImVec2(ImGui::GetContentRegionAvail().x , ImGui::GetFontSize() * 1.5)))
        {
            pid_t pid_off = fork();

            if (pid_off == -1) {
                console->AddLog(SGS_CONMOD_ERR, "Failed to fork process");
            }
            else if (pid_off == 0) {
                // Child process
                execl("./amp/ampoff", "ampoff", NULL);
            }
            else
            {
                console->AddLog(SGS_CONMOD_OK, "Forked Safe Amplifier Process. (OFF)");
            }
            State.AMPON = false;
        }
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void * Doppler(void *) {
    int index = -2;
    uint32_t rxfreq, txfreq;
    double sol = 299792458;  // [m/s]
    double vel;
    double updated_freq;
    while(State.Doppler)
    {
        while(!State.RotatorReadReady)
            continue;
        if (!State.Engage) {
            console->AddLog(SGS_CONMOD_ERR, "You tried doppler correction with no engaging");
            State.Doppler = false;
            break;
        }
        if(index < -1)
        {
            console->AddLog(SGS_CONMOD_ERR, "Please load TLE or FDS. Stop Doppler Correction.");
            State.Doppler = false;
            break;
        }
        else
        {
            if(State.Satellites[State.TLEref][State.TLEnow] == NULL)
            {
                State.Doppler = false;
                break;
            }
            vel= 1 - (State.Satellites[State.TLEref][State.TLEnow]->topo.range_rate * 1000)/sol;
            updated_freq = vel * (setup->default_freq);
            
            rxfreq = uint32_t(static_cast<unsigned int>(updated_freq));
            txfreq = uint32_t(static_cast<unsigned int>((setup->default_freq) * 2 - updated_freq));
            console->AddLog(SGS_CONMOD_DBG, "TX : %u, RX : %u", txfreq, rxfreq);
            //Set freq
            SetRxFreq(rxfreq);
            SetTxFreq(txfreq);
            int8_t rx_table_id = 1;
            int8_t tx_table_id = 5;
            // Save2FRAM((void *)&rx_table_id);
            // Save2FRAM((void *)&tx_table_id);
            usleep(DOPPLER_TIMESTEP);
            UpdateFreq();
        }
    }
    return NULL;
};


void * AmpTimer(void *)
{
	State.AMPON = true;
    time_t targetTime = time(NULL) + 14 * 60; // Add 14 minutes in seconds
	time_t currentTime;
	while(!State.InitializeRequired)
	{
		currentTime = time(NULL);
		if(currentTime > targetTime)
		{
			State.AMPON = false;
			break;
		}
			
		State.AmpTime = targetTime - currentTime;
		State.AmpTM = gmtime(&State.AmpTime);

	}
	State.AMPON = false;
	return NULL;
}

