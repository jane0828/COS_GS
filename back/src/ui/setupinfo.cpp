#ifndef _SCL_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_SetupInfoWindow::SGS_SetupInfoWindow()
{
    this->_open = true;
    memset(this->_Transciever_devname, 0, sizeof(this->_Transciever_devname));
    memset(this->_Rotator_devname, 0, sizeof(this->_Rotator_devname));
    memset(this->_Switch_devname, 0, sizeof(this->_Switch_devname));
    memset(this->_DebugFilePath, 0, sizeof(this->_DebugFilePath));
    memset(this->_S_Username, 0, sizeof(this->_S_Username));
    memset(this->_S_Address, 0, sizeof(this->_S_Address));
    memset(this->_S_passwd, 0, sizeof(this->_S_passwd));
    memset(this->_S_passwd, 0, sizeof(this->_S_passwd));
}

SGS_SetupInfoWindow::~SGS_SetupInfoWindow()
{
    this->_open = false;
}

void SGS_SetupInfoWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{

    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}

void SGS_SetupInfoWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("Setup##SGS_SetupInfoWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);
    ImGui::BeginChild("##setuptitle_transceiver", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5), false, this->WindowFlag);
    ImGui::Text("Transceiver");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##setuptitle_router", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5), false, this->WindowFlag);
    ImGui::Text("Route Table");
    ImGui::EndChild();
    
    ImGui::BeginChild("##setup_transceiver", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 11), true, this->WindowFlag);
    ImGui::Text("Device Name");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##GS100_name", &this->_Transciever_devname[0], sizeof(this->_Transciever_devname));
    ImGui::Text("Default Frequency(Hz)");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputDouble("##GS100_freq", &this->_default_freq);
    ImGui::Text("Baudrate");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##GS100_baud", ImGuiDataType_U32, &this->_Transceiver_baud, NULL, NULL, "%u");
    ImGui::Text("Delay(ms)");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##GS100_delay", ImGuiDataType_U32, &this->_default_timeout, NULL, NULL, "%u");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##setup_node", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 11), true, this->WindowFlag);
    ImGui::Text("GS100 node");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##gs100_node", ImGuiDataType_U8, &this->_gs100_node, NULL, NULL, "%u");
    ImGui::Text("GS PC node");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##gspc_node", ImGuiDataType_U8, &this->_kiss_node, NULL, NULL, "%u");
    ImGui::Text("AX100 node");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##ax100_node", ImGuiDataType_U8, &this->_ax100_node, NULL, NULL, "%u");
    ImGui::Text("OBC   node");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##obc_node", ImGuiDataType_U8, &this->_obc_node, NULL, NULL, "%u");
    ImGui::EndChild();

    ImGui::BeginChild("##setuptitle_rotator", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5), false, this->WindowFlag);
    ImGui::Text("Rotator Controller");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##setuptitle_switch", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5), false, this->WindowFlag);
    ImGui::Text("Switch Box");
    ImGui::EndChild();

    ImGui::BeginChild("##setup_rotator", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 6.0), true, this->WindowFlag);
    ImGui::Text("Device Name");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##GS232B_name", &this->_Rotator_devname[0], sizeof(this->_Rotator_devname));
    ImGui::Text("Baudrate");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##GS232B_baud", ImGuiDataType_U32, &this->_Rotator_baud, NULL, NULL, "%u");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##setup_switch", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 6.0), true, this->WindowFlag);
    ImGui::Text("Device Name");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##KTA223_name", &this->_Switch_devname[0], sizeof(this->_Switch_devname));
    ImGui::Text("Baudrate");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##KTA223_baud", ImGuiDataType_U32, &this->_Switch_baud, NULL, NULL, "%u");
    ImGui::EndChild();

    ImGui::Text("S-Band");
    ImGui::SameLine();
    ImGui::Checkbox("##s_band_use_state_check", &this->_SbandUse);
    if (this->_SbandUse)
    {
        ImGui::BeginChild("##setup_s_band", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 7.2), true, this->WindowFlag);
        ImGui::Text("Username  ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##S-BandUsername", &this->_S_Username[0], sizeof(this->_S_Username));
        ImGui::Text("IP Address");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##S-BandIP", &this->_S_Address[0], sizeof(this->_S_Address));
        ImGui::Text("Password  ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("##S-BandPW", &this->_S_passwd[0], sizeof(this->_S_passwd));
        ImGui::EndChild();
    }
    if (ImGui::Button("Start", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetFontSize() * 1.5)))
    {
        //State.InitializeRequired = true;
        //set_serial_spec_micsec(this->_Transceiver_baud, this->_Switch_baud, 1000000, this->_queue_delay, this->_gs100_node, this->_default_timeout, this->_guard_delay);

    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Watchdog", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        //ResetWatchdog();
    }
    ImGui::SameLine();
    if (ImGui::Button("Quit", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {

    }
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}