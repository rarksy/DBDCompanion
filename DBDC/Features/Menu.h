#pragma once
#include <nlohmann/json.hpp>

#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "Menu.h"
#include "stb_image.h"
#include "../Backend/Backend.hpp"
#include "Exe Icons/256x256.hpp"
#include <ctime>
#include "GUI/GUI.h"

struct color
{
    int r;
    int g;
    int b;
    int a = 255;

    color() : r(0), g(0), b(0)
    {
    }

    color(int _r, int _g, int _b, int _a = 255) : r(_r), g(_g), b(_b), a(_a)
    {
    }

    ImVec4 to_imvec4()
    {
        return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
    }

    ImColor to_imcolor()
    {
        return ImColor(r, g, b, a);
    }

    void apply_from_imcolor(const ImColor& imcolor)
    {
        this->r = static_cast<int>(imcolor.Value.x);
        this->g = static_cast<int>(imcolor.Value.y);
        this->b = static_cast<int>(imcolor.Value.z);
        this->a = static_cast<int>(imcolor.Value.w);
    }

    void apply_from_imvec4(const ImVec4& imvec4)
    {
        this->r = static_cast<int>(imvec4.x) * 255;
        this->g = static_cast<int>(imvec4.y) * 255;
        this->b = static_cast<int>(imvec4.z) * 255;
        this->a = static_cast<int>(imvec4.w) * 255;
    }

    ImColor* as_imcolor()
    {
        static ImColor im_color;
        im_color = this->to_imcolor();
        return &im_color;
    }

    ImVec4* as_imvec4()
    {
        static ImVec4 im_vec4;
        im_vec4 = this->to_imvec4();
        return &im_vec4;
    }
};

namespace menu
{
    void run_loop();
    void render_ui();
    void create_global_style();

    inline GLFWwindow* main_window = nullptr;
    inline ImGuiContext* main_context = nullptr;

    // namespace shrine_of_secrets
    // {
    //     inline bool obtained_perk_data = false;
    //     inline bool is_ready = false;
    //     
    //     inline nlohmann::json shrine_data = nullptr;
    //     inline std::vector<nlohmann::json> perk_data = {0, 0, 0, 0};
    //
    //     inline std::vector<const char*> perk_image_paths;
    //     inline std::vector<GLuint> perk_textures = {0, 0, 0, 0};
    //     
    //     inline time_t reset_time_start = 0;
    //     inline time_t reset_time_end = 0;
    // }

    namespace overlay
    {
        inline int window_width;
        inline int window_height;

        inline GLFWwindow* window = nullptr;
        inline ImGuiContext* context = nullptr;


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

        inline constexpr float font_size = 22.F;

        inline color menu_accent = color(255, 83, 83);
    }

    namespace icons
    {
        inline GLuint config_editor;
    }


    inline int menu_to_show = 0;

    inline constexpr ImGuiWindowFlags menu_flags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove;
}
