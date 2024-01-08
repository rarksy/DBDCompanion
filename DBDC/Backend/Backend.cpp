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

void Backend::ProcessInput(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();

    // Update ImGui input state
    io.DeltaTime = 1.0f / 60.0f; // You may adjust this based on your frame rate

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

    int leftMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    io.MouseDown[0] = leftMouseState == GLFW_PRESS;

    int rightMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    io.MouseDown[1] = rightMouseState == GLFW_PRESS;

    // Clear previous input characters
    io.ClearInputCharacters();

    // Other mouse buttons can be handled similarly

    // Modifiers (Ctrl, Shift, Alt)
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}
