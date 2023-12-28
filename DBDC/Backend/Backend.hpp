#pragma once
#include <GLFW/glfw3.h>

namespace Backend
{
    int InitGLFW();
    GLFWwindow* SetupWindow(const char* title, int width, int height);
    bool SetupImageTextures();
}
