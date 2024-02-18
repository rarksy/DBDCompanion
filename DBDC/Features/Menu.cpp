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

#include "HookTracker\HookTracker.hpp"
#include "Perk Packager/PerkPackager.h"
#include "Perk Packager/PPMenu.h"

void Menu::RunLoop()
{
    {
        const mINI::INIFile file(Backend::exeDirectory.string() + Backend::settingsDirectory + "Settings");
        mINI::INIStructure ini;
        file.read(ini);

        if (!ini["Settings"]["MenuAccentR"].empty())
        {
            Styling::menuAccent.r = std::atoi(ini["Settings"]["MenuAccentR"].c_str());
            Styling::menuAccent.g = std::atoi(ini["Settings"]["MenuAccentG"].c_str());
            Styling::menuAccent.b = std::atoi(ini["Settings"]["MenuAccentB"].c_str());
            Styling::menuAccent.a = std::atoi(ini["Settings"]["MenuAccentA"].c_str());
        }
    }

    while (!glfwWindowShouldClose(mainWindow))
    {
        const double startTime = glfwGetTime();

        glfwMakeContextCurrent(Menu::mainWindow);

        ImGui::SetCurrentContext(Menu::mainContext);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Menu::CreateGlobalStyle();
        Menu::RenderUI();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(Menu::mainWindow);

        if (Menu::Overlay::window != nullptr)
        {
            glfwMakeContextCurrent(Menu::Overlay::window);
            ImGui::SetCurrentContext(Menu::Overlay::context);

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
            glfwSwapBuffers(Menu::Overlay::window);
        }

        const double endTime = glfwGetTime();
        const double elapsedTime = endTime - startTime;

        constexpr double targetFrameTime = 1.0 / static_cast<double>(80);

        if (elapsedTime < targetFrameTime)
        {
            const double sleepTime = targetFrameTime - elapsedTime;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }
    }
}

void Menu::RenderUI()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(Styling::menuWidth, Styling::menuHeight), ImGuiCond_Once);
    ImGui::Begin("menu", nullptr, menuFlags);

    static bool hamburger_open = true;
    static float hamburger_width = 0.F;
    static float hamburger_height = Styling::menuHeight / 3.F;

    if (GUI::BeginHamburgerMenu(hamburger_open, hamburger_width, hamburger_height, Styling::menuAccent.AsImColor()))
    {
        ImGui::SetCursorPosY(45);

        if (ImGui::Button("Config Editor"))
            menuToShow = 1;
        GUI::ToolTip("Allows you to adjust your game settings in\nmore detail than the base game offers");

        ImGui::Spacing();

        if (ImGui::Button("Hook Tracker"))
            menuToShow = 2;
        GUI::ToolTip("This is a pre-release alpha of the hook Tracker\nIt is not finished and WILL contain bugs");

        ImGui::Spacing();

        if (ImGui::Button("Crosshair Menu"))
            menuToShow = 3;
        
        GUI::EndhamburgerMenu(hamburger_open, menuToShow, hamburger_width, hamburger_height, Styling::menuAccent.AsImColor());
    }
    GUI::DrawHamburger(hamburger_open, Styling::menuAccent.AsImColor());

    static bool showColorPicker = false;
    if (ImGui::IsKeyPressed(ImGuiKey_Space, false))
        showColorPicker = !showColorPicker;

    if (showColorPicker)
    {
        ImGui::SetCursorPos({45, 9});
        if (GUI::ColorPicker("Menu Accent", Styling::menuAccent))
        {
            const mINI::INIFile file(Backend::exeDirectory.string() + Backend::settingsDirectory + "Settings");
            mINI::INIStructure ini;
            file.read(ini);

            ini["Settings"]["MenuAccentR"] = std::to_string(Styling::menuAccent.r);
            ini["Settings"]["MenuAccentG"] = std::to_string(Styling::menuAccent.g);
            ini["Settings"]["MenuAccentB"] = std::to_string(Styling::menuAccent.b);
            ini["Settings"]["MenuAccentA"] = std::to_string(Styling::menuAccent.a);
            file.write(ini);
        }
    }

    ImGui::BeginDisabled(hamburger_open || hamburger_width > 0.F);

    if (menuToShow == 0)
    {
        ImGui::SetCursorPos({10, 470});
        ImGui::TextColored(ImVec4(0.1F, 0.1F, 0.1F, 0.3F), "I DONT KNOW HOW TO MAKE A GOOD MAIN MENU");
    }

    else if (menuToShow == 1)
    {
        static std::once_flag flag;
        std::call_once(flag, CEMenu::Setup);

        CEMenu::RenderUI();
    }

    else if (menuToShow == 2)
    {
        static std::once_flag flag;
        std::call_once(flag, HTMenu::Setup);

        HTMenu::RenderUI();
    }

    else if (menuToShow == 3)
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
    GUI::ToolTip(
        "Hold right click when hovering an option to view information about it.\n"
        "Tip: Some options have images associated to assist in selection."
        "\n\nDead By Daylight Companion By rarksy/ski\n"
        "Press Enter To Join The Discord Server.\n\n"
        "Build Version: Early Access "
#ifdef _DEBUG
        "Debug"
#else
                 "Release"
#endif
        "\nBuild Date: " + std::string(__DATE__)
        + "\nBuild Time: " + std::string(__TIME__), false
    );

    if
    (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
        ShellExecuteA(NULL, "open", "https://discord.gg/vKjjS8yazu", NULL, NULL, SW_SHOWNORMAL);

    ImGui::End();
}

