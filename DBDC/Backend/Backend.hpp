#pragma once
#include <GLFW/glfw3.h>

namespace Backend
{
    int InitGLFW();
    void ShutdownGLFW();
    
    GLFWwindow* SetupWindow(const char* title, int width, int height);
    void SetupImGui();
    
    bool SetupImageTextures();

    void ShutdownImGui();
}
