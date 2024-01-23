#pragma once
#include <filesystem>
#include <GLFW/glfw3.h>
#include "ImGui/imgui_internal.h"

namespace Backend
{
    int InitGLFW();
    void ShutdownGLFW();
    
    GLFWwindow* SetupWindow(const char* title, int width, int height);
    void SetupImGui(GLFWwindow* window, ImGuiContext*& context);
    
    bool SetupImageTextures();

    void ShutdownImGui();

    inline int screenWidth;
    inline int screenHeight;
    inline std::filesystem::path exeDirectory;
}
