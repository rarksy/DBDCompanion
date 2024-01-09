#pragma once
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include <Windows.h>

#include "../Backend/Backend.hpp"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

namespace Menu
{
    void RunLoop();
    void RenderUI();
    void CreateGlobalStyle();

    inline GLFWwindow* mainWindow = nullptr;
    inline ImGuiContext* mainContext = nullptr;

    namespace Overlay
    {
        inline int windowWidth;
        inline int windowHeight;
        
        inline GLFWwindow* window = nullptr;
        inline ImGuiContext* context = nullptr;


        inline bool IsOverlayCreated()
        {
            return window != nullptr;
        }

        inline void CreateOverlay()
        {
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
            glfwWindowHint(GLFW_DECORATED, false);
            glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
            glfwWindowHint(GLFW_FLOATING, true);
            glfwWindowHint(GLFW_MAXIMIZED, true);
            Overlay::window = Backend::SetupWindow("Overlay", Backend::screenWidth, Backend::screenHeight);
            glfwSwapBuffers(Overlay::window);

            glfwSetWindowPos(Overlay::window, 0, 0);

            Backend::SetupImGui(Menu::Overlay::window, Menu::Overlay::context);
        }

        inline void DestroyOverlay()
        {
            glfwDestroyWindow(Menu::Overlay::window);
            Menu::Overlay::window = nullptr;
        }
    }

    namespace Styling
    {
        inline constexpr int menuWidth = 750;
        inline constexpr int menuHeight = 500;
        inline constexpr int itemWidth = 95;

        inline constexpr float fontSize = 22.F;
    }

    namespace Icons
    {
        inline GLuint ConfigEditor;
    }


    inline int menuToShow = 0;

    inline constexpr ImGuiWindowFlags menuFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove;
}
