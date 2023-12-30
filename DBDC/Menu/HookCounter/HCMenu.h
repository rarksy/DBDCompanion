#pragma once
#include "GLFW/glfw3.h"
#include <ImGui/imgui.h>

namespace HCMenu
{
    void RenderUI();
    void CreateStyle();

    inline ImGuiContext* overlayContext = nullptr;
    inline GLFWwindow* overlayWindow = nullptr;
    inline bool hasCreatedOverlay = false;
}
