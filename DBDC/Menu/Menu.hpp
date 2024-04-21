#pragma once
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "color/color.hpp"

namespace menu
{
    void run_loop();
    void render_ui();
    void create_global_style();

    inline GLFWwindow* main_window = nullptr;
    inline ImGuiContext* main_context = nullptr;

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
        inline GLuint config_editor_icon;
        inline GLuint icon_packager_icon;
        inline GLuint hook_tracker_icon;
        inline GLuint crosshair_overlay_icon;
        inline GLuint on_screen_timers_icon;
        
        inline GLuint help_icon;
        inline GLuint update_icon;
        inline GLuint settings_icon;
        inline GLuint back_icon;
        
        inline GLuint discord_icon;
    }


    inline int menu_to_show = 0;

    inline constexpr ImGuiWindowFlags menu_flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
}
