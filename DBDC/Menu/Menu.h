#pragma once
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include <Windows.h>

namespace Menu
{
    void RunLoop();
    void RenderUI();
    void CreateGlobalStyle();

    inline GLFWwindow* mainWindow = nullptr;

    namespace Overlay
    {
        inline GLFWwindow* window = nullptr;
        inline ImGuiContext* context = nullptr;
        
        
        inline bool IsOverlayCreated()
        {
            return window != nullptr;
        }

        inline void CreateOverlay()
        {
            const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
            glfwWindowHint(GLFW_DECORATED, false);
            glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
            glfwWindowHint(GLFW_FLOATING, true);
            Overlay::window = glfwCreateWindow(screenWidth / 3, screenHeight, "Hook Counter", nullptr, nullptr);
            glfwSetWindowPos(Overlay::window, 0, 0);
            glfwFocusWindow(mainWindow);

            Overlay::context = ImGui::CreateContext();
        }

        inline void DestroyOverlay()
        {
            glfwDestroyWindow(Overlay::window);
            ImGui::DestroyContext(Overlay::context);
            Overlay::window = nullptr;
        }
    }

    namespace Styling
    {
        inline constexpr int menuWidth = 750;
        inline constexpr int menuHeight = 500;
        inline constexpr int itemWidth = 95;

        inline constexpr float fontSize = 22.F;
    }

    inline ImGuiContext* mainContext = nullptr;

    inline int menuToShow = 0;
    
    inline constexpr ImGuiWindowFlags menuFlags =
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoMove;
}
