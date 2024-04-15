#pragma once

#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include "stb_image.h"
#include "../Backend/Backend.hpp"
#include "Exe Icons/256x256.hpp"

#include "Crosshair/Crosshair.h"
#include "OnScreenTimers/OnScreenTimers.hpp"
#include "miscLIB/miscLIB.hpp"
#include "color/color.hpp"

namespace menu
{
    void run_loop();
    void render_ui();
    void create_global_style();

    inline GLFWwindow* main_window = nullptr;
    inline ImGuiContext* main_context = nullptr;

    namespace overlay
    {
        inline int window_width;
        inline int window_height;

        inline GLFWwindow* window = nullptr;
        inline ImGuiContext* context = nullptr;

        inline bool is_overlay_needed()
        {
            return (cvars.enabled || onscreen_timers::enabled);
        }

        inline bool is_overlay_created()
        {
            return window != nullptr;
        }

        inline void create_overlay()
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

            backend::setup_imgui(menu::overlay::window, menu::overlay::context);
        }

        inline void destroy_overlay()
        {
            glfwDestroyWindow(menu::overlay::window);
            menu::overlay::window = nullptr;
        }
    }

    namespace styling
    {
        inline constexpr int menu_width = 750;
        inline constexpr int menu_height = 500;
        inline constexpr int item_width = 100;

        inline constexpr float font_size = 21.F;
        inline ImFont* main_font = nullptr;
        inline ImFont* child_font = nullptr;

        inline color menu_accent = color(255, 83, 83);
        inline bool show_color_picker = false;
    }

    namespace icons
    {
        inline GLuint help_icon;
        inline GLuint update_icon;
        inline GLuint settings_icon;
        inline GLuint back_icon;
    }


    inline int menu_to_show = 0;

    inline constexpr ImGuiWindowFlags menu_flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
}
