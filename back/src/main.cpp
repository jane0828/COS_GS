//PROJECT       SGS
//VERSION       2.2.11
//AUTHORS       MIMAN COMS/GS Team
//REFERENCE     CANYVAL_GS
//Author        Dukmin Jang

//Define Envs
#define STB_IMAGE_IMPLEMENTATION
#define READ_BUF_SIZE   1024

#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

#undef CSP_USE_RDP

Command * command;
dlreqdata * request = (dlreqdata *)malloc(sizeof(dlreqdata));

////GUI

////Thread
/* pthread index */
// 0 : Radio Control
// 1 : Rotator Control
// 2 : Rotator Read
// 3 : TLE Trackings
// 4 : TX scheduler
// 5 : 
// 6 : AMP Timer
// 7 : Doppler Shift Correction
// 8 : FTP downlink/uplink
// 9 : 
//10 : Signaltest
//11 : TRX Controller
//12 : Radio Controller
//13 : AutoPilot
//14 : 
//15 : TLE load
pthread_t p_thread[16];
pthread_mutex_t conn_lock = PTHREAD_MUTEX_INITIALIZER;
csp_debug_level_t debug_level;


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


void Initialize_ConfigSettings()
{
    memset(setup->Transciever_devname, 0, sizeof(setup->Transciever_devname));
    memset(setup->Switch_devname, 0, sizeof(setup->Switch_devname));
    memset(setup->Rotator_devname, 0, sizeof(setup->Rotator_devname));
    memset(setup->S_Username, 0, sizeof(setup->S_Username));
    memset(setup->S_Address, 0, sizeof(setup->S_Address));
    memset(setup->S_passwd, 0, sizeof(setup->S_passwd));
    sprintf(setup->Transciever_devname, "/dev/GS102");
    sprintf(setup->Switch_devname, "/dev/KTA223");
    sprintf(setup->Rotator_devname, "/dev/GS232B");
    sprintf(setup->S_Username, "s-band");
    sprintf(setup->S_Address, "165.132.28.235");
    sprintf(setup->S_passwd, "qwe123");
    setup->Transceiver_baud = 500000;
    setup->Rotator_baud = 9600;
    setup->Switch_baud = 115200;
    setup->gs100_node = 20;
    setup->kiss_node = 8;
    setup->obc_node = 3;
    setup->ax100_node = 5;
    setup->default_freq = 436500000;
    setup->default_timeout = 1000;
    setup->pingsize = 4;
    setup->guard_delay = 150;
    setup->queue_delay = 150;

    TleRefresh();
}

const char* fontfile = "./bin/font/Inconsolata.ttf";

