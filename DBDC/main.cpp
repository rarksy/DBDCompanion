#include <Windows.h>
#include "Backend/Backend.hpp"
#include "Menu/Menu.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    // Setup GLFW Window
    if (Backend::InitGLFW() != GLFW_TRUE)
        return 2;
    
    GLFWwindow* window = Backend::SetupWindow("Dead By Daylight Companion", Menu::Styling::menuWidth,
                                              Menu::Styling::menuHeight);
    if (!window)
        return 3;
    
    Menu::mainWindow = window;
    
    glfwMakeContextCurrent(window);

    Backend::SetupImGui();
    
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
    glfwSwapInterval(2);

    // Start Menu Loop
    Menu::RunLoop();

    Backend::ShutdownImGui();
    Backend::ShutdownGLFW();
    
    return 0;
}
