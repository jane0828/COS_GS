#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

SGS_FTPWindow::SGS_FTPWindow()
{
    this->_open = true;
    memset(this->_FTPWindowBuffer, 0, sizeof(this->_FTPWindowBuffer));
    memset(this->_temppath, 0, sizeof(this->_temppath));
    memset(this->ftplistup, 0, sizeof(this->ftplistup));
    memset(this->_temppath, 0, sizeof(this->_temppath));
}

SGS_FTPWindow::~SGS_FTPWindow()
{
    this->_open = false;
}

void SGS_FTPWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{
    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}


void SGS_FTPWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin("SGS_FTPWindow", &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(FontScale);
    ImGui::BeginChild("##FTPManager", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.33), true, this->WindowFlag);
    ImGui::Text("File List");
    ImGui::Text("Telemetry");
    ImGui::RadioButton("HK ##ftpwindow", &this->_temptarget, MIM_DLTYPE_HK);
    ImGui::SameLine();
    ImGui::RadioButton("AOD##ftpwindow", &this->_temptarget, MIM_DLTYPE_AOD);
    ImGui::SameLine();
    ImGui::RadioButton("LOG##ftpwindow", &this->_temptarget, MIM_DLTYPE_LOG);
    ImGui::SameLine();
    ImGui::RadioButton("SNSR##ftpwindow", &this->_temptarget, MIM_DLTYPE_SNSR);
    ImGui::SameLine();
    ImGui::RadioButton("GPS##ftpwindow", &this->_temptarget, MIM_DLTYPE_GPS);
    ImGui::SameLine();
    ImGui::RadioButton("CTRLO##ftpwindow", &this->_temptarget, MIM_DLTYPE_CTRLO);

    ImGui::Text("Status");
    ImGui::RadioButton("NOW##ftpwindow", &this->_tempstatus, MIM_DLSTAT_NEW);
    ImGui::SameLine();
    ImGui::RadioButton("OLD##ftpwindow", &this->_tempstatus, MIM_DLSTAT_OLD);
    ImGui::SameLine();
    ImGui::RadioButton("CUR##ftpwindow", &this->_tempstatus, MIM_DLSTAT_ING);
    ImGui::BeginTable("##filelisttable", 3, false, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 3.0f));

    ImGui::TableSetupScrollFreeze(1, 1);
    ImGui::TableSetupColumn("Number##tle", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Step##tle", ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableSetupColumn("Offset##tle", ImGuiTableColumnFlags_NoHide, 0.0f);
    ImGui::TableHeadersRow();
    if (this->_tempfilenum < 1)
        this->_tempfilenum = 1;
    if (this->_tempstep < 0)
        this->_tempstep = 0;
    if (this->_tempoffset < 0)
        this->_tempoffset = 0;
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::InputInt("##ftpwindow_filenum", &this->_tempfilenum);
    ImGui::TableSetColumnIndex(1);
    ImGui::InputInt("##ftpwindow_step", &this->_tempstep);
    ImGui::TableSetColumnIndex(2);
    ImGui::InputInt("##ftpwindow_offset", &this->_tempoffset);
    ImGui::EndTable();


    if (ImGui::Button("Save##ftpwindow", ImVec2(ImGui::GetContentRegionAvail().x * 0.5, ImGui::GetContentRegionAvail().y)))
    {
        //request->target = (uint16_t)this->_temptarget;
        //request->filestatus = (uint16_t)this->_tempstatus;
        //request->filenum = (uint32_t)this->_tempfilenum;
        //request->offset = (uint32_t)this->_tempoffset;
        //request->step = (uint32_t)this->_tempstep;
    }
    ImGui::SameLine();
    if (ImGui::Button("Request##ftpwindow", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        //packetsign* request_ = (packetsign*)malloc(sizeof(dlreqdata) + 8);
        //request_->Identifier = MIM_ID;
        //request_->PacketType = MIM_PT_DLREQ;
        //request_->Length = sizeof(dlreqdata);
        //memcpy(request_->Data, request, sizeof(dlreqdata));
        //pthread_create(&p_thread[4], NULL, task_uplink_onorbit, (void*)request_);
    }
    ImGui::EndChild();
    ImGui::BeginChild("##FTPMagic", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, this->WindowFlag);
    ImGui::Text("FTP");
    ImGui::RadioButton("V1      ##FTPVersionSelect1", &this->_ftp_version, 1);
    ImGui::SameLine();
    ImGui::RadioButton("V2      ##FTPVersionSelect2", &this->_ftp_version, 2);
    ImGui::Text("Chunk Size ");
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputScalar("##ftpchunksize", ImGuiDataType_U32, &this->_chunk_sz, NULL, NULL, "%u");
    if (this->_chunk_sz < 1)
        this->_chunk_sz = 1;
    ImGui::Text("Task ");
    ImGui::RadioButton("Upload  ##FTPTaskSelect00", &this->_ftp_task, FTP_UPLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Download##FTPTaskSelect02", &this->_ftp_task, FTP_DOWNLOAD_REQUEST);
    ImGui::SameLine();
    ImGui::RadioButton("Listup  ##FTPTaskSelect07", &this->_ftp_task, FTP_LIST_REQUEST);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    // ImGui::Text("     ");
    // ImGui::SameLine();
    // ImGui::RadioButton("Move    ##FTPTaskSelect10", &this->_ftp_task, FTP_MOVE_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("Remove  ##FTPTaskSelect12", &this->_ftp_task, FTP_REMOVE_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("Copy    ##FTPTaskSelect23", &this->_ftp_task, FTP_COPY_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("MKDIR   ##FTPTaskSelect25", &this->_ftp_task, FTP_MKDIR_REQUEST);
    // ImGui::SameLine();
    // ImGui::RadioButton("RMDIR   ##FTPTaskSelect27", &this->_ftp_task, FTP_RMDIR_REQUEST);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.8);
    if (ImGui::BeginCombo("##FTPCombobox", this->ftplistup[this->NowFTP].name))
    {
        for (int FTPindex = 0; FTPindex < IM_ARRAYSIZE(this->ftplistup); FTPindex++)
        {
            if (this->ftplistup[FTPindex].name[0] == 0)
                continue;
            bool SelectedFTP = (this->NowFTP == FTPindex);
            char Selectedlabel[64];
            sprintf(Selectedlabel, "%s##%d", this->ftplistup[FTPindex].name, FTPindex);
            if (ImGui::Selectable(Selectedlabel, SelectedFTP))
                this->NowFTP = FTPindex;

            if (SelectedFTP)
                ImGui::SetItemDefaultFocus();
            usleep(10);
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Upload", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        if (this->_ftp_mode)
            console->AddLog(SGS_CONMOD_ERR, "Still Uplink Thread is Running. Please Retry Later.");
        else
        {
            //pthread_join(p_thread[8], NULL);
            //if (this->_ftp_version == 1)
            //    pthread_create(&p_thread[8], NULL, ftp_uplink_force, &this->ftplistup[this->NowFTP]);
            //else
            //    pthread_create(&p_thread[8], NULL, ftp_uplink_onorbit, &this->ftplistup[this->NowFTP]);
        }
    }
    ImGui::Text("Local  ");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##FTP_local_filepath : ", this->ftplistup[this->NowFTP].local_path, sizeof(this->ftplistup[this->NowFTP].local_path));
    ImGui::PopItemWidth();
    ImGui::Text("Remote ");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##FTP_remote_filepath : ", this->ftplistup[this->NowFTP].remote_path, sizeof(this->ftplistup[this->NowFTP].remote_path));
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.8);
    if (ImGui::BeginCombo("##FTPCombobox", this->ftplistup[this->NowFTP].name))
    {
        for (int FTPindex = 0; FTPindex < IM_ARRAYSIZE(this->ftplistup); FTPindex++)
        {
            if (this->ftplistup[FTPindex].name[0] == 0)
                continue;
            bool SelectedFTP = (this->NowFTP == FTPindex);
            char Selectedlabel[64];
            sprintf(Selectedlabel, "%s##%d", this->ftplistup[FTPindex].name, FTPindex);
            if (ImGui::Selectable(Selectedlabel, SelectedFTP))
                this->NowFTP = FTPindex;

            if (SelectedFTP)
                ImGui::SetItemDefaultFocus();
            usleep(10);
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (ImGui::Button("Download", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() * 1.5)))
    {
        if (this->_ftp_mode)
            console->AddLog(SGS_CONMOD_ERR, "Still Uplink Thread is Running. Please Retry Later.");
        else
        {
            //pthread_join(p_thread[8], NULL);
            //if (this->_ftp_version == 1)
            //    pthread_create(&p_thread[8], NULL, ftp_downlink_force, &this->ftplistup[this->NowFTP]);
            //else
            //    pthread_create(&p_thread[8], NULL, ftp_downlink_onorbit, &this->ftplistup[this->NowFTP]);
        }
    }
    ImGui::Text("Local  ");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##FTP_local_filepath : ", this->ftplistup[this->NowFTP].local_path, sizeof(this->ftplistup[this->NowFTP].local_path));
    ImGui::PopItemWidth();
    ImGui::Text("Remote ");
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##FTP_remote_filepath : ", this->ftplistup[this->NowFTP].remote_path, sizeof(this->ftplistup[this->NowFTP].remote_path));
    ImGui::PopItemWidth();
    ImGui::Text(this->_FTPWindowBuffer);

    ImGui::EndChild();
    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}