inline ImVec4 RGBToImVec4(int r, int g, int b, int a = 255)
{
    return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
}


void Menu::CreateGlobalStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    // Button
    colors[ImGuiCol_Button] = Styling::menuAccent.ToImVec4();
    colors[ImGuiCol_ButtonHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);
    colors[ImGuiCol_ButtonActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);

    // Main Window
    colors[ImGuiCol_FrameBg] = RGBToImVec4(20, 20, 20);
    colors[ImGuiCol_FrameBgHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);
    colors[ImGuiCol_FrameBgActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);
    style.FrameRounding = 2.F;
    style.DisabledAlpha = 0.3F;
    style.FrameBorderSize = 1.7F;
    style.DisabledAlpha = 0.1f;

    // Slider
    colors[ImGuiCol_Slider] = Styling::menuAccent.ToImVec4();
    colors[ImGuiCol_SliderActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);
    colors[ImGuiCol_SliderHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);
    colors[ImGuiCol_SliderGrab] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g, Styling::menuAccent.b);
    style.GrabRounding = style.FrameRounding;

    // Checkbox
    colors[ImGuiCol_CheckMark] = Styling::menuAccent.ToImVec4();
    colors[ImGuiCol_CheckMarkHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);
    colors[ImGuiCol_CheckMarkActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);

    // Combo
    colors[ImGuiCol_Combo] = Styling::menuAccent.ToImVec4();
    colors[ImGuiCol_ComboActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);
    colors[ImGuiCol_ComboHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);

    // Header ( Selectables Etc )
    colors[ImGuiCol_Header] = Styling::menuAccent.ToImVec4();
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 70, Styling::menuAccent.b + 70);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(Styling::menuAccent.r, Styling::menuAccent.g + 120, Styling::menuAccent.b + 120);

    // Separator
    colors[ImGuiCol_Separator] = Styling::menuAccent.ToImVec4();

    // Input Text
    colors[ImGuiCol_InputText] = Styling::menuAccent.ToImVec4();

    colors[ImGuiCol_TextSelectedBg] = RGBToImVec4(Styling::menuAccent.r - 30, Styling::menuAccent.g - 20, Styling::menuAccent.b - 20, Styling::menuAccent.a - 105);
}
