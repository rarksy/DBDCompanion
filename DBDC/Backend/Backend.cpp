#include "Backend.hpp"
#include <iostream>
#include "Fonts/Rethink.hpp"
#include "../Menu/Menu.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

static void GLFWErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error: " << error << "Description: " << description;
}

int Backend::InitGLFW()
{
    glfwSetErrorCallback(GLFWErrorCallback);

    return glfwInit();
}

void Backend::ShutdownGLFW()
{
    glfwDestroyWindow(Menu::mainWindow);
    glfwTerminate();
}

GLFWwindow* Backend::SetupWindow(const char* title, int width, int height)
{
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void Backend::SetupImGui()
{
    // Setup ImGui
    Menu::mainContext = ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Menu::mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330"); 
    
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, Menu::Styling::fontSize);
    
    ImGui::GetIO().IniFilename = nullptr;
}

void Backend::ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

