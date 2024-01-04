#include <iostream>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <Windows.h>
#include "Config/Config.h"
#include "Backend/Backend.hpp"
#include "Menu/Menu.h"
#include <Fonts/Rethink.hpp>
#include "Scripts/Scripts.hpp"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    
    if (Backend::InitGLFW() != GLFW_TRUE)
        return 2;
    
    // Create GLFW Window
    GLFWwindow* window = Backend::SetupWindow("Dead By Daylight Companion", Menu::Styling::menuWidth,
                                              Menu::Styling::menuHeight);
    if (!window)
        return 3;
    
    Menu::window = window;
    
    glfwMakeContextCurrent(window);
    
    // Create ImGui Context
    Menu::mainContext = ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); 
    
    // Load Font
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Rethink_compressed_data, Rethink_compressed_size, 18.F);
    
    ImGui::GetIO().IniFilename = nullptr;
    
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
    glfwSwapInterval(2);
    
    Menu::RunLoop();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
