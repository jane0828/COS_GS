#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_TRXWindow::SGS_TRXWindow()
{
    this->_open = true;
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
        //console->AddLog(SGS_CONMOD_LOG, "Set Delay Spec : Default : %u, Guard : %u, Queue : %u", setup->default_timeout, setup->guard_delay, setup->queue_delay);
    }
    ImGui::EndChild();
    ImGui::End();
}

void SGS_TLMWindow::StateWindowColumnManager(const char * info)
{
    ImGui::TableNextColumn();
    ImGui::Text(info);
    ImGui::TableNextColumn();
}



void SGS_TLMWindow::Render_Beacon(float fontscale)
{
            
    ImGui::Begin("Beacon GUI", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(fontscale);

    ImGui::BeginTabBar("##InfoDesk");
    if(ImGui::BeginTabItem("Beacon"))
    {
        if (ImGui::BeginTable("##BeaconTables", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("Parameter",           ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Data",       ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableHeadersRow();
            ImGui::TableNextColumn();
            ImGui::Text("Call Sign");
            ImGui::TableNextColumn();
            memcpy(callsignbuf, beacon->CallSign, sizeof(beacon->CallSign));
            callsignbuf[6] = 0;
            // sprintf(callsignbuf, "%s\r\r\r\r", beacon->CallSign);
            ImGui::Text(callsignbuf);
            StateWindowColumnManager("Message ID");
            ImGui::Text("%X", csp_ntoh16(beacon->CCMessage_ID));
            StateWindowColumnManager("Sequence");
            ImGui::Text("%X", csp_ntoh16(beacon->CCSequence));
            StateWindowColumnManager("Length");
            ImGui::Text("%" PRIu16 , csp_ntoh16(beacon->CCLength) + 7);
            StateWindowColumnManager("Time Code");
            ImGui::Text("%X %X %X %X %X %X", beacon->CCTime_code[0], beacon->CCTime_code[1], beacon->CCTime_code[2], beacon->CCTime_code[3], beacon->CCTime_code[4], beacon->CCTime_code[5]);
            StateWindowColumnManager("Spacecraft Time");
            ImGui::Text("%" PRIu32 , beacon->FMSpaceTime);
            StateWindowColumnManager("Reboot Count");
            ImGui::Text("%" PRIu16 , beacon->FMRebootCount);
            StateWindowColumnManager("Current Mode");
            ImGui::Text("%" PRIu8 , beacon->FMCurMode);
            StateWindowColumnManager("Current Submode");
            ImGui::Text("%" PRIu8 , beacon->FmCurSubMode);
            StateWindowColumnManager("Previous Mode");
            ImGui::Text("%" PRIu8 , beacon->FMPreviousMode);
            StateWindowColumnManager("Previous SubMode");
            ImGui::Text("%" PRIu8 , beacon->FMPreviousSubMode);
            StateWindowColumnManager("Mode");
            ImGui::Text("%" PRIu8 , beacon->SYMode);
            StateWindowColumnManager("Reset Cause");
            ImGui::Text("%" PRIu8 , beacon->SYResetCause);
            StateWindowColumnManager("Reset Count");
            ImGui::Text("%" PRIu16 , beacon->SYResetcount);
            StateWindowColumnManager("Battery Status");
            ImGui::Text("%X", beacon->BAtteryStatus);
            StateWindowColumnManager("Battery In Voltage");
            ImGui::Text("%" PRId16 , beacon->BAtteryInV);
            StateWindowColumnManager("Battery In Current");
            ImGui::Text("%" PRId16 , beacon->BAtteryInI);
            StateWindowColumnManager("Battery Temperature");
            ImGui::Text("%" PRId16 , beacon->BAtteryTemp);
            StateWindowColumnManager("Mboard MPPT In V");
            ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 , beacon->MOMPPTInV[0], beacon->MOMPPTInV[1], beacon->MOMPPTInV[2]);
            StateWindowColumnManager("Mboard MPPT In I");
            ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 , beacon->MOMPPTInI[0], beacon->MOMPPTInI[1], beacon->MOMPPTInI[2]);
            StateWindowColumnManager("Mboard Channel I 3V");
            ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 , beacon->MOChancur3V[0], beacon->MOChancur3V[1], beacon->MOChancur3V[2], beacon->MOChancur3V[3]);
            StateWindowColumnManager("Mboard Channel I 5V");
            ImGui::Text("%" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 , beacon->MOChancur5V[0], beacon->MOChancur5V[1], beacon->MOChancur5V[2], beacon->MOChancur5V[3]);
            StateWindowColumnManager("Mboared Supply V");
            ImGui::Text("%" PRId16 , beacon->MOBoardSupV);
            StateWindowColumnManager("Mboard Temperature");
            ImGui::Text("%" PRId16 , beacon->MOBoardTemp);
            StateWindowColumnManager("Dboard MPPT In V");
            ImGui::Text("%" PRId16 " %" PRId16 , beacon->DAMPPTInV[0], beacon->DAMPPTInV[1]);
            StateWindowColumnManager("Dboard MPPT In I");
            ImGui::Text("%" PRId16 " %" PRId16 , beacon->DAMPPTInI[0], beacon->DAMPPTInI[1]);
            StateWindowColumnManager("Dboard Channel I 5V");
            ImGui::Text("%" PRId16 , beacon->DAChancur5V);
            StateWindowColumnManager("Dboard Channel I 12V");
            ImGui::Text("%" PRId16 , beacon->DAChancur12V);
            StateWindowColumnManager("Temperature");
            ImGui::Text("%.2f", beacon->IMTemp);
            StateWindowColumnManager("Angular Velocity");
            ImGui::Text("%.2f %.2f %.2f", beacon->IMAngV[0], beacon->IMAngV[1], beacon->IMAngV[2]);
            StateWindowColumnManager("RW Error Count");
            ImGui::Text("%" PRIu16 , beacon->IMRWErrcount);
            StateWindowColumnManager("Total RX Bytes");
            ImGui::Text("%" PRIu32 , beacon->UXTotRXByte);
            StateWindowColumnManager("Boot Count");
            ImGui::Text("%" PRIu16 , beacon->UXBootCount);
            StateWindowColumnManager("Active Conf");
            ImGui::Text("%" PRIu8 , beacon->UXActiveConf);

            ImGui::EndTable();
        }  

        ImGui::EndTabItem();
        ImGui::SetWindowFontScale(1.0 * fontscale);
    }
    if(ImGui::BeginTabItem("Telemetry"))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        ImGui::Text("Total RX Bytes(Beacon) : %" PRIu32 , get_rx_bytes());
        ImGui::SetWindowFontScale(0.6 * fontscale);
        ImGui::PopStyleColor();

        ImGui::SetWindowFontScale(1.0 * fontscale);
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Shell"))
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        ImGui::Text("Total RX Bytes(Beacon) : %" PRIu32 , get_rx_bytes());
        ImGui::SetWindowFontScale(0.6 * fontscale);
        ImGui::PopStyleColor();
        
        ImGui::BeginChild("##AX100_shell_typeselect", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.8), true);
        
        ImGui::Text("RSSI busy        ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct4_rssi_busy", ImGuiDataType_S16, &remote_rssibusy, NULL, NULL, "%d");
        if(ImGui::Button("Load##ax100_direct4_rssi_busy", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, &remote_rssibusy) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. rssi_busy : %" PRIu8 , rparam0.csp_node);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct4_rssi_busy", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_int16(setup->ax100_node, 5, 0x0048, 0xB00B, setup->default_timeout, remote_rssibusy) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        if(get_rx_bytes() == 0)
            ImGui::Text("Total RX Bytes   ");
        else
            ImGui::Text("Total RX bytes : %" PRIu8 "", get_rx_bytes());
        if(ImGui::Button("Load##ax100_direct4_total_rx_bytes", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 4, 0x003C, 0xB00B, setup->default_timeout, get_rx_bytes_address()) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Total RX Bytes : %u", get_rx_bytes());
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        ImGui::Text("CAN enable       ");
        ImGui::SameLine();
        ImGui::Checkbox("##ax100_direct0_can_en", &rparam0.can_en);
        if(ImGui::Button("Load##ax100_direct0_can_en", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (int8_t *)&rparam0.can_en) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. can_en : %s", rparam0.can_en ? "true" : "false");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }.0
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_can_en", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_int8(setup->ax100_node, 0, rparam0.can_en_addr, 0xB00B, setup->default_timeout, (bool)rparam0.can_en) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        ImGui::Text("TX inhibit       ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct0_tx_inhibit", ImGuiDataType_U32, &rparam0.tx_inhibit, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct0_tx_inhibit", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_get_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, &rparam0.tx_inhibit) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. tx_inhibit : %" PRIu32 , rparam0.tx_inhibit);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_tx_inhibit", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            sleep(0.3);
            if(gs_rparam_set_uint32(setup->ax100_node, 0, rparam0.tx_inhibit_addr, 0xB00B, setup->default_timeout, rparam0.tx_inhibit) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();
        
        ImGui::Text("TX power         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct0_tx_pwr", ImGuiDataType_U8, &rparam0.tx_pwr, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct0_tx_pwr", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, &rparam0.tx_pwr) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. tx_pwr : %" PRIu8 , rparam0.tx_pwr);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_tx_pwr", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint8(setup->ax100_node, 0, rparam0.tx_pwr_addr, 0xB00B, setup->default_timeout, rparam0.tx_pwr) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        ImGui::Text("CSP rtable       ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##ax100_direct0_csp_rtable", rparam0.csp_rtable, sizeof(rparam0.csp_rtable));
        if(ImGui::Button("Load##ax100_direct0_csp_rtable", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. csp_rtable : %s", rparam0.csp_rtable);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct0_csp_rtable", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_string(setup->ax100_node, 0, rparam0.csp_rtable_addr, 0xB00B, setup->default_timeout, rparam0.csp_rtable, sizeof(rparam0.csp_rtable)) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        if(remote_activeconf > 3)
            ImGui::Text("Active Conf      ");
        else
            ImGui::Text("Active Conf : %" PRIu8 "", remote_activeconf);
        if(ImGui::Button("Load##ax100_direct0_active_conf", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint8(setup->ax100_node, 4, 0x0018, 0xB00B, setup->default_timeout, &remote_activeconf) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. Active Conf : %" PRId8 , remote_activeconf);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();

        if(remote_lastrssi > 3)
            ImGui::Text("Last RSSI        ");
        else
            ImGui::Text("Last RSSI : %" PRId16 "", remote_lastrssi);
        if(ImGui::Button("Load##ax100_direct0_last_rssi", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_int16(setup->ax100_node, 4, 0x0004, 0xB00B, setup->default_timeout, &remote_lastrssi) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. Last RSSI : %" PRId16 , remote_lastrssi);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
            
        }
        ImGui::Spacing();

        ImGui::Text("RX guard         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct1_rx_guard", ImGuiDataType_U16, &rparam1.guard, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct1_rx_guard", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, &rparam1.guard) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. rx_guard : %" PRIu16 , rparam1.guard);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct1_rx_guard", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint16(setup->ax100_node, 1, rparam1.guard_addr, 0xB00B, setup->default_timeout, rparam1.guard) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();
        
        ImGui::Text("TX guard         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct5_tx_guard", ImGuiDataType_U16, &rparam5.guard, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct5_tx_guard", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, &rparam5.guard) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. tx_guard : %" PRIu16 , rparam5.guard);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct5_tx_guard", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint16(setup->ax100_node, 5, rparam5.guard_addr, 0xB00B, setup->default_timeout, rparam5.guard) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        ImGui::Text("RX Baudrate         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct1_rxbaud", ImGuiDataType_U32, &rparam1.baud, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct1_rx_baud", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, &rparam1.baud) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. rx_baud : %" PRIu16 , rparam1.baud);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct1_rxbaud", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint32(setup->ax100_node, 1, rparam1.baud_addr, 0xB00B, setup->default_timeout, rparam1.baud) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        ImGui::Text("TX Baudrate         ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputScalar("##ax100_direct5_txbaud", ImGuiDataType_U32, &rparam5.baud, NULL, NULL, "%u");
        if(ImGui::Button("Load##ax100_direct5_txbaud", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_get_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, &rparam5.baud) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK. tx_baud : %" PRIu16 , rparam5.baud);
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Save##ax100_direct5_txbaud", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            if(gs_rparam_set_uint32(setup->ax100_node, 5, rparam5.baud_addr, 0xB00B, setup->default_timeout, rparam5.baud) == GS_OK)
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::Spacing();

        if(ImGui::Button("Save to FRAM##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y)))
        {
            if(gs_rparam_save(setup->ax100_node, setup->default_timeout, 0, 0) == GS_OK)
            {
                gs_rparam_save(setup->ax100_node, setup->default_timeout, 1, 1);
                gs_rparam_save(setup->ax100_node, setup->default_timeout, 5, 5);
                set_guard_spec_micsec(param5.baud, param1.baud, param5.guard, param1.guard, rparam5.guard, rparam1.guard);
                console->AddLog(SGS_CONMOD_OK, "Remote Param Control OK.");
            }
                
            else
                console->AddLog(SGS_CONMOD_ERR, "Cannot Load Remote Parameter.");
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset Watchdog##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
        {
            if(csp_transaction(CSP_PRIO_HIGH, setup->ax100_node, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0) > 0)
                console->AddLog(SGS_CONMOD_LOG, "Remote Param Control OK.(No Reply)");
            else
                console->AddLog(SGS_CONMOD_ERR, "Remote Param Control Failed.(No Reply)");
        }
        ImGui::SameLine();
        if(ImGui::Button("Reboot AX100##ax100_direct", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
        {
            csp_reboot(setup->ax100_node);
            console->AddLog(SGS_CONMOD_LOG, "Remote Param Control OK.(No Reply)");
        }
        ImGui::EndChild();

        ImGui::BeginChild("##MEOW_shell_typeselect", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
        ImGui::RadioButton("System CMD   ##shelltype_syscmd", &State.Shelltype, TYPE_SYSCMD);
        ImGui::SameLine();
        ImGui::RadioButton("Redirect CMD ##shelltype_rmdir ", &State.Shelltype, TYPE_SYSCMD_REDIR);
        ImGui::SameLine();
        ImGui::RadioButton("Remove Dir   ##shelltype_rmdirp", &State.Shelltype, TYPE_SET_REDIR_PATH);
        ImGui::SameLine();
        ImGui::RadioButton("DS Cleanup   ##shelltype_cleand", &State.Shelltype, TYPE_DS_CLEANUP);

        ImGui::RadioButton("HK Request   ##shelltype_requhk", &State.Shelltype, TYPE_HK_REQUEST);
        ImGui::SameLine();
        ImGui::RadioButton("Reserved     ##shelltype_reserv", &State.Shelltype, TYPE_RESERVED);
        ImGui::SameLine();
        ImGui::RadioButton("KILL         ##shelltype_killin", &State.Shelltype, TYPE_KILL);

        if(State.Shelltype != (int)this->shellcmd->type)
        {
            this->shellcmd->type = (uint8_t)State.Shelltype;
            memset(&this->shellcmd->required, 0, sizeof(this->shellcmd->required));
        }

        switch (State.Shelltype)
        {
        case TYPE_SYSCMD:
        {   
            ImGui::Text("Out U8   ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputScalar("##shell_syscmd_redirout", ImGuiDataType_U8, &(this->shellcmd->required.syscmd.redir_out), NULL, NULL, "%u");
            ImGui::Text("CMD      ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd", this->shellcmd->required.syscmd.cmd, sizeof(this->shellcmd->required.syscmd.cmd));
            break;
        }
        case TYPE_SYSCMD_REDIR:
        {   
            ImGui::Text("CMD      ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd1", this->shellcmd->required.syscmd_redir.cmd, sizeof(this->shellcmd->required.syscmd_redir.cmd));
            ImGui::Text("Redirect ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd2", this->shellcmd->required.syscmd_redir.redir_path, sizeof(this->shellcmd->required.syscmd_redir.redir_path));
            break;
        }
        case TYPE_SET_REDIR_PATH:
        {   
            ImGui::Text("Path     ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputText("##shell_syscmd_cmd4", this->shellcmd->required.set_redir.redir_path, sizeof(this->shellcmd->required.set_redir.redir_path));
            ImGui::Text("");
            break;
        }
        case TYPE_DS_CLEANUP:
        {   
            ImGui::Text("Path     ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputScalar("##shell_syscmd_redirout5", ImGuiDataType_U8, &(this->shellcmd->required.ds_cleanup.opt), NULL, NULL, "%u");
            ImGui::Text("");
            break;
        }
        
        default:
        {
            ImGui::Text("ERROR!");
            break;
        }
        }

        if(ImGui::Button("Send##SendShellcmd", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3)))
        {
            if(State.Debugmode)
            {
                printf("Input Shell Data : ");
                for(int i = 0; i < sizeof(cmd_packet_t); i++)
                    printf("%u\t", ((uint8_t *)this->shellcmd)[i]);
                printf("\n");
            }
            pthread_create(&p_thread[4], NULL, Direct_Shell, (void *)this->shellcmd); 
        }
        ImGui::EndChild();
        ImGui::SetWindowFontScale(1.0 * fontscale);
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
    ImGui::SetWindowFontScale(1.0);

    ImGui::End();
}
