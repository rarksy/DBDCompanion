#include <cstdio>
#include <Windows.h>
#include "Backend/Backend.hpp"
#include "Images/Images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Menu/Menu.h"
#include "Dependencies/Images/Icons/ConfigEditor.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "ImGui/imgui_impl_glfw.h"
#include "miscLIB/miscLIB.hpp"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    if (!Backend::InitGLFW())
        return -1;

    Backend::screenWidth = ml::get_screen_width();
    Backend::screenHeight = ml::get_screen_height();

    Menu::Overlay::windowWidth = Backend::screenWidth;
    Menu::Overlay::windowHeight = Backend::screenHeight;

    char pathBuffer[MAX_PATH];
    GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
    Backend::exeDirectory = std::filesystem::path(pathBuffer).parent_path();

    ml::create_directory(Backend::exeDirectory.string() + Backend::settingsDirectory);

    Menu::mainWindow = Backend::SetupWindow("Dead By Daylight Companion", Menu::Styling::menuWidth, Menu::Styling::menuHeight);

    if (!Menu::mainWindow)
        return -1;

    int width, height, channels;
    unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
    const GLFWimage exeIcon = {width, height, iconData};
    glfwSetWindowIcon(Menu::mainWindow, 1, &exeIcon);

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
