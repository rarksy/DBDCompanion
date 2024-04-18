#include "Overlay.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "Features/Crosshair/Crosshair.h"
#include "Features/OnScreenTimers/OnScreenTimers.hpp"


bool overlay::is_overlay_needed()
{
    return (cvars.enabled || onscreen_timers::enabled);
}

bool overlay::is_overlay_created()
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
