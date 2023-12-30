#pragma once
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

namespace Menu
{
    void RunLoop();
    void RenderUI();

    inline GLFWwindow* window = nullptr;

    namespace Styling
    {
        inline constexpr int menuWidth = 750;
        inline constexpr int menuHeight = 500;
        inline constexpr int itemWidth = 120;   
    }

    inline ImGuiContext* mainContext = nullptr;

    inline int menuToShow = 1;
    
    inline constexpr ImGuiWindowFlags menuFlags =
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoMove;
}
