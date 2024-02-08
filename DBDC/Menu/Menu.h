#pragma once
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include "../Backend/Backend.hpp"
#include "GUI/GUI.h"

struct Color
{
    int r;
    int g;
    int b;
    int a = 255;

    Color(int _r, int _g, int _b, int _a = 255) : r(_r), g(_g), b(_b), a(_a) {}

    ImVec4 ToImVec4()
    {
        return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
    }

    ImColor ToImColor()
    {
        return ImColor(r, g, b);
    }
};

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
            glfwFocusWindow(Menu::mainWindow);

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
        inline constexpr int itemWidth = 100;

        inline constexpr float fontSize = 22.F;

        inline Color menuAccent = Color(255, 83, 83);
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
