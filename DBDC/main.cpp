#include "Backend/Backend.hpp"
#include "Images/Images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Menu/Menu.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "ImGui/imgui_impl_glfw.h"
#include "miscLIB/miscLIB.hpp"
#include "Overlay/Overlay.hpp"

//#define debug_winmain
#ifndef debug_winmain

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    const char* mutex_name = "com_rarksy_apps_deadbydaylightcompanion";
    HANDLE mutex_handle = CreateMutexA(NULL, TRUE, mutex_name);
    if (ERROR_ALREADY_EXISTS == GetLastError())
        return -1;
    
    if (!backend::init_glfw())
        return -2;

    backend::screen_width = ml::get_screen_width();
    backend::screen_height = ml::get_screen_height();

    overlay::window_width = backend::screen_width;
    overlay::window_height = backend::screen_height;

    char pathBuffer[MAX_PATH];
    GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
    backend::exe_name = std::filesystem::path(pathBuffer).filename().string();
    backend::exe_directory = std::filesystem::path(pathBuffer).parent_path();

    ml::create_directory(backend::exe_directory.string() + backend::settings_directory + backend::data_directory);

    menu::main_window = backend::setup_window("Dead By Daylight Companion", menu::styling::menu_width, menu::styling::menu_height);

    if (!menu::main_window)
        return -3;

    int width, height, channels;
    unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
    const GLFWimage exeIcon = {width, height, iconData};
    glfwSetWindowIcon(menu::main_window, 1, &exeIcon);

    backend::setup_imgui(menu::main_window, menu::main_context);

    std::thread update_thread([]
    {
        backend::update_available = backend::check_for_update();

        if (ml::file_or_directory_exists(backend::exe_directory.string() + "\\update.bat"))
            std::filesystem::remove(backend::exe_directory.string() + "\\update.bat");
    });
    update_thread.detach();
    
    images::load_all();
    
    menu::run_loop();

    if (overlay::window != nullptr)
    {
        overlay::destroy_overlay();
        ImGui::SetCurrentContext(overlay::context);
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext(overlay::context);
    }

    ImGui::SetCurrentContext(menu::main_context);

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(menu::main_context);

    glfwDestroyWindow(menu::main_window);

    glfwTerminate();

    ReleaseMutex(mutex_handle);
    CloseHandle(mutex_handle);

    return 0;
}
#else

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int) // alternate winmain used for testing
{

    return 0;
}

#endif
