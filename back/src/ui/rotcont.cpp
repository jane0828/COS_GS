#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_RotatorWindow::SGS_RotatorWindow()
{
    this->_open = true;
}

SGS_RotatorWindow::~SGS_RotatorWindow()
{
    this->_open = false;
}

void SGS_RotatorWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}


void SGS_RotatorWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("SGS_RotatorWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(FontScale);
    ImGui::Text("Rotator Window");
    if (this->Engage == false)
    {
        if (ImGui::Button("Start Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            this->Engage = true;
            //pthread_create(&p_thread[1], NULL, Sattracker, NULL);
            console->AddLog(SGS_CONMOD_OK, "Start Engaging.");
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
        if (ImGui::Button("Stop Rotator Engaging", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            this->Engage = false;
            TRXWindow->Doppler = false;
            //pthread_join(p_thread[1], NULL);
            console->AddLog(SGS_CONMOD_OK, "Finish Engaging.");
            console->AddLog(SGS_CONMOD_LOG, "Beacon Success : %d, Ping Success : %d", this->_BeaconCounter, this->_Pingcounter);
            this->_BeaconCounter = 0;
            this->_Pingcounter = 0;
        }
        ImGui::PopStyleColor();
    }


    ImGui::Text("Rotator Manual Control");
    ImGui::Text("AZ : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_az", &this->ant.az_tag);
    ImGui::PopItemWidth();
    ImGui::Text("EL : ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputInt("##tg_el", &this->ant.el_tag);
    ImGui::PopItemWidth();
    if (ImGui::Button("Set", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        //RotatorSetTo(this->ant.az_tag, this->ant.el_tag);
        //memset(&ControlBuf,0,sizeof(ControlBuf));
    }
    ImGui::SameLine();
    if (ImGui::Button("Init", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        //RotatorSetTo(0, 0);
    }
    if (ImGui::BeginTable("##Rotspectable", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.8)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Band", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Azimuth", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Elevation", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();
        ImGui::TableNextColumn();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("UHF");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%d", this->ant.az_now);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%d", this->ant.el_now);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("S-band");
        ImGui::TableSetColumnIndex(1);
        // ImGui::Text("%d ��", this->ant.az_this->ant);
        ImGui::TableSetColumnIndex(2);
        // ImGui::Text("%d ��", this->ant.el_this->ant);
        ImGui::EndTable();
    }


    if (ImGui::Button("Rotator On", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
    {
        //init_rotator();
        //pthread_create(&p_thread[2], NULL, RotatorReadInfo, NULL);
    }
    ImGui::SameLine();
    if (ImGui::Button("Rotator OFF", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        //this->RotatorReading = false;
        //pthread_join(p_thread[2], NULL);
        // fin_rotator();
    }
    if (ImGui::Button("Rotator IO Buffer Free", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        //RotatorEnter(NULL);
    }
    ImGui::SetWindowFontScale(1.0f);

    ImGui::End();
}