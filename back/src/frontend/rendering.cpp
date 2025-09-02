#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#endif

void GLFW_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
void VK_error_callback(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

Render::Render(const char* title)
{
    glfwSetErrorCallback(GLFW_error_callback);
    if (!glfwInit())
        printf("GLFW: Initialization Failed.\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create window with Vulkan context
    this->monitor = glfwGetPrimaryMonitor();
    this->mode = glfwGetVideoMode(this->monitor);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    this->window = glfwCreateWindow(this->mode->width, this->mode->height, title, nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
    }

    this->SetExtensions();
    this->SetupVulkan();

    // Create Window Surface

    VkResult err = glfwCreateWindowSurface(this->_Instance, this->window, this->_Allocator, &this->surface);
    VK_error_callback(err);

    // Create Framebuffers
    glfwGetFramebufferSize(this->window, &this->GLFWwidth, &this->GLFWheight);
    
    this->SetupVulkanWindow();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui this->style
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(this->window, true);
    glfwMaximizeWindow(this->window);

    glfwGetWindowSize(this->window, &this->Winwidth, &this->Winheight);
    this->width = (float)this->Winwidth;
    this->height = (float)this->Winheight;
}

Render::~Render()
{
    VkResult err = vkDeviceWaitIdle(this->Device);
    VK_error_callback(err);

    this->CleanupVulkan();
    Sleep(100);
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

void Render::SetExtensions()
{
    uint32_t extensions_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    for (uint32_t i = 0; i < extensions_count; i++)
        this->extensions.push_back(glfw_extensions[i]);
}

void Render::SetupVulkan()
{
    VkResult err;

    // Create Vulkan Instance
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Enumerate available extensions
    uint32_t properties_count;
    ImVector<VkExtensionProperties> properties;
    vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
    properties.resize(properties_count);
    err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
    VK_error_callback(err);

    // Enable required extensions
    if (this->IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
        this->extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

    // Create Vulkan Instance
    create_info.enabledExtensionCount = (uint32_t)this->extensions.Size;
    create_info.ppEnabledExtensionNames = this->extensions.Data;
    err = vkCreateInstance(&create_info, this->_Allocator, &this->_Instance);
    VK_error_callback(err);

    // Select Physical Device (GPU)
    this->PhysicalDevice = this->SetupVulkan_SelectPhysicalDevice();

    // Select graphics queue family
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(this->PhysicalDevice, &count, nullptr);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(this->PhysicalDevice, &count, queues);
        for (uint32_t i = 0; i < count; i++)
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                this->_QueueFamily = i;
                break;
            }
        free(queues);
        IM_ASSERT(this->_QueueFamily != (uint32_t)-1);
    }

    // Create Logical Device (with 1 queue)
    {
        ImVector<const char*> device_extensions;
        device_extensions.push_back("VK_KHR_swapchain");

        // Enumerate physical device extension
        uint32_t properties_count;
        ImVector<VkExtensionProperties> properties;
        vkEnumerateDeviceExtensionProperties(this->PhysicalDevice, nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        vkEnumerateDeviceExtensionProperties(this->PhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
            device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

        const float queue_priority[] = { 1.0f };
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = this->_QueueFamily;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
        create_info.ppEnabledExtensionNames = device_extensions.Data;
        err = vkCreateDevice(this->PhysicalDevice, &create_info, this->_Allocator, &this->Device);
        VK_error_callback(err);
        vkGetDeviceQueue(this->Device, this->_QueueFamily, 0, &this->Queue);
    }

    // Create Descriptor Pool
    // The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
    // If you wish to load e.g. additional textures you may need to alter pools sizes.
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(this->Device, &pool_info, this->_Allocator, &this->_DescriptorPool);
        VK_error_callback(err);
    }
}


bool Render::IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
{
    for (const VkExtensionProperties& p : properties)
        if (strcmp(p.extensionName, extension) == 0)
            return true;
    return false;
}

VkPhysicalDevice Render::SetupVulkan_SelectPhysicalDevice()
{
    uint32_t gpu_count;
    VkResult err = vkEnumeratePhysicalDevices(this->_Instance, &gpu_count, nullptr);
    VK_error_callback(err);
    IM_ASSERT(gpu_count > 0);

    ImVector<VkPhysicalDevice> gpus;
    gpus.resize(gpu_count);
    err = vkEnumeratePhysicalDevices(this->_Instance, &gpu_count, gpus.Data);
    VK_error_callback(err);

    // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
    // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
    // dedicated GPUs) is out of scope of this sample.
    for (VkPhysicalDevice& device : gpus)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return device;
    }

    // Use first GPU (Integrated) is a Discrete one is not available.
    if (gpu_count > 0)
        return gpus[0];
    return VK_NULL_HANDLE;
}

void Render::SetupVulkanWindow()
{
    this->MainWindowData.Surface = this->surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(this->PhysicalDevice, this->_QueueFamily, this->MainWindowData.Surface, &res);
    if (res != VK_TRUE)
    {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    this->MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(this->PhysicalDevice, this->MainWindowData.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    this->MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(this->PhysicalDevice, this->MainWindowData.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(this->_MinImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(this->_Instance, this->PhysicalDevice, this->Device, &this->MainWindowData, this->_QueueFamily, this->_Allocator, this->GLFWwidth, this->GLFWheight, this->_MinImageCount);
}

void Render::Impl_Vulkan_Init()
{
    init_info.Instance = this->_Instance;
    init_info.PhysicalDevice = this->PhysicalDevice;
    init_info.Device = this->Device;
    init_info.QueueFamily = this->_QueueFamily;
    init_info.Queue = this->Queue;
    init_info.PipelineCache = this->_PipelineCache;
    init_info.DescriptorPool = this->_DescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = this->_MinImageCount;
    init_info.ImageCount = this->MainWindowData.ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = this->_Allocator;
    init_info.CheckVkResultFn = VK_error_callback;
    ImGui_ImplVulkan_Init(&init_info, this->MainWindowData.RenderPass);
}

void Render::RebuildGLFWwindow()
{
    this->WindowShouldClose = glfwWindowShouldClose(window);
    this->draw_data = ImGui::GetDrawData();
    this->WindowShouldMinimized = (this->draw_data->DisplaySize.x <= 0.0f || this->draw_data->DisplaySize.y <= 0.0f);
}

void Render::RebuildSwapChain()
{
    // Resize swap chain?
    if (this->_SwapChainRebuild)
    {
        glfwGetFramebufferSize(window, &this->GLFWwidth, &this->GLFWheight);
        if (this->GLFWwidth > 0 && this->GLFWheight > 0)
        {
            ImGui_ImplVulkan_SetMinImageCount(this->_MinImageCount);
            ImGui_ImplVulkanH_CreateOrResizeWindow(this->_Instance, this->PhysicalDevice, this->Device, &this->MainWindowData, this->_QueueFamily, this->_Allocator, this->GLFWwidth, this->GLFWheight, this->_MinImageCount);
            this->MainWindowData.FrameIndex = 0;
            this->_SwapChainRebuild = false;
        }
    }
}

void Render::FrameRender()
{
    VkResult err;

    VkSemaphore image_acquired_semaphore = this->MainWindowData.FrameSemaphores[this->MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = this->MainWindowData.FrameSemaphores[this->MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(this->Device, this->MainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &this->MainWindowData.FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        this->_SwapChainRebuild = true;
        return;
    }
    VK_error_callback(err);

    ImGui_ImplVulkanH_Frame* fd = &this->MainWindowData.Frames[this->MainWindowData.FrameIndex];
    {
        err = vkWaitForFences(this->Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        VK_error_callback(err);

        err = vkResetFences(this->Device, 1, &fd->Fence);
        VK_error_callback(err);
    }
    {
        err = vkResetCommandPool(this->Device, fd->CommandPool, 0);
        VK_error_callback(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        VK_error_callback(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = this->MainWindowData.RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = this->MainWindowData.Width;
        info.renderArea.extent.height = this->MainWindowData.Height;
        info.clearValueCount = 1;
        info.pClearValues = &this->MainWindowData.ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(this->draw_data, fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        VK_error_callback(err);
        err = vkQueueSubmit(this->Queue, 1, &info, fd->Fence);
        VK_error_callback(err);
    }
}

void Render::FramePresent()
{
    if (this->_SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = this->MainWindowData.FrameSemaphores[this->MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &this->MainWindowData.Swapchain;
    info.pImageIndices = &this->MainWindowData.FrameIndex;
    VkResult err = vkQueuePresentKHR(this->Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        this->_SwapChainRebuild = true;
        return;
    }
    VK_error_callback(err);
    this->MainWindowData.SemaphoreIndex = (this->MainWindowData.SemaphoreIndex + 1) % this->MainWindowData.ImageCount; // Now we can use the next set of semaphores
}

void Render::BackgroundRender(ImVec4 clear_color)
{
    if (!this->WindowShouldMinimized)
    {
        this->MainWindowData.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        this->MainWindowData.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        this->MainWindowData.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        this->MainWindowData.ClearValue.color.float32[3] = clear_color.w;
    }
}

void Render::CleanupVulkan()
{
    ImGui_ImplVulkanH_DestroyWindow(this->_Instance, this->Device, &this->MainWindowData, this->_Allocator);
    vkDestroyDescriptorPool(this->Device, this->_DescriptorPool, this->_Allocator);

#ifdef IMGUI_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(this->_Instance, "vkDestroyDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT(this->_Instance, this->_DebugReport, this->_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

    vkDestroyDevice(this->Device, this->_Allocator);
    vkDestroyInstance(this->_Instance, this->_Allocator);
}

void Render::ImGuiCustomStyle()
{
    this->style->DisplayWindowPadding = ImVec2(15, 15);
    this->style->WindowRounding = 5.0f;
    this->style->FramePadding = ImVec2(5, 5);
    this->style->FrameRounding = 4.0f;
    this->style->ItemSpacing = ImVec2(12, 8);
    this->style->ItemInnerSpacing = ImVec2(8, 6);
    this->style->IndentSpacing = 25.0f;
    this->style->ScrollbarSize = 15.0f;
    this->style->ScrollbarRounding = 9.0f;
    this->style->GrabMinSize = 5.0f;
    this->style->GrabRounding = 3.0f;

    //Window Background
    ImVec4 Colorset_1 = ImVec4(0.141f, 0.364f, 0.376f, 1.00f);
    //Disabled
    ImVec4 Colorset_2 = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    //Active Title
    ImVec4 Colorset_3 = ImVec4(0.235f, 0.603f, 0.623f, 1.00f);
    //General Title, Button
    ImVec4 Colorset_4 = ImVec4(0.180f, 0.462f, 0.478f, 1.00f);
    //ScrollBar
    ImVec4 Colorset_5 = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    //Active Button
    ImVec4 Colorset_6 = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

    this->style->Colors[ImGuiCol_Text] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
    this->style->Colors[ImGuiCol_TextDisabled] = Colorset_2;
    this->style->Colors[ImGuiCol_WindowBg] = Colorset_1;
    this->style->Colors[ImGuiCol_PopupBg] = Colorset_1;
    this->style->Colors[ImGuiCol_Border] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
    this->style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    this->style->Colors[ImGuiCol_FrameBg] = Colorset_4;
    this->style->Colors[ImGuiCol_FrameBgHovered] = Colorset_2;
    this->style->Colors[ImGuiCol_FrameBgActive] = Colorset_6;
    this->style->Colors[ImGuiCol_TitleBg] = Colorset_4;
    this->style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    this->style->Colors[ImGuiCol_TitleBgActive] = Colorset_3;
    this->style->Colors[ImGuiCol_MenuBarBg] = Colorset_4;
    this->style->Colors[ImGuiCol_ScrollbarBg] = Colorset_1;
    this->style->Colors[ImGuiCol_ScrollbarGrab] = Colorset_5;
    this->style->Colors[ImGuiCol_ScrollbarGrabHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_ScrollbarGrabActive] = Colorset_5;
    this->style->Colors[ImGuiCol_CheckMark] = Colorset_5;
    this->style->Colors[ImGuiCol_SliderGrab] = Colorset_5;
    this->style->Colors[ImGuiCol_SliderGrabActive] = Colorset_1;
    this->style->Colors[ImGuiCol_Button] = Colorset_4;
    this->style->Colors[ImGuiCol_ButtonHovered] = Colorset_2;
    this->style->Colors[ImGuiCol_ButtonActive] = Colorset_6;
    this->style->Colors[ImGuiCol_Header] = Colorset_4;
    this->style->Colors[ImGuiCol_HeaderHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_HeaderActive] = Colorset_1;
    this->style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    this->style->Colors[ImGuiCol_ResizeGripHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_ResizeGripActive] = Colorset_1;
    this->style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    this->style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    this->style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    this->style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    this->style->Colors[ImGuiCol_TextSelectedBg] = Colorset_3;
    this->style->Colors[ImGuiCol_Tab] = Colorset_4;
    this->style->Colors[ImGuiCol_TabActive] = Colorset_3;
}

void Render::ImGuiClientStyle()
{
    this->style->DisplayWindowPadding = ImVec2(15, 15);
    this->style->WindowRounding = 5.0f;
    this->style->FramePadding = ImVec2(5, 5);
    this->style->FrameRounding = 4.0f;
    this->style->ItemSpacing = ImVec2(12, 8);
    this->style->ItemInnerSpacing = ImVec2(8, 6);
    this->style->IndentSpacing = 25.0f;
    this->style->ScrollbarSize = 15.0f;
    this->style->ScrollbarRounding = 9.0f;
    this->style->GrabMinSize = 5.0f;
    this->style->GrabRounding = 3.0f;

    //Window Background
    ImVec4 Colorset_1 = ImVec4(25.0/255.0, 25.0/255.0, 25.0/255.0, 1.00f);
    //Disabled
    ImVec4 Colorset_2 = ImVec4(46.0/255.0, 46.0/255.0, 46.0/255.0, 1.00f);
    //Active Title
    ImVec4 Colorset_3 = ImVec4(128.0/255.0, 151.0/255.0, 144.0/255.0, 1.00f);
    //General Title, Button
    ImVec4 Colorset_4 = ImVec4(3.0/255.0, 95.0/255.0, 72.0/255.0, 1.00f);
    //ScrollBar
    ImVec4 Colorset_5 = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    //Active Button
    ImVec4 Colorset_6 = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

    this->style->Colors[ImGuiCol_Text] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
    this->style->Colors[ImGuiCol_TextDisabled] = Colorset_2;
    this->style->Colors[ImGuiCol_WindowBg] = Colorset_1;
    this->style->Colors[ImGuiCol_PopupBg] = Colorset_1;
    this->style->Colors[ImGuiCol_Border] = ImVec4(0.972f, 0.733f, 0.701f, 1.00f);
    this->style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    this->style->Colors[ImGuiCol_FrameBg] = Colorset_4;
    this->style->Colors[ImGuiCol_FrameBgHovered] = Colorset_2;
    this->style->Colors[ImGuiCol_FrameBgActive] = Colorset_6;
    this->style->Colors[ImGuiCol_TitleBg] = Colorset_4;
    this->style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    this->style->Colors[ImGuiCol_TitleBgActive] = Colorset_3;
    this->style->Colors[ImGuiCol_MenuBarBg] = Colorset_4;
    this->style->Colors[ImGuiCol_ScrollbarBg] = Colorset_1;
    this->style->Colors[ImGuiCol_ScrollbarGrab] = Colorset_5;
    this->style->Colors[ImGuiCol_ScrollbarGrabHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_ScrollbarGrabActive] = Colorset_5;
    this->style->Colors[ImGuiCol_CheckMark] = Colorset_5;
    this->style->Colors[ImGuiCol_SliderGrab] = Colorset_5;
    this->style->Colors[ImGuiCol_SliderGrabActive] = Colorset_1;
    this->style->Colors[ImGuiCol_Button] = Colorset_4;
    this->style->Colors[ImGuiCol_ButtonHovered] = Colorset_2;
    this->style->Colors[ImGuiCol_ButtonActive] = Colorset_6;
    this->style->Colors[ImGuiCol_Header] = Colorset_4;
    this->style->Colors[ImGuiCol_HeaderHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_HeaderActive] = Colorset_1;
    this->style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    this->style->Colors[ImGuiCol_ResizeGripHovered] = Colorset_6;
    this->style->Colors[ImGuiCol_ResizeGripActive] = Colorset_1;
    this->style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    this->style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    this->style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    this->style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    this->style->Colors[ImGuiCol_TextSelectedBg] = Colorset_3;
    this->style->Colors[ImGuiCol_Tab] = Colorset_4;
    this->style->Colors[ImGuiCol_TabActive] = Colorset_3;
    this->style->Colors[ImGuiCol_TableHeaderBg] = Colorset_4;
}