//Main
int main(int, char**)
{
    Initialize_ConfigSettings();
    Initialize_CMDLabels();
    bool GUIneedshutdown = true;

    rotator = new Rotator();
    
    render = new Render("Space Ground Station Server");

    PropagatorWindow = new SGS_PropagatorWindow();
    TLEInfoWindow = new SGS_TLEInfoWindow();
    console = new SGS_ConsoleWindow();

    tle[0] = new SGS_TLE(0, "FDS", false);
    tle[0]->color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    tle[1] = new SGS_TLE(1, "cubesat", true);
    tle[0]->color = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
    tle[2] = new SGS_TLE(2, "last-30-days", true);
    tle[0]->color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    tle[3] = new SGS_TLE(3, "weather", true);
    tle[0]->color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    tle[4] = new SGS_TLE(4, "dmc", true);
    tle[0]->color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    tle[5] = new SGS_TLE(5, "amateur", true);
    tle[0]->color = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);
    tle[6] = new SGS_TLE(6, "active", true);
    tle[0]->color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
    for (int i = 0; i < sizeof(tle) / sizeof(SGS_TLE*); i++)
    {
        if (!tle[i])
            break;
        tle[i]->obs = new Observer(37.564021, 126.934600, 0.05);
    }

    render->Impl_Vulkan_Init();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.Fonts->AddFontFromFileTTF("bin/fonts/space-mono/SpaceMono-Regular.ttf", 50.0f);
    io.Fonts->AddFontFromFileTTF("bin/fonts/space-mono/SpaceMono-Bold.ttf", 50.0f);
    io.Fonts->AddFontFromFileTTF("bin/fonts/space-mono/SpaceMono-Italic.ttf", 50.0f);
    io.Fonts->AddFontFromFileTTF("bin/fonts/space-mono/SpaceMono-BoldItalic.ttf", 50.0f);
    io.FontDefault = io.Fonts->Fonts[0];

    ImVec4 clear_color = ImVec4(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.00f);
    ImGuiWindowFlags SGS_WindowFlag_Fixed = 0|
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_HorizontalScrollbar |
        ImGuiWindowFlags_NoTitleBar;
    ImGuiWindowFlags SGS_WindowFlag_Folder = 0|
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_HorizontalScrollbar;

    render->style = &ImGui::GetStyle();
    render->ImGuiCustomStyle();
    
    console->WindowFlag = SGS_WindowFlag_Fixed;
    SetupWindow->WindowFlag = SGS_WindowFlag_Folder;
    AutoPilotWindow->WindowFlag = SGS_WindowFlag_Fixed;
    PropagatorWindow->WindowFlag = SGS_WindowFlag_Fixed;
    TLEInfoWindow->WindowFlag = SGS_WindowFlag_Fixed;
    CMDWindow->WindowFlag = SGS_WindowFlag_Fixed;
    FTPWindow->WindowFlag = SGS_WindowFlag_Fixed;
    TRXWindow->WindowFlag = SGS_WindowFlag_Fixed;
    TRXParamWindow->WindowFlag = SGS_WindowFlag_Fixed;
    RotatorWindow->WindowFlag = SGS_WindowFlag_Fixed;
    BeaconWindow->WindowFlag = SGS_WindowFlag_Fixed;
    TLMWindow->WindowFlag = SGS_WindowFlag_Fixed;

    command = new Command;
    memset(command, 0, sizeof(Command));

    



    while(!render->WindowShouldClose)
    {
        GUIneedshutdown = true;
        glfwPollEvents();
        render->RebuildSwapChain();

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // glfwGetWindowSize(window, &WinWidth, &WinHeight);
        // ReactiveWidth = (float)WinWidth / DISPLAY_WINDWIDTH;
        // ReactiveHeight = (float)WinHeight / DISPLAY_WINDHEIGHT;
        now_rx_bytes_update();

        ImGui_MainMenu();

        // // Show Tracking Window
        // ImGui::SetNextWindowPos(ImVec2(0 * ReactiveWidth, DISPLAY_ROW3 * ReactiveHeight), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_TRACK_W * ReactiveWidth, DISPLAY_TRACK_H * ReactiveHeight), ImGuiCond_Always);
        // ImGui_TrackWindow(ReactiveHeight * 0.5);
        PropagatorWindow->FontScale = 1.0f;
        PropagatorWindow->Locate(0, 0.7, 0.5, 0.3, render->width, render->height)
        PropagatorWindow->Render();

        // //Show Frequency Window
        // ImGui::SetNextWindowPos(ImVec2(0, MENUPADDING), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_FREQ_W * ReactiveWidth, ReactiveHeight * DISPLAY_FREQ_H), ImGuiCond_Always);
        // ImGui_FrequencyWindow(ReactiveHeight * 0.5);
        

        // //Show Rotator Window
        // ImGui::SetNextWindowPos(ImVec2(0, ReactiveHeight * DISPLAY_ROW1), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_ROTAT_W * ReactiveWidth, ReactiveHeight * DISPLAY_ROTAT_H ), ImGuiCond_Always);
        // ImGui_RotatorWindow(ReactiveHeight * 0.5);

        // // Show Console Window
        // console->ChangeWindowSize(DISPLAY_COL1 * ReactiveWidth, MENUPADDING, ReactiveWidth * DISPLAY_CONSOLE_W, ReactiveHeight * DISPLAY_CONSOLE_H);
        // console->Draw("General Console", &Showstate, ReactiveHeight * 0.6);

        // //Show Beacon State Window
        // ImGui::SetNextWindowPos(ImVec2(DISPLAY_COL3 * ReactiveWidth, MENUPADDING), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_STATE_W * ReactiveWidth, ReactiveHeight * DISPLAY_STATE_H), ImGuiCond_Always);
        // ImGui_BeaconWindow(ReactiveHeight * 0.65);

        // //Show Control Window
        // ImGui::SetNextWindowPos(ImVec2(DISPLAY_COL2 * ReactiveWidth, DISPLAY_ROW2 * ReactiveHeight), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_CONTROL_W * ReactiveWidth, DISPLAY_CONTROL_H * ReactiveHeight), ImGuiCond_Always);
        // ImGui_ControlWindow(ReactiveHeight * 0.7);
        
        // //Show Command Window
        // ImGui::SetNextWindowPos(ImVec2(DISPLAY_COL2 * ReactiveWidth, DISPLAY_ROW3 * ReactiveHeight), ImGuiCond_Always);
        // ImGui::SetNextWindowSize(ImVec2(DISPLAY_COMMAND_W * ReactiveWidth, DISPLAY_COMMAND_H * ReactiveHeight), ImGuiCond_Always);
        // ImGui_TelemetryWindow(ReactiveHeight * 0.5);


        //Display Popup Windows
        if (State.Display_Setup)
            State.Display_Setup = popup_setup(setup);
        if(State.InitializeRequired)
        {
            //Switch Box
            if(init_switch(setup->Switch_devname, setup->Switch_baud) < 0)
            {
                console->AddLog(SGS_CONMOD_ERR, "Connect to Switch Box..............Failed.");
                State.SwitchConnection = false;
            }
            else
            {
                console->AddLog(SGS_CONMOD_OK, "Connect to Switch Box.............Success.");
                State.SwitchConnection = true;
            }


            //Rotator
            if(init_rotator() < 0)
            {
                console->AddLog(SGS_CONMOD_ERR, "Connect to Rotator.................Failed.");
            }
            else
            {
                State.RotatorConnection = true;
                State.RotatorInitialize = true;
                console->AddLog(SGS_CONMOD_OK, "Connect to Rotator................Success.");
            }
            if(init_transceiver() < 0)
            {
                console->AddLog(SGS_CONMOD_ERR, "Connect to GS100...................Failed.");
                State.GS100_Connection = false;
            }
            else
            {
                // pthread_join(p_thread[11], NULL);
                // pthread_join(p_thread[12], NULL);
                // pthread_join(p_thread[13], NULL);
                console->AddLog(SGS_CONMOD_OK, "Connect to GS100..................Success.");
                State.GS100_Connection = true;
                
                pthread_create(&p_thread[12], NULL, RadInitialize, (void *)setup);
                State.TRx_mode = true;
                State.downlink_mode = true;
                pthread_create(&p_thread[11], NULL, TRxController, NULL); 
                
            }
            // if(State.SbandUse)
            // {
            //     if(init_sband(setup) < 0)
            //     {
            //         console->AddLog("[ERROR]##Connect to S-band GS...............Failed.");
            //     }
            //     else
            //     {
            //         console->AddLog("[OK]##Connect to S-band GS...............Success.");
            //     }
            // }
            State.InitializeRequired = false;
            pthread_create(&p_thread[3], NULL, TrackingSatellites, NULL);
            pthread_create(&p_thread[2], NULL, RotatorReadInfo, NULL);
            
            
        }

        // Rendering
        // ImGui_ClearColorBuf(window, clear_color);
        
        
        // Exclude about 35 pixels in map's height not to invade tab bar
        // Note that unlike ImGui, origin of openGL screen coordinate locates at bottom left corner
        // (top left for ImGui)
        // glViewport(DISPLAY_COL1 * ReactiveWidth, DISPLAY_WINDHEIGHT * ReactiveHeight - (DISPLAY_ROW2 * ReactiveHeight), DISPLAY_MODEL_W * ReactiveWidth, ReactiveHeight * DISPLAY_MODEL_H );
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffers
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_BLEND);  // Enable transparency

        // ShaderWorldmap2D->use();
        // glActiveTexture(GL_TEXTURE0 + 1);
        // glBindTexture(GL_TEXTURE_2D, TextureWorldmap2D);
        // ShaderWorldmap2D->setInt("texture1", 1);
        // Worldmap2D->Draw(ShaderWorldmap2D);
        // // Sattext2D->RenderText("Test", 100.0, 100.0, 1.0, glm::vec3(1.0f, 1.0f, 1.0f));

        // glViewport(0, DISPLAY_WINDHEIGHT * ReactiveHeight - (DISPLAY_ROW3 * ReactiveHeight), DISPLAY_COL1 * ReactiveWidth, ReactiveHeight * (DISPLAY_ROW3 - DISPLAY_ROW2) );
        // // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear buffers
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_BLEND);  // Enable transparency

        // ShaderPolarmap2D_el0->use();
        // Polarmap2D_el0->Update();
        // ShaderPolarmap2D_el60->setFloat("alpha", 1.0f);
        // Polarmap2D_el0->drawLoop(ShaderPolarmap2D_el0);

        // ShaderPolarmap2D_el30->use();
        // Polarmap2D_el30->Update();
        // ShaderPolarmap2D_el30->setFloat("alpha", 1.0f);
        // Polarmap2D_el30->drawLoop(ShaderPolarmap2D_el30);

        // ShaderPolarmap2D_el60->use();
        // Polarmap2D_el60->Update();
        // ShaderPolarmap2D_el0->setFloat("alpha", 1.0f);
        // Polarmap2D_el60->drawLoop(ShaderPolarmap2D_el60);

        // ShaderPolarmap2D_azNS->use();
        // Polarmap2D_azNS->Update();
        // ShaderPolarmap2D_azNS->setFloat("alpha", 1.0f);
        // Polarmap2D_azNS->draw(ShaderPolarmap2D_azNS);

        // ShaderPolarmap2D_azEW->use();
        // Polarmap2D_azEW->Update();
        // ShaderPolarmap2D_azEW->setFloat("alpha", 1.0f);
        // Polarmap2D_azEW->draw(ShaderPolarmap2D_azEW);

        // PathDisplayer();
        // for(int i = 0; i < sizeof(State.Satellites) / sizeof(State.Satellites[0]); i++)
        // {
        //     for(int j = 0; j < sizeof(State.Satellites[0]) / sizeof(SatelliteObject *); j++)
        //     {
        //         if(!State.Satellites[i][j])
        //             continue;
        //         if(!State.Satellites[i][j]->use)
        //             continue;
        //         SatpointDisplayer(State.Satellites[i][j]);
        //         SatDisplayer(State.Satellites[i][j], &TextureSatellites2D);
        //     }
        // }

        ImGui::Render();
        render->RebuildGLFWwindow();
        render->BackgroundRender(clear_color);
        render->FrameRender();
        render->FramePresent();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
            wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
            wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
            wd->ClearValue.color.float32[3] = clear_color.w;
            FrameRender(wd, draw_data);
            FramePresent(wd);
        }
    }
    
    // Cleanup
    if(GUIneedshutdown)
    {
        err = vkDeviceWaitIdle(g_Device);
        check_vk_result(err);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        CleanupVulkanWindow();
        CleanupVulkan();

        glfwDestroyWindow(window);
        glfwTerminate();
        GUIneedshutdown = false;
    }
    

    fin_rotator();
    fin_transceiver();

    if (State.AllThread) // "Start" at popup
    {
        State.TRx_mode = false;
        State.downlink_mode = false;
        State.uplink_mode = false;
        State.ftp_mode = false;
        State.Engage = false;
        State.Doppler = false;
        State.GS100_Connection =false;
        State.TLEref = 0;
        State.TLEnow = 0;
        State.TrackThread = false;
        State.RotatorReading = false;
        
        State.InitializeRequired = true;
        autopilot->state = false;
        printf("Joined thread : \n");
        for(int i = 0; i < 16; i++)
        {
            pthread_join(p_thread[i], NULL);
            printf("%d\t", i);
        }
        printf("\n");
        
    }

    if(autopilot->state)
    {
        sleep(10);
    }
    else
    {
        State.AllThread = false;
    }

    

    delete setup; 
    setup = NULL;
    // delete ShaderWorldmap2D;
    // ShaderWorldmap2D = NULL;
    // delete Worldmap2D;
    // Worldmap2D = NULL;
    // delete ShaderPolarmap2D_el0;
    // ShaderPolarmap2D_el0 = NULL;
    // delete ShaderPolarmap2D_el30;
    // ShaderPolarmap2D_el30 = NULL;
    // delete ShaderPolarmap2D_el60;
    // ShaderPolarmap2D_el60 = NULL;
    // delete ShaderPolarmap2D_azNS;
    // ShaderPolarmap2D_azNS = NULL;
    // delete ShaderPolarmap2D_azEW;
    // ShaderPolarmap2D_azEW = NULL;
    // delete Polarmap2D_el0;
    // Polarmap2D_el0 = NULL;
    // delete Polarmap2D_el30;
    // Polarmap2D_el30 = NULL;
    // delete Polarmap2D_el60;
    // Polarmap2D_el60 = NULL;
    // delete Polarmap2D_azNS;
    // Polarmap2D_azNS = NULL;
    // delete Polarmap2D_azEW;
    // Polarmap2D_azEW = NULL;

    for(int i = 0; i < sizeof(State.Satellites) / sizeof(State.Satellites[0]); i++)
    {
        for(int j = 0; j < sizeof(State.Satellites[0]) / sizeof(SatelliteObject *); j++)
        {
            delete State.Satellites[i][j];
            State.Satellites[i][j] = NULL;
        }
    }
    // PathDelete(); 
    sleep(0.5);
    printf("Finish MIMAN GS.\n");
    return 0;
    
}
