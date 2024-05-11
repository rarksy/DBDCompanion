#include "backend/backend.hpp"
#include "images/images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "menu/menu.hpp"
#include "stb_image.h"
#include "miscLIB/miscLIB.hpp"
#include "overlay/overlay.hpp"

//#define debug_winmain
#ifndef debug_winmain

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    backend::create_mutex();

    backend::init();

    if (!backend::init_glfw())
        return -1;

    menu::main_window = backend::setup_window("Dead By Daylight Companion", menu::styling::menu_width, menu::styling::menu_height);
    
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

    if (overlay::is_overlay_active())
    {
        overlay::destroy_overlay();
        ImGui::SetCurrentContext(overlay::context);
        backend::shutdown_imgui();
    }

    ImGui::SetCurrentContext(menu::main_context);

    backend::shutdown_imgui();
    backend::shutdown_glfw();
    backend::destroy_mutex();

    return 0;
}
#else

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int) // alternate winmain used for testing
{

    return 0;
}

#endif
