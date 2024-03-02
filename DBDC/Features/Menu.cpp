#include "Menu.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <chrono>
#include <thread>
#include "ConfigEditor/CEMenu.hpp"
#include "Crosshair/CMenu.h"
#include "Crosshair/Crosshair.h"
#include "GUI/GUI.h"
#include "HookTracker\HTMenu.h"
#include <Windows.h>

#include "../Misc/Misc.hpp"
#include "HookTracker\HookTracker.hpp"
#include "Images/Images.h"
#include "Perk Packager/PerkPackager.h"
#include "Shrine Of Secrets/ShrineOfSecrets.hpp"

void menu::run_loop()
{
    {
        const mINI::INIFile file(backend::exe_directory.string() + backend::settings_directory + "Settings");
        mINI::INIStructure ini;
        file.read(ini);

        if (!ini["Settings"]["MenuAccentR"].empty())
        {
            styling::menu_accent.r = std::atoi(ini["Settings"]["MenuAccentR"].c_str());
            styling::menu_accent.g = std::atoi(ini["Settings"]["MenuAccentG"].c_str());
            styling::menu_accent.b = std::atoi(ini["Settings"]["MenuAccentB"].c_str());
            styling::menu_accent.a = std::atoi(ini["Settings"]["MenuAccentA"].c_str());
        }
    }

    std::thread shrine_load_thread([]
    {
        if (shrine_of_secrets::is_cache_valid())
            shrine_of_secrets::load_cache();
        else
        {
            shrine_of_secrets::init();
            shrine_of_secrets::cache();
        }
    });
    shrine_load_thread.detach();

    while (!glfwWindowShouldClose(main_window))
    {
        const double start_time = glfwGetTime();

        glfwMakeContextCurrent(menu::main_window);

        ImGui::SetCurrentContext(menu::main_context);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        menu::create_global_style();
        menu::render_ui();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(menu::main_window);

        if (menu::overlay::window != nullptr)
        {
            glfwMakeContextCurrent(menu::overlay::window);
            ImGui::SetCurrentContext(menu::overlay::context);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (HTVars.enabled)
                HookTracker::RenderDetection();

            if (CVars.enabled)
                Crosshair::DrawCrosshairs();


            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(menu::overlay::window);
        }

        const double end_time = glfwGetTime();
        const double elapsed_time = end_time - start_time;

        constexpr double target_frame_time = 1.0 / static_cast<double>(80);

        if (elapsed_time < target_frame_time)
        {
            const double sleep_time = target_frame_time - elapsed_time;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
}

void menu::render_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(styling::menu_width, styling::menu_height), ImGuiCond_Once);
    ImGui::Begin("menu", nullptr, menu_flags);

    static bool hamburger_open = true;
    static float hamburger_width = 0.F;
    static float hamburger_height = styling::menu_height / 3.F;
    static bool show_color_picker = false;

    if (gui::begin_hamburger_menu(hamburger_open, hamburger_width, hamburger_height, styling::menu_accent.as_imcolor()))
    {
        ImGui::SetCursorPosY(45);

        if (ImGui::Button("Config Editor"))
            menu_to_show = 1;
        gui::tool_tip("Allows you to adjust your game settings in\nmore detail than the base game offers");

        ImGui::Spacing();

        if (ImGui::Button("Hook Tracker"))
            menu_to_show = 2;
        gui::tool_tip("This is a pre-release alpha of the hook Tracker\nIt is not finished and WILL contain bugs");

        ImGui::Spacing();

        if (ImGui::Button("Crosshair Menu"))
            menu_to_show = 3;

        if (menu_to_show != 0)
        {
            ImGui::SetCursorPos({(show_color_picker ? 60.F : 45.F), 9});
            if (ImGui::Button("<-"))
                menu_to_show = 0;
        }

        gui::end_hamburger_menu(hamburger_open, menu_to_show, hamburger_width, hamburger_height);
    }
    gui::draw_hamburger_menu(hamburger_open, styling::menu_accent.as_imcolor());

    if (ImGui::IsKeyPressed(ImGuiKey_Space, false))
        show_color_picker = !show_color_picker;

    if (show_color_picker)
    {
        ImGui::SetCursorPos({45, 9});
        if (gui::color_picker("Menu Accent", &styling::menu_accent))
        {
            const mINI::INIFile file(backend::exe_directory.string() + backend::settings_directory + "Settings");
            mINI::INIStructure ini;
            file.read(ini);

            ini["Settings"]["MenuAccentR"] = std::to_string(styling::menu_accent.r);
            ini["Settings"]["MenuAccentG"] = std::to_string(styling::menu_accent.g);
            ini["Settings"]["MenuAccentB"] = std::to_string(styling::menu_accent.b);
            ini["Settings"]["MenuAccentA"] = std::to_string(styling::menu_accent.a);
            file.write(ini);
        }
    }

    ImGui::BeginDisabled(hamburger_open || hamburger_width > 0.F);

    if (menu_to_show == 0)
    {
        shrine_of_secrets::render_ui();

        ImGui::SetCursorPos({10, 470});
        ImGui::TextColored(ImVec4(0.1F, 0.1F, 0.1F, 0.3F), "I DONT KNOW HOW TO MAKE A GOOD MAIN MENU");
    }

    if (menu_to_show == 1)
    {
        static std::once_flag flag;
        std::call_once(flag, CEMenu::Setup);

        CEMenu::RenderUI();
    }

    else if (menu_to_show == 2)
    {
        static std::once_flag flag;
        std::call_once(flag, HTMenu::Setup);

        HTMenu::RenderUI();
    }

    else if (menu_to_show == 3)
    {
        static std::once_flag flagCrosshair;
        static std::once_flag flagMenu;
        std::call_once(flagCrosshair, Crosshair::Setup);
        std::call_once(flagMenu, CMenu::Setup);

        CMenu::RenderUI();
    }


    ImGui::EndDisabled();


    ImGui::SetCursorPos({720, 470});
    ImGui::TextColored(ImVec4(0.8F, 0.8F, 0.8F, 0.5F), "(?)");
    gui::tool_tip(
        "Hold right click when hovering an option to view information about it.\n"
        "Tip: Some options have images associated to assist in selection."
        "\n\nDead By Daylight Companion By Rarksy\n"
        "Press Enter To Join The Discord Server.\n\n"
        "Build Version: Early Access "
#ifdef _DEBUG
        "Debug"
#else
        "Release"
#endif
        "\nBuild Date: " + std::string(__DATE__)
        + "\nBuild Time: " + std::string(__TIME__), 500, false
    );

    if (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
        ShellExecuteA(NULL, "open", "https://discord.gg/vKjjS8yazu", NULL, NULL, SW_SHOWNORMAL);

    ImGui::End();
}

inline ImVec4 RGBToImVec4(int r, int g, int b, int a = 255)
{
    return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
}


void menu::create_global_style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    // Button
    colors[ImGuiCol_Button] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_ButtonHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_ButtonActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Main Window
    colors[ImGuiCol_FrameBg] = RGBToImVec4(20, 20, 20);
    colors[ImGuiCol_FrameBgHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_FrameBgActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    style.FrameRounding = 2.F;
    style.DisabledAlpha = 0.3F;
    style.FrameBorderSize = 1.7F;
    style.DisabledAlpha = 0.1f;

    // Slider
    colors[ImGuiCol_Slider] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_SliderActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    colors[ImGuiCol_SliderHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_SliderGrab] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g, styling::menu_accent.b);
    style.GrabRounding = style.FrameRounding;

    // Checkbox
    colors[ImGuiCol_CheckMark] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_CheckMarkHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_CheckMarkActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Combo
    colors[ImGuiCol_Combo] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_ComboActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);
    colors[ImGuiCol_ComboHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);

    // Header ( Selectables Etc )
    colors[ImGuiCol_Header] = styling::menu_accent.to_imvec4();
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 70, styling::menu_accent.b + 70);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(styling::menu_accent.r, styling::menu_accent.g + 120, styling::menu_accent.b + 120);

    // Separator
    colors[ImGuiCol_Separator] = styling::menu_accent.to_imvec4();

    // Input Text
    colors[ImGuiCol_InputText] = styling::menu_accent.to_imvec4();

    colors[ImGuiCol_TextSelectedBg] = RGBToImVec4(styling::menu_accent.r - 30, styling::menu_accent.g - 20, styling::menu_accent.b - 20, styling::menu_accent.a - 105);
}
