#include "Backend.hpp"
#include <iostream>

static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << error << "Description: " << description;
}

int Backend::InitGLFW()
{
    glfwSetErrorCallback(GLFWErrorCallback);

    return glfwInit();
}

GLFWwindow* Backend::SetupWindow(const char* title, int width, int height)
{
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

