#include <cstdio>
#include <Windows.h>
#include "Backend/Backend.hpp"
#include "Images/Images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Features/Menu.h"
#include "Dependencies/Images/Icons/ConfigEditor.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "ImGui/imgui_impl_glfw.h"
#include "miscLIB/miscLIB.hpp"
#include <opencv2/opencv.hpp>

#include "Misc/Misc.hpp"

//#define alternate_winmain

#ifndef alternate_winmain

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    if (!backend::init_glfw())
        return -1;
    
    backend::screen_width = ml::get_screen_width();
    backend::screen_height = ml::get_screen_height();
    
    menu::overlay::window_width = backend::screen_width;
    menu::overlay::window_height = backend::screen_height;
    
    char pathBuffer[MAX_PATH];
    GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
    backend::exe_directory = std::filesystem::path(pathBuffer).parent_path();
    
    ml::create_directory(backend::exe_directory.string() + backend::settings_directory);
    
    menu::main_window = backend::setup_window("Dead By Daylight Companion", menu::styling::menu_width, menu::styling::menu_height);
    
    if (!menu::main_window)
        return -1;
    
    int width, height, channels;
    unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
    const GLFWimage exeIcon = {width, height, iconData};
    glfwSetWindowIcon(menu::main_window, 1, &exeIcon);
    
    backend::setup_imgui(menu::main_window, menu::main_context);
    
    images::load_texture_from_memory(configEditorIconRawData, sizeof configEditorIconRawData, &menu::icons::config_editor);
     
    menu::run_loop();
    
    if (menu::overlay::window != nullptr)
    {
        menu::overlay::destroy_overlay();
        ImGui::SetCurrentContext(menu::overlay::context);
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext(menu::overlay::context);
    }
    
    ImGui::SetCurrentContext(menu::main_context);
    
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(menu::main_context);
    
    glfwDestroyWindow(menu::main_window);
    
    glfwTerminate();
    
    return 0;
}
#else


int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int) // alternate winmain used for testing
{

    menu::shrine_of_secrets shrine;

    std::cout << shrine.get_perk(0).name << std::endl;
    
    // menu::shrine_of_secrets::shrine_data = ml::json_get("https://dbd.tricky.lol/api/shrine?includeperkinfo=1");
    //
    // for (int i = 0; i < 4; i++)
    //     menu::shrine_of_secrets::perk_data[i] = ml::json_get(
    //         "https://dbd.tricky.lol/api/perkinfo?perk=" + menu::shrine_of_secrets::shrine_data["perks"][i]["id"].get_ref<std::string&>());
    //
    // menu::shrine_of_secrets::is_ready = true;
    //
    // for (int i = 0; i < 4; i++)
    // {
    //     images::load_texture_from_file(misc::get_game_root_directory() + "DeadByDaylight/Content/" + menu::shrine_of_secrets::perk_data[i]["image"].get_ref<std::string&>(),
    //                                    &menu::shrine_of_secrets::perk_textures[0]);
    //     std::cout << "tid: " << menu::shrine_of_secrets::perk_textures[i] << std::endl;
    // }
    //
    // while (!menu::shrine_of_secrets::is_ready);
    return 0;
}

#endif
