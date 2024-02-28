#include <cstdio>
#include <Windows.h>
#include "Backend/Backend.hpp"
#include "Images/Images.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Features/Menu.h"
#include "Dependencies/Images/Icons/ConfigEditor.hpp"
#include "stb_image.h"
#include "Exe Icons/256x256.hpp"
#include "ImGui/imgui_impl_glfw.h"
#include "miscLIB/miscLIB.hpp"
#include <opencv2/opencv.hpp>

#include "Features/HookTracker/HookTracker.hpp"
#include "Images/HookTracker/Hook.hpp"
#include "Misc/Misc.hpp"

//#define debug_winmain

#ifndef debug_winmain

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int)
{
    if (!backend::init_glfw())
        return -1;
    
    backend::screen_width = ml::get_screen_width();
    backend::screen_height = ml::get_screen_height();
    
    menu::overlay::window_width = backend::screen_width;
    menu::overlay::window_height = backend::screen_height;
    
    char pathBuffer[MAX_PATH];
    GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
    backend::exe_directory = std::filesystem::path(pathBuffer).parent_path();
    
    ml::create_directory(backend::exe_directory.string() + backend::settings_directory);
    
    menu::main_window = backend::setup_window("Dead By Daylight Companion", menu::styling::menu_width, menu::styling::menu_height);
    
    if (!menu::main_window)
        return -1;
    
    int width, height, channels;
    unsigned char* iconData = stbi_load_from_memory(exeIconRawData, sizeof exeIconRawData, &width, &height, &channels, 0);
    const GLFWimage exeIcon = {width, height, iconData};
    glfwSetWindowIcon(menu::main_window, 1, &exeIcon);
    
    backend::setup_imgui(menu::main_window, menu::main_context);
    
    images::load_texture_from_memory(configEditorIconRawData, sizeof configEditorIconRawData, &menu::icons::config_editor);
     
    menu::run_loop();
    
    if (menu::overlay::window != nullptr)
    {
        menu::overlay::destroy_overlay();
        ImGui::SetCurrentContext(menu::overlay::context);
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext(menu::overlay::context);
    }
    
    ImGui::SetCurrentContext(menu::main_context);
    
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(menu::main_context);
    
    glfwDestroyWindow(menu::main_window);
    
    glfwTerminate();
    
    return 0;
}
#else

bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
{
    auto result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);

    cv::matchTemplate(Frame, ElementToFind, result, cv::TM_CCOEFF_NORMED);
    cv::threshold(result, result, Threshold, 1.0, cv::THRESH_TOZERO);
    double AccuracyValue;
    cv::minMaxLoc(result, NULL, &AccuracyValue, NULL, &Detectedlocation);
    return AccuracyValue >= Threshold;
}


int WINAPI wWinMain(HINSTANCE, HINSTANCE, PTSTR, int) // alternate winmain used for testing
{
    using hook_tracker::_internal::vec2;

    hook_tracker::in_game_ui_scale.load_value();

    int ui_scale = 100;

    for (int i = 0; i < 4; i++)
    {
        hook_tracker::survivor surv;

        surv.index = i;

        if (ml::get_screen_height() == 1440)
        {
            const auto region = hook_tracker::_internal::survivor_regions_1440[i];

            surv.location = region;
            surv.size = vec2(300, 100);
        }

        hook_tracker::all_survivors.push_back(surv);
    }

    backend::init_glfw();

    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_DECORATED, false);
    const auto window = backend::setup_window("debug", 800, 600);

    ImGuiContext* ctx = nullptr;
    backend::setup_imgui(window, ctx);

    cv::Mat hook_image = cv::imread("C:\\Users\\rarks\\Desktop\\hook.png", cv::IMREAD_GRAYSCALE);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (int i = 0; i < 4; i++)
        {
            auto surv = hook_tracker::all_survivors[i];

            ImGui::GetBackgroundDrawList()->AddRect(surv.location.to_imvec2(), (surv.location + surv.size).to_imvec2(), ImColor(255, 0, 0));

            cv::Rect area_to_scan(surv.location.x, surv.location.y, surv.size.x, surv.size.y);

            const auto frame = misc::get_screenshot(area_to_scan);

            cv::Point detected_location;
            if (TemplateMatch(frame, hook_image, 0.9, detected_location))
            {
                ImGui::GetBackgroundDrawList()->AddRect(
                    {(surv.location + vec2((float)detected_location.x, (float)detected_location.y)).to_imvec2()},
                    {(surv.location + vec2((float)detected_location.x + hook_image.cols, (float)detected_location.y + hook_image.rows)).to_imvec2()},
                    ImColor(255, 0, 0));

            }
        }

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    return 0;
}

#endif
