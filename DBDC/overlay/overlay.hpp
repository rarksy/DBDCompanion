#pragma once
#include "GLFW/glfw3.h"
#include "menu/menu.hpp"

namespace overlay
{
    inline int window_width;
    inline int window_height;

    inline GLFWwindow* window = nullptr;
    inline ImGuiContext* context = nullptr;

    bool is_overlay_needed();
    bool is_overlay_active();
    void create_overlay();
    void destroy_overlay();

    bool detect_keypress(int key);
}
