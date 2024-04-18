﻿#pragma once
#include "GLFW/glfw3.h"
#include "Menu/Menu.hpp"

namespace overlay
{
    inline int window_width;
    inline int window_height;

    inline GLFWwindow* window = nullptr;
    inline ImGuiContext* context = nullptr;

    bool is_overlay_needed();
    bool is_overlay_created();
    void create_overlay();
    void destroy_overlay();
}