#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_TRXParamWindow::SGS_TRXParamWindow()
{
    this->_open = true;
    this->param0 = new Ptable0;
    this->param1 = new Ptable1;
    this->param5 = new Ptable5;
    memset(this->param0, 0, sizeof(Ptable0));
    memset(this->param1, 0, sizeof(Ptable1));
    memset(this->param5, 0, sizeof(Ptable5));
}

SGS_TRXParamWindow::~SGS_TRXParamWindow()
{
    this->_open = false;
}

void SGS_TRXParamWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}


void SGS_TRXParamWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("SGS_TRXParamWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(FontScale);
    ImGui::BeginChild("##Ptable0", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y * 0.9), true, this->WindowFlag);
    ImGui::PushItemWidth(300);
    ImGui::Text("RSSI Offset        : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0rssi_offset", ImGuiDataType_S8, &this->param0->rssi_offset, NULL, NULL, "%d");
    ImGui::Text("Max Temperature    : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0max_temp", ImGuiDataType_S16, &this->param0->max_temp, NULL, NULL, " % d");
    ImGui::Text("bgndrssl_ema       : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0bgndrssl_ema", ImGuiDataType_Float, &this->param0->bgndrssl_ema);
    ImGui::Text("CSP Node           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0csp_node", ImGuiDataType_U8, &this->param0->csp_node, NULL, NULL, "%u");
    ImGui::Text("I2C                : ");
    ImGui::SameLine();
    ImGui::Checkbox("##0i2c_enable", &this->param0->i2c_en);
    ImGui::Text("CAN                : ");
    ImGui::SameLine();
    ImGui::Checkbox("##0can_enable", &this->param0->can_en);
    ImGui::Text("EXTPTT             : ");
    ImGui::SameLine();
    ImGui::Checkbox("##0extptt_enable", &this->param0->extptt_en);
    ImGui::SameLine();
    ImGui::InputScalar("##0max_temp", ImGuiDataType_S8, &this->param0->max_temp, NULL, NULL, "%d");
    ImGui::Text("I2C Address        : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0i2c_addr", ImGuiDataType_U8, &this->param0->i2c_addr, NULL, NULL, "%u");
    ImGui::Text("I2C Frequency      : ");
    ImGui::SameLine();
    ImGui::InputScalar("kHz##0i2c_khz", ImGuiDataType_U16, &this->param0->i2c_khz, NULL, NULL, "%u");
    ImGui::Text("CAN Frequency      : ");
    ImGui::SameLine();
    ImGui::InputScalar("kHz##0can_khz", ImGuiDataType_U16, &this->param0->can_khz, NULL, NULL, "%u");
    ImGui::Text("Reboot in          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0reboot_in", ImGuiDataType_U16, &this->param0->reboot_in, NULL, NULL, "%u");
    ImGui::Text("TX Inhibit         : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0tx_inhibit", ImGuiDataType_U32, &this->param0->tx_inhibit, NULL, NULL, "%u");
    ImGui::Text("Store Log          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0log_store", ImGuiDataType_U8, &this->param0->log_store, NULL, NULL, "%u");
    ImGui::Text("TX Power           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##0tx_power", ImGuiDataType_U8, &this->param0->tx_pwr, NULL, NULL, "%d");
    ImGui::Text("Maximum TX Time    : ");
    ImGui::SameLine();
    ImGui::InputScalar("second##0max_tx_time", ImGuiDataType_U16, &this->param0->max_tx_time, NULL, NULL, "%u");
    ImGui::Text("Maximum IDLE Time  : ");
    ImGui::SameLine();
    ImGui::InputScalar("second##0max_idle_time", ImGuiDataType_U16, &this->param0->max_idle_time, NULL, NULL, "%u");
    ImGui::Text("CSP Routing Table  : ");
    ImGui::SameLine();
    ImGui::InputText("##0csp_rtable", this->param0->csp_rtable, sizeof(this->param0->csp_rtable));
    ImGui::PopItemWidth();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Ptable1", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y * 0.9), true, this->WindowFlag);
    ImGui::PushItemWidth(300);
    ImGui::Text("Rx Frequency       : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1freq", ImGuiDataType_U32, &this->param1->freq, NULL, NULL, "%u");
    ImGui::Text("BaudRate           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1baud", ImGuiDataType_U32, &this->param1->baud, NULL, NULL, "%u");
    ImGui::Text("modindex           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1modeindex", ImGuiDataType_Float, &this->param1->modindex, NULL, NULL, "%f");
    ImGui::Text("Guard              : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1guard", ImGuiDataType_U16, &this->param1->guard, NULL, NULL, "%u");
    ImGui::Text("pllrang            : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1pllrang", ImGuiDataType_U8, &this->param1->pllrang, NULL, NULL, "%u");
    ImGui::Text("mode               : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1mode", ImGuiDataType_U8, &this->param1->mode, NULL, NULL, "%u");
    ImGui::Text("csp_hmac           : ");
    ImGui::SameLine();
    ImGui::Checkbox("##1csp_hmac", &this->param1->csp_hmac);
    ImGui::Text("csp_rs             : ");
    ImGui::SameLine();
    ImGui::Checkbox("##1csp_rs", &this->param1->csp_rs);
    ImGui::Text("csp_crc            : ");
    ImGui::SameLine();
    ImGui::Checkbox("##1csp_crc", &this->param1->csp_crc);
    ImGui::Text("csp_rand           : ");
    ImGui::SameLine();
    ImGui::Checkbox("##1csp_rand", &this->param1->csp_rand);
    ImGui::Text("AX25 Call          : ");
    ImGui::SameLine();
    ImGui::InputText("##1ax25_call", this->param1->ax25_call, sizeof(this->param1->ax25_call));
    ImGui::Text("Bandwidth          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1bw", ImGuiDataType_U32, &this->param1->bw, NULL, NULL, "%u");
    ImGui::Text("AFC Range          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##1afcrange", ImGuiDataType_S32, &this->param1->afcrange, NULL, NULL, "%d");
    ImGui::PopItemWidth();
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Ptable5", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.9), true, this->WindowFlag);
    ImGui::PushItemWidth(300);
    ImGui::Text("Tx Frequency       : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5freq", ImGuiDataType_U32, &this->param5->freq, NULL, NULL, "%u");
    ImGui::Text("BaudRate           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5baud", ImGuiDataType_U32, &this->param5->baud, NULL, NULL, "%u");
    ImGui::Text("modindex           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5modeindex", ImGuiDataType_Float, &this->param5->modindex, NULL, NULL, "%f");
    ImGui::Text("Guard              : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5guard", ImGuiDataType_U16, &this->param5->guard, NULL, NULL, "%u");
    ImGui::Text("pllrang            : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5pllrang", ImGuiDataType_U8, &this->param5->pllrang, NULL, NULL, "%u");
    ImGui::Text("mode               : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5mode", ImGuiDataType_U8, &this->param5->mode, NULL, NULL, "%u");
    ImGui::Text("csp_hmac           : ");
    ImGui::SameLine();
    ImGui::Checkbox("##5csp_hmac", &this->param5->csp_hmac);
    ImGui::Text("csp_rs             : ");
    ImGui::SameLine();
    ImGui::Checkbox("##5csp_rs", &this->param5->csp_rs);
    ImGui::Text("csp_crc            : ");
    ImGui::SameLine();
    ImGui::Checkbox("##5csp_crc", &this->param5->csp_crc);
    ImGui::Text("csp_rand           : ");
    ImGui::SameLine();
    ImGui::Checkbox("##5csp_rand", &this->param5->csp_rand);
    ImGui::Text("AX25 Call          : ");
    ImGui::SameLine();
    ImGui::InputText("##5ax25_call", this->param5->ax25_call, sizeof(this->param5->ax25_call));
    ImGui::Text("preamblen          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5preamblen", ImGuiDataType_U8, &this->param5->preamblen, NULL, NULL, "%u");
    ImGui::Text("preambflags        : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5preambflags", ImGuiDataType_U8, &this->param5->preambflags, NULL, NULL, "%u");
    ImGui::Text("intfrmlen          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5intfrmlen", ImGuiDataType_U8, &this->param5->intfrmlen, NULL, NULL, "%u");
    ImGui::Text("intfrmflags        : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5intfrmflags", ImGuiDataType_U8, &this->param5->intfrmflags, NULL, NULL, "%u");
    ImGui::Text("RSSI busy          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5rssibusy", ImGuiDataType_S16, &this->param5->rssibusy, NULL, NULL, "%d");
    ImGui::Text("kup delay          : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5kup_delay", ImGuiDataType_U8, &this->param5->kup_delay, NULL, NULL, "%u");
    ImGui::Text("PA Level           : ");
    ImGui::SameLine();
    ImGui::InputScalar("##5pa_level", ImGuiDataType_S16, &this->param5->pa_level, NULL, NULL, "%d");
    ImGui::PopItemWidth();
    ImGui::EndChild();

    ImGui::BeginChild("##Button0", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y), false, this->WindowFlag);
    if (ImGui::Button("Update##p0", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    //pthread_create(&p_thread[0], NULL, &Load_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save##p0", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    //pthread_create(&p_thread[0], NULL, &Save_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save to FRAM##p0", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Button1", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y), false, this->WindowFlag);
    if (ImGui::Button("Update##p1", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    //pthread_create(&p_thread[0], NULL, &Load_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save##p1", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    //pthread_create(&p_thread[0], NULL, &Save_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save to FRAM##p1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##Button5", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, this->WindowFlag);
    if (ImGui::Button("Update##p5", ImVec2(ImGui::GetContentRegionAvail().x * 0.33, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
        //pthread_create(&p_thread[0], NULL, &Load_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save##p5", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
        //pthread_create(&p_thread[0], NULL, &Save_Paramtable5, NULL);
    ImGui::SameLine();
    if (ImGui::Button("Save to FRAM##p5", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        usleep(10);
    }
    ImGui::EndChild();
}