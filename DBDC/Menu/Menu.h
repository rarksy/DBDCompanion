﻿#pragma once
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include "stb_image.h"
#include "../Backend/Backend.hpp"
#include "Exe Icons/256x256.hpp"
#include "GUI/GUI.h"

struct Color
{
    int r;
    int g;
    int b;
    int a = 255;
    
    Color() : r(0), g(0), b(0) {}
    Color(int _r, int _g, int _b, int _a = 255) : r(_r), g(_g), b(_b), a(_a) {}

    ImVec4 ToImVec4()
    {
        return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
    }

    ImColor ToImColor()
    {
        return ImColor(r, g, b);
    }

    void ApplyFromImColor(const ImColor& imcolor)
    {
        this->r = imcolor.Value.x;
        this->g = imcolor.Value.y;
        this->b = imcolor.Value.z;
        this->a = imcolor.Value.w;
    }

    void ApplyFromImVec4(const ImVec4& imvec4)
    {
        this->r = imvec4.x * 255;
        this->g = imvec4.y * 255;
        this->b = imvec4.z * 255;
        this->a = imvec4.w * 255;
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

            int width, height, channels;
            unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
            GLFWimage exeIcon = {width, height, iconData};
            glfwSetWindowIcon(Overlay::window, 1, &exeIcon);

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
