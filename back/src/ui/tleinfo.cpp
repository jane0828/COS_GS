#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_TLEInfoWindow::SGS_TLEInfoWindow()
{
    this->_open = true;
    memset(this->SearchBuf, 0, sizeof(this->SearchBuf));
    memset(this->SelectButtonTextBuf, 0, sizeof(this->SelectButtonTextBuf));

}

SGS_TLEInfoWindow::~SGS_TLEInfoWindow()
{
    this->_open = false;
}

void SGS_TLEInfoWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}


void SGS_TLEInfoWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("TLE Settings##SGS_TLEInfoWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::BeginCombo("##TLECombobox", tle[PropagatorWindow->TLEref]->label))
    {
        for (int TLEindex = 0; TLEindex < sizeof(tle) / sizeof(SGS_TLE *); TLEindex++)
        {
            if (!tle[TLEindex])
                break;

            if (ImGui::Selectable(tle[TLEindex]->label, (PropagatorWindow->TLEref == TLEindex)))
                PropagatorWindow->TLEref = TLEindex;

            if (PropagatorWindow->TLEref == TLEindex)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##", this->SearchBuf, sizeof(this->SearchBuf));

    ImGui::Checkbox("##sgp4checkbox", &tle[PropagatorWindow->TLEref]->sgp4on);
    ImGui::SameLine();
    ImGui::Text("SGP4 Calculation");

    if (ImGui::Button("Load TLE", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 2.0)))
    {
        if (PropagatorWindow->HANDLES[0])
        {
            PropagatorWindow->THREAD[0] = false;
            WaitForSingleObject(PropagatorWindow->HANDLES[0], 50000);
            CloseHandle(PropagatorWindow->HANDLES[0]);
        }
        tle[PropagatorWindow->TLEref]->Import();
        PropagatorWindow->THREAD[0] = true;
        PropagatorWindow->HANDLES[0] = CreateThread(
            NULL,
            0,
            TrackingSatellites,
            NULL,
            0,
            NULL
        );

        if (PropagatorWindow->HANDLES[0] == NULL)
        {
            console->AddLog(SGS_CONMOD_ERR, "Failed to create thread.");
        }
    }
    if (ImGui::BeginTable("##TLESatelliteListup", 4, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);
        ImGui::TableSetupColumn("Name##tle", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Use##tle", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Next AOS##tle", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableSetupColumn("Max Elevation##tle", ImGuiTableColumnFlags_NoHide, 0.0f);
        ImGui::TableHeadersRow();

        // if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
        // {
        //     if (sorts_specs->SpecsDirty)
        //     {
        //         // If the sorting specs have changed, update the sorting state
        //         if (items.Size > 1)
        //             qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
        //         MyItem::s_current_sort_specs = NULL;
        //         sorts_specs->SpecsDirty = false;
        //         sort_column = sorts_specs->Specs[0].ColumnIndex;
        //         sort_ascending = sorts_specs->Specs[0].SortDirection == ImGuiSortDirection_Ascending;
        //     }
        // }

        for (uint32_t pidx = 0; pidx < sizeof(tle[PropagatorWindow->TLEref]->Satellites) / sizeof(SatelliteObject*); pidx++)
        {

            if (tle[PropagatorWindow->TLEref]->Satellites[pidx] == NULL)
                break;
            if (tle[PropagatorWindow->TLEref]->Satellites[pidx]->_max_elevation[0] < 0 && tle[PropagatorWindow->TLEref]->sgp4on)
            {
                continue;
            }
            if (strlen(this->SearchBuf) != 0 && strstr(tle[PropagatorWindow->TLEref]->Satellites[pidx]->Name(), this->SearchBuf) == NULL)
            {
                continue;
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", tle[PropagatorWindow->TLEref]->Satellites[pidx]->Name());
            ImGui::TableSetColumnIndex(1);
            if (tle[PropagatorWindow->TLEref]->Satellites[pidx]->use == false)
            {
                sprintf_s(this->SelectButtonTextBuf, "Select##TLE%d", pidx);
                if (ImGui::Button(this->SelectButtonTextBuf, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                {
                    tle[PropagatorWindow->TLEref]->Satellites[pidx]->use = true;
                    if (tle[PropagatorWindow->TLEref]->Satellites[pidx]->cal == false)
                    {
                        tle[PropagatorWindow->TLEref]->Satellites[pidx]->cal = true;
                        tle[PropagatorWindow->TLEref]->Satellites[pidx]->Refresh();
                    }
                    // SatelliteModelInitialize(tle[PropagatorWindow->TLEref]->Satellites[pidx]);
                }
            }
            else
            {
                sprintf_s(this->SelectButtonTextBuf, "Delete##TLE%d", pidx);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.650f, 0.266f, 0.322f, 1.0f));
                if (ImGui::Button(this->SelectButtonTextBuf, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
                {
                    tle[PropagatorWindow->TLEref]->Satellites[pidx]->use = false;
                    // SatelliteModelDelete(tle[PropagatorWindow->TLEref]->Satellites[pidx]);
                }
                ImGui::PopStyleColor();
            }
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d/%d %d:%d:%d", tle[PropagatorWindow->TLEref]->Satellites[pidx]->_nextaos[0].AddHours(9).Month(),
                tle[PropagatorWindow->TLEref]->Satellites[pidx]->_nextaos[0].AddHours(9).Day(),
                tle[PropagatorWindow->TLEref]->Satellites[pidx]->_nextaos[0].AddHours(9).Hour(),
                tle[PropagatorWindow->TLEref]->Satellites[pidx]->_nextaos[0].AddHours(9).Minute(),
                tle[PropagatorWindow->TLEref]->Satellites[pidx]->_nextaos[0].AddHours(9).Second());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.3lf", tle[PropagatorWindow->TLEref]->Satellites[pidx]->_max_elevation[0] * RAD_TO_DEG);
        }
        ImGui::EndTable();
    }
    //ImGui::SameLine();
    //if (ImGui::Button("Close", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    //{
    //    ImGui::EndPopup();
    //    ImGui::CloseCurrentPopup();
    //}

    ImGui::End();
}

bool SGS_TLEInfoWindow::popup_load()
{
    ImGui::OpenPopup("Load");
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Appearing);
    if ( ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize) )
    {
        ImGui::Text("Progress : %d%c", State.TLEref * 100 / State.tleallindex, '%');
        ImGui::EndPopup(); 
    }
    return true;
}
