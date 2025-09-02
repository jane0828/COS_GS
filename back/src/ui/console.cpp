#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#endif


SGS_ConsoleWindow::SGS_ConsoleWindow()
{
    ClearLog();
    this->_open = true;
    this->_ntt = time(NULL);
    localtime(&this->_ntt);

    strftime(this->_date, sizeof(this->_date), "%Y-%m-%d %H:%M\n", &this->_ntm);
    this->_history_fs.write(this->_date, strlen(this->_date));
}

SGS_ConsoleWindow::~SGS_ConsoleWindow()
{
    ClearLog();
    this->_open = false;
    this->_history_fs.close();
    this->_debug_fs.close();
}

int SGS_ConsoleWindow::Stricmp(const char* str1, const char* str2)
{
    int d;
    while (d = toupper(*str2) - toupper(*str1) == 0 && *str1)
    {
        str1++;
        str2++;
    }
    return d;
}

int SGS_ConsoleWindow::Strnicmp(const char* str1, const char* str2, int n)
{
    int d = 0;
    while (n > 0 && (d = toupper(*str2) - toupper(*str1) == 0 && *str1))
    {
        str1++;
        str2++;
        n--;
    }
    return d;
}

char* SGS_ConsoleWindow::Strdup(const char* str)
{
    size_t len = strlen(str) + 1;
    void* buf = malloc(len);
    IM_ASSERT(buf);
    return (char*)memcpy(buf, (const void*)str, len);
}

void SGS_ConsoleWindow::Strtrim(char* str)
{
    char* str_end = str + strlen(str);
    while (str_end > str && str_end[-1] == ' ')
        str_end--;
    *str_end = 0;
}

void SGS_ConsoleWindow::ClearLog()
{
    for (int i = 0; i < _items.Size; i++)
        free(_items[i]);
    _items.clear();
}

void SGS_ConsoleWindow::AddLog(int mode, const char* fmt, ...)
{
    this->_ntt = time(NULL);
    localtime(&this->_ntt);
    strftime(this->_now, sizeof(this->_now), "[%H:%M:%S]  ", &this->_ntm);

    va_list args;
    va_start(args, fmt);
    vsnprintf(this->_buf, IM_ARRAYSIZE(this->_buf), fmt, args);
    this->_buf[IM_ARRAYSIZE(this->_buf) - 1] = 0;
    va_end(args);
    snprintf(this->_msg, sizeof(this->_now), this->_now);
    snprintf(this->_msg + sizeof(this->_now) - 1, sizeof(this->_buf), this->_buf);
    
    switch (mode) {
    case SGS_CONMOD_LOG: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_ERR: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_WRN: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_DBG: {
        this->_debug_fs.write(this->_msg, strlen(this->_msg));
        this->_debug_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_OK: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_AUTO: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    case SGS_CONMOD_AUTOERR: {
        _items.push_back(Strdup(this->_msg));
        this->_history_fs.write(this->_msg, strlen(this->_msg));
        this->_history_fs.write("\n", 1);
        break;
    }
    default: {
        printf(this->_msg);
        break;
    }
    }

        //Write to IMGUI Console.
    this->_history_fs.flush();
    this->_debug_fs.flush();
}

void SGS_ConsoleWindow::DelStartingCharLog(const char* fmt)
{
    char buf[1024];
    strcpy(buf, fmt);
    for (int i = 0; i < _items.Size; i++)
    {
        char* item = _items[i];
        if (strstr(item, buf))
        {
            _items.erase(&_items[i]);
        }
    }
}

void SGS_ConsoleWindow::DelPrefixLog(const char* fmt)
{
    char buf[1024];
    strcpy(buf, fmt);
    for (int i = 0; i < _items.Size; i++)
    {
        char* item = _items[i];
        if (strstr(item, buf))
        {
            _items.insert(&_items[i], item + 1);
            _items.erase(&_items[i + 1]);
        }
    }
}

void SGS_ConsoleWindow::Locate(float x_pos, float y_pos, float width, float height, float Winwidth, float Winheight)
{

    this->_x_pos = x_pos * Winwidth;
    this->_y_pos = y_pos * Winheight;
    this->_width = width * Winwidth;
    this->_height = height * Winheight;
    this->FontScale = this->FontScale * Winheight / 2160;
}

void SGS_ConsoleWindow::Render()
{
    ImGui::SetNextWindowPos(ImVec2(this->_x_pos, this->_y_pos), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(this->_width, this->_height), ImGuiCond_Always);
    ImGui::Begin(this->title, &this->_open, this->WindowFlag);
    ImGui::SetWindowFontScale(this->FontScale);
    bool copy_to_clipboard = false;

    const float footer_height_to_reserve = 0.0;
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    //ImGui::BeginChild("scrolling");
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear"))
            ClearLog();
        copy_to_clipboard = ImGui::Selectable("Copy");
        ImGui::EndPopup();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    for (int i = 0; i < _items.Size; i++)
    {
        char* item = _items[i];

        bool pop_color = false;
        if (strstr(item, "[ERROR]##"))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.38f, 0.36f, 1.0f));
            pop_color = true;
            item += 9;
        }
        else if (strstr(item, "[OK]##"))
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.60f, 0.70f, 1.0f));
            pop_color = true;
            item += 6;
        }
        else if (strncmp(item, "# ", 2) == 0)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
            pop_color = true;
        }
        else if (strstr(item, "$"))
        {
            item += 1;
        }
        ImGui::SetWindowFontScale(1.1);
        ImGui::TextUnformatted(item);
        if (pop_color)
        {
            ImGui::PopStyleColor();
        }
        ImGui::SetWindowFontScale(1.0);
    }

    if (copy_to_clipboard)
        ImGui::LogFinish();

    if (_push_to_bottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::SetWindowFontScale(1.0);

    ImGui::End();

}
