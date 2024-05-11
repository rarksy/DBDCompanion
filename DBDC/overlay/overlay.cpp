#include "overlay.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "features/crosshair/crosshair.h"
#include "..\features\on_screen_timers\on_screen_timers.hpp"
#include <Windows.h>


bool overlay::is_overlay_needed()
{
    return (cvars.enabled || onscreen_timers::enabled);
}

bool overlay::is_overlay_active()
{
    return window != nullptr;
}

void overlay::create_overlay()
{
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    overlay::window = backend::setup_window("Overlay", backend::screen_width, backend::screen_height);
    glfwSwapBuffers(overlay::window);

    glfwSetWindowPos(overlay::window, 0, 0);
    glfwFocusWindow(menu::main_window);

    int width, height, channels;
    unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
    GLFWimage exeIcon = {width, height, iconData};
    glfwSetWindowIcon(overlay::window, 1, &exeIcon);

    backend::setup_imgui(window, context);
}

void overlay::destroy_overlay()
{
    glfwDestroyWindow(window);
    window = nullptr;
}

bool overlay::detect_keypress(int key)
{
    bool key_pressed = false;
    for (int i = 0; i < 256; ++i)
    {
        if (!(GetAsyncKeyState(i) & 1))
            continue;

        if (i != key)
            continue;

        key_pressed = true;
    }

    return key_pressed;
}
