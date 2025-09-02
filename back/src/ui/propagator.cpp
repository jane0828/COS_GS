#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

#define RAD_TO_DEG      57.2958

void * TrackingSatellites(void * param)
{
    SGS_PropagatorWindow* prop = (SGS_PropagatorWindow *)param;
    while (prop->THREAD[0])
    {
        if(!prop)
            continue;
        if (!tle[prop->TLEref]->imported)
            return 0;
        for (int idx = 0; idx < sizeof(tle) / sizeof(SGS_TLE*); idx++)
        {
            for (int row = 0; row < sizeof(tle[prop->TLEref]->Satellites) / sizeof(SatelliteObject*); row++)
            {
                if ((tle[prop->TLEref]->Satellites[row] == NULL) || (tle[prop->TLEref]->Satellites[row]->use == false))
                    continue;
                else
                {
                    if (tle[prop->TLEref]->Satellites[row]->cal == false)
                    {
                        tle[prop->TLEref]->Satellites[row]->cal = true;
                        tle[prop->TLEref]->Satellites[row]->Refresh();
                    }
                    try
                    {
                        tle[prop->TLEref]->Satellites[row]->Update();
                    }
                    catch (const std::exception& e)
                    {
                        console->AddLog(SGS_CONMOD_ERR, "Unable to Track Satellite : %s", tle[prop->TLEref]->Satellites[row]->Name());
                        console->AddLog(SGS_CONMOD_ERR, e.what());
                    }
                }
            }
        }
        usleep(100);
    }
    return NULL;
}

SGS_PropagatorWindow::SGS_PropagatorWindow()
{
    this->_open = true;
    memset(this->THREAD, false, sizeof(this->THREAD));
    memset(this->HANDLES, NULL, sizeof(this->HANDLES));
}

SGS_PropagatorWindow::~SGS_PropagatorWindow()
{

}



void SGS_PropagatorWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}

void SGS_PropagatorWindow::Render()
{#include "SGS_include.h"

#define RAD_TO_DEG      57.2958

DWORD WINAPI TrackingSatellites(LPVOID param)
{
    SGS_PropagatorWindow* prop = PropagatorWindow;
    while (prop->THREAD[0])
    {
        if (!tle[prop->TLEref]->imported)
            return 0;
        for (int idx = 0; idx < sizeof(tle) / sizeof(SGS_TLE*); idx++)
        {
            for (int row = 0; row < sizeof(tle[prop->TLEref]->Satellites) / sizeof(SatelliteObject*); row++)
            {
                if ((tle[prop->TLEref]->Satellites[row] == NULL) || (tle[prop->TLEref]->Satellites[row]->use == false))
                    continue;
                else
                {
                    if (tle[prop->TLEref]->Satellites[row]->cal == false)
                    {
                        tle[prop->TLEref]->Satellites[row]->cal = true;
                        tle[prop->TLEref]->Satellites[row]->Refresh();
                    }
                    try
                    {
                        tle[prop->TLEref]->Satellites[row]->Update();
                    }
                    catch (const std::exception& e)
                    {
                        console->AddLog(SGS_CONMOD_ERR, "Unable to Track Satellite : %s", tle[prop->TLEref]->Satellites[row]->Name());
                        console->AddLog(SGS_CONMOD_ERR, e.what());
                    }
                }
            }
        }
        usleep(100);
    }
    return 0;
}

SGS_PropagatorWindow::SGS_PropagatorWindow()
{
    this->_open = true;
    memset(this->THREAD, false, sizeof(this->THREAD));
    memset(this->HANDLES, NULL, sizeof(this->HANDLES));
}

SGS_PropagatorWindow::~SGS_PropagatorWindow()
{

}



void SGS_PropagatorWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}

