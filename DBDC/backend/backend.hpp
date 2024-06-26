﻿#pragma once
#include <filesystem>
#include <GLFW/glfw3.h>

#include <Windows.h>
#include "ImGui/imgui_internal.h"

#define DBDC_VERSION std::string("master_v0.10")

namespace backend
{
    void init();
    
    inline HANDLE dbdc_mutex_handle;
    const std::string dbdc_mutex_label = "com_rarksy_apps_deadbydaylightcompanion";
    bool create_mutex();
    bool destroy_mutex();
    
    int init_glfw();
    void shutdown_glfw();
    
    GLFWwindow* setup_window(const char* title, int width, int height);
    void setup_imgui(GLFWwindow* window, ImGuiContext*& context);

    bool check_for_update();
    bool update();
    
    bool setup_image_textures();

    void shutdown_imgui();

    inline bool update_available = false;
    inline std::string updated_binary_url;

    inline int screen_width;
    inline int screen_height;
    inline std::string exe_name;
    inline std::filesystem::path exe_directory;
    inline const std::string settings_directory = "\\DBDC\\";
    inline const std::string data_directory = "data\\";
    
}
