#include "Backend.hpp"
#include <iostream>
#include "Fonts/Rethink.hpp"
#include "../Menu/Menu.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

int Backend::InitGLFW()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Error: Failed to initialize GLFW.\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    return true;
}

void Backend::ShutdownGLFW()
{
    glfwDestroyWindow(Menu::mainWindow);
    glfwTerminate();
}

GLFWwindow* Backend::SetupWindow(const char* title, int width, int height)
{
    glfwWindowHint(GLFW_RESIZABLE, false);
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Error: Failed to create GLFW window.\n");
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void Backend::SetupImGui(GLFWwindow* window, ImGuiContext*& context)
{
    // Setup ImGui
    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    auto& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, Menu::Styling::fontSize);
    io.IniFilename = nullptr;
}

void Backend::ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}