void SGS_PropagatorWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("Propagator##SGS_PropagatorWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);

    if (ImGui::BeginTable("##SatelliteListup", 12, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Next AOS", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Next LOS", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Max El.", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Azimuth", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Elevation", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Range", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Range Rate", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Latitude",  ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Longitude",  ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Altitude", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();

        // ImGui::TableNextColumn();
        for (int idx = 0; idx < sizeof(tle) / sizeof(SGS_TLE*); idx++)
        {
            if (!tle[idx])
                break;
            //ImGui::PushStyleColor(ImGuiCol_Text, tle[idx]->color);
            for (uint32_t row = 0; row < sizeof(tle[idx]->Satellites) / sizeof(SatelliteObject*); row++)
            {
                if (!tle[idx]->Satellites[row])
                    break;
                if (tle[idx]->Satellites[row]->use)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tle[idx]->Satellites[row]->Name());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d/%d %d:%d:%d", tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Month(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Day(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Hour(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Minute(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Second());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d/%d %d:%d:%d", tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Month(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Day(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Hour(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Minute(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Second());
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2f", tle[idx]->Satellites[row]->_max_elevation[0] * RAD_TO_DEG);
                    ImGui::TableNextColumn();

                    sprintf_s(this->_ButtonLabel_Info, "Info##trackingwindow%d", row);
                    if (ImGui::Button(this->_ButtonLabel_Info, ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
                    {
                        //State.Display_Satinfo = true;
                    }
                    //if (State.Display_Satinfo)
                    //    State.Display_Satinfo = popup_satinfo(idx, row);
                    ImGui::SameLine();
                    if (tle[idx]->trackidx == row && tle[idx]->imported)
                    {
                        sprintf_s(this->_ButtonLabel_Track, "  On");
                        ImGui::Text(this->_ButtonLabel_Track);
                    }
                    else
                    {
                        sprintf_s(this->_ButtonLabel_Track, "Track##trackingwindow%d", row);
                        if (ImGui::Button(this->_ButtonLabel_Track, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                        {
                            tle[idx]->trackidx = row;
                            // PathGenerator(tle[idx]->Satellites[row]);
                        }
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->topo.azimuth * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->topo.elevation * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm", tle[idx]->Satellites[row]->topo.range);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm/s", tle[idx]->Satellites[row]->topo.range_rate);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->geo.latitude * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->geo.longitude * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm", tle[idx]->Satellites[row]->geo.altitude);
                }

            }
            //ImGui::PopStyleColor();
        }

        ImGui::EndTable();
    }
    ImGui::End();

}
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("Propagator##SGS_PropagatorWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);

    if (ImGui::BeginTable("##SatelliteListup", 12, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Next AOS", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Next LOS", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Max El.", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Azimuth", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Elevation", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Range", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Range Rate", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Latitude",  ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Longitude",  ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Altitude", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();

        // ImGui::TableNextColumn();
        for (int idx = 0; idx < sizeof(tle) / sizeof(SGS_TLE*); idx++)
        {
            if (!tle[idx])
                break;
            //ImGui::PushStyleColor(ImGuiCol_Text, tle[idx]->color);
            for (uint32_t row = 0; row < sizeof(tle[idx]->Satellites) / sizeof(SatelliteObject*); row++)
            {
                if (!tle[idx]->Satellites[row])
                    break;
                if (tle[idx]->Satellites[row]->use)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", tle[idx]->Satellites[row]->Name());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d/%d %d:%d:%d", tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Month(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Day(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Hour(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Minute(),
                        tle[idx]->Satellites[row]->_nextaos[0].AddHours(9).Second());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d/%d %d:%d:%d", tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Month(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Day(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Hour(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Minute(),
                        tle[idx]->Satellites[row]->_nextlos[0].AddHours(9).Second());
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2f", tle[idx]->Satellites[row]->_max_elevation[0] * RAD_TO_DEG);
                    ImGui::TableNextColumn();

                    sprintf_s(this->_ButtonLabel_Info, "Info##trackingwindow%d", row);
                    if (ImGui::Button(this->_ButtonLabel_Info, ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetFontSize() * 1.5)))
                    {
                        //State.Display_Satinfo = true;
                    }
                    //if (State.Display_Satinfo)
                    //    State.Display_Satinfo = popup_satinfo(idx, row);
                    ImGui::SameLine();
                    if (tle[idx]->trackidx == row && tle[idx]->imported)
                    {
                        sprintf_s(this->_ButtonLabel_Track, "  On");
                        ImGui::Text(this->_ButtonLabel_Track);
                    }
                    else
                    {
                        sprintf_s(this->_ButtonLabel_Track, "Track##trackingwindow%d", row);
                        if (ImGui::Button(this->_ButtonLabel_Track, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                        {
                            tle[idx]->trackidx = row;
                            // PathGenerator(tle[idx]->Satellites[row]);
                        }
                    }
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->topo.azimuth * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->topo.elevation * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm", tle[idx]->Satellites[row]->topo.range);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm/s", tle[idx]->Satellites[row]->topo.range_rate);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->geo.latitude * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.3lf", tle[idx]->Satellites[row]->geo.longitude * RAD_TO_DEG);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.2lfkm", tle[idx]->Satellites[row]->geo.altitude);
                }

            }
            //ImGui::PopStyleColor();
        }

        ImGui::EndTable();
    }
    ImGui::End();

}



bool SGS_PropagatorWindow::popup_satinfo(uint32_t idx, uint32_t row)
{
    sprintf(SatWindowLabelBuf, "Details : %s", State.Satellites[idx][row]->Name());

    ImGui::OpenPopup(SatWindowLabelBuf);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(1080, 640), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal(SatWindowLabelBuf, NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        if (ImGui::BeginTable("##State.SatellitesInformations", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable, ImVec2(ImGui::GetContentRegionAvail().x, 480)));
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("AOS",           ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("LOS",       ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableSetupColumn("Max Elevation",  ImGuiTableColumnFlags_NoHide, 0.0f);
            ImGui::TableHeadersRow();
            
            // ImGui::TableNextColumn();
            for(int i = 0; i < 64; i++)
            {
                if(State.Satellites[idx][row]->_max_elevation[i] <= 0.0f || State.Satellites[idx][row]->_max_elevation[i] > 90.0f)
                    break;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d/%d %d:%d:%d",   State.Satellites[idx][row]->_nextaos[i].AddHours(9).Month(), 
                                                State.Satellites[idx][row]->_nextaos[i].AddHours(9).Day(),
                                                State.Satellites[idx][row]->_nextaos[i].AddHours(9).Hour(),
                                                State.Satellites[idx][row]->_nextaos[i].AddHours(9).Minute(),
                                                State.Satellites[idx][row]->_nextaos[i].AddHours(9).Second());
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d/%d %d:%d:%d",   State.Satellites[idx][row]->_nextlos[i].AddHours(9).Month(), 
                                                State.Satellites[idx][row]->_nextlos[i].AddHours(9).Day(),
                                                State.Satellites[idx][row]->_nextlos[i].AddHours(9).Hour(),
                                                State.Satellites[idx][row]->_nextlos[i].AddHours(9).Minute(),
                                                State.Satellites[idx][row]->_nextlos[i].AddHours(9).Second());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f", State.Satellites[idx][row]->_max_elevation[i] * RAD_TO_DEG);
            }
            ImGui::EndTable();
        }
        if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
        {
            ImGui::EndPopup();
            ImGui::CloseCurrentPopup();
            return false;
        }
        ImGui::EndPopup();
        return true;
    }
    return 0;
}
