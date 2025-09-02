#pragma once
#ifndef _SGS_RENDERING_H_
#define _SGS_RENDERING_H_

#ifndef _SGS_INCLUDE_H_
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#endif


void GLFW_error_callback(int error, const char* description);
void VK_error_callback(VkResult err);

class Render
{
private:
	VkAllocationCallbacks*   _Allocator = nullptr;
	VkInstance               _Instance = VK_NULL_HANDLE;
	
	
	uint32_t                 _QueueFamily = (uint32_t)-1;
	
	VkDebugReportCallbackEXT _DebugReport = VK_NULL_HANDLE;
	VkPipelineCache          _PipelineCache = VK_NULL_HANDLE;
	VkDescriptorPool         _DescriptorPool = VK_NULL_HANDLE;
	int                      _MinImageCount = 2;
	bool                     _SwapChainRebuild = false;
	int Winwidth;
	int Winheight;

public:
	GLFWwindow* window;
	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
	ImGui_ImplVulkanH_Window MainWindowData;
	VkSurfaceKHR			surface;
	VkPhysicalDevice        PhysicalDevice = VK_NULL_HANDLE;
	VkDevice                Device = VK_NULL_HANDLE;
	VkQueue                 Queue = VK_NULL_HANDLE;
	ImGui_ImplVulkan_InitInfo init_info = {};
	ImVector<const char*> extensions;
	ImDrawData* draw_data;
	int GLFWwidth;
	int GLFWheight;
	bool WindowShouldClose = false;
	bool WindowShouldMinimized = false;
	ImGuiStyle* style;

	float width;
	float height;


	Render(const char* title);
	~Render();
	void SetExtensions();
	void SetupVulkan();
	bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
	VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
	void SetupVulkanWindow();
	void Impl_Vulkan_Init();
	void RebuildGLFWwindow();
	void RebuildSwapChain();
	void FrameRender();
	void FramePresent();
	void BackgroundRender(ImVec4 clear_color);
	void CleanupVulkan();
	void ImGuiCustomStyle();
	void ImGuiClientStyle();
};

#endif