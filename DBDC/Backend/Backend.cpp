#include "Backend.hpp"
#include "Fonts/Rethink.hpp"
#include "../Features/Menu.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

int backend::init_glfw()
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

void backend::shutdown_glfw()
{
    glfwDestroyWindow(menu::main_window);
    glfwTerminate();
}

GLFWwindow* backend::setup_window(const char* title, int width, int height)
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

void backend::setup_imgui(GLFWwindow* window, ImGuiContext*& context)
{
    // Setup ImGui
    context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    auto& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, menu::styling::font_size);
    menu::styling::child_font = io.Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, 16.F);
    io.IniFilename = nullptr;
}

void backend::shutdown_imgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}