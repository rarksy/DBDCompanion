#include <cstdio>
#include <iostream>
#include <Windows.h>
#include "Backend/Backend.hpp"
#include "Images/Images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Menu/Menu.h"
#include "Dependencies/Images/Icons/ConfigEditor.hpp"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    if (!Backend::InitGLFW())
    {
        return -1;
    }
    
    Backend::screenWidth = GetSystemMetrics(SM_CXSCREEN);
    Backend::screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    Menu::Overlay::windowWidth = Backend::screenWidth;
    Menu::Overlay::windowHeight = Backend::screenHeight;
    
    Menu::mainWindow = Backend::SetupWindow("Dead By Daylight Companion", Menu::Styling::menuWidth,
                                            Menu::Styling::menuHeight);
    
    if (!Menu::mainWindow)
    {
        return -1;
    }
    
    Backend::SetupImGui(Menu::mainWindow, Menu::mainContext);
    
    Images::LoadTextureFromMemory(configEditorIconRawData, sizeof configEditorIconRawData, &Menu::Icons::ConfigEditor);
    
    Menu::RunLoop();
    
    if (Menu::Overlay::window != nullptr)
    {
        Menu::Overlay::DestroyOverlay();
        ImGui::SetCurrentContext(Menu::Overlay::context);
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext(Menu::Overlay::context);
    }
    
    ImGui::SetCurrentContext(Menu::mainContext);
    
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(Menu::mainContext);
    
    glfwDestroyWindow(Menu::mainWindow);
    
    glfwTerminate();
    
    return 0;
}
        
       