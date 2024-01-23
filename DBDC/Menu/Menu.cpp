#include "Menu.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <chrono>
#include <thread>
#include "ConfigEditor/CEMenu.hpp"
#include "Crosshair/CMenu.h"
#include "Crosshair/Crosshair.h"
#include "GUI/GUI.h"
#include "HookCounter/HCMenu.h"
#include <Windows.h>

#include "HookCounter/HookCounter.h"

void Menu::RunLoop()
{
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

            if (HCVars.enabled)
                HookCounter::RenderDetection();

            if (CVars.enabled)
                Crosshair::DrawCrosshair();

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

    if (menuToShow != 0)
    {
        if (ImGui::Button("<- Back"))
            menuToShow = 0;
    }

    if (menuToShow == 0)
    {
        if (ImGui::Button("Config Editor"))
            menuToShow = 1;

        if (ImGui::Button("Hook Counter (ALPHA)"))
            menuToShow = 2;
        GUI::ToolTip("This is a pre-release alpha of the hook counter\nIt is not finished and WILL contain bugs");

        if (ImGui::Button("Crosshair Menu"))
            menuToShow = 3;

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
        std::call_once(flag, HCMenu::Setup);
        
        HCMenu::RenderUI();
    }

    else if (menuToShow == 3)
    {
        static std::once_flag flagCrosshair;
        static std::once_flag flagMenu;
        std::call_once(flagCrosshair, Crosshair::Setup);
        std::call_once(flagMenu, CMenu::Setup);
        
        CMenu::RenderUI();
    }

    ImGui::SetCursorPos({720, 470});
    ImGui::TextColored(ImVec4(0.8F, 0.8F, 0.8F, 0.5F), "(?)");
    GUI::ToolTip("Hold right click when hovering an option to view information about it.\n"
                 "Tip: Some options have images associated to assist in selection."
                 "\n\nDead By Daylight Companion By rarksy/ski\n"
                 "Press Enter To Join The Discord Server.\n\n"
                 "Build Version: Early Access "
#ifdef _DEBUG
                 "Debug"
#else
                 "Release"
#endif
                 "\nBuild Date: " + std::string(__DATE__) +
                 "\nBuild Time: " + std::string(__TIME__), false);

    if (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
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
    colors[ImGuiCol_Button] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_ButtonHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_ButtonActive] = RGBToImVec4(255, 203, 203);

    // Main Window
    colors[ImGuiCol_FrameBg] = RGBToImVec4(20, 20, 20);
    //colors[ImGuiCol_Border] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_FrameBgHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_FrameBgActive] = RGBToImVec4(255, 203, 203);
    style.FrameRounding = 2.F;
    style.DisabledAlpha = 0.3F;
    style.FrameBorderSize = 1.7F;

    // Slider
    colors[ImGuiCol_Slider] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_SliderActive] = RGBToImVec4(255, 203, 203);
    colors[ImGuiCol_SliderHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_SliderGrab] = RGBToImVec4(255, 83, 83);
    style.GrabRounding = style.FrameRounding;

    // Checkbox
    colors[ImGuiCol_CheckMark] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_CheckMarkHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_CheckMarkActive] = RGBToImVec4(255, 203, 203);

    // Combo
    colors[ImGuiCol_Combo] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_ComboActive] = RGBToImVec4(255, 203, 203);
    colors[ImGuiCol_ComboHovered] = RGBToImVec4(255, 153, 153);

    // Header ( Selectables Etc )
    colors[ImGuiCol_Header] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(255, 203, 203);

    // Separator
    colors[ImGuiCol_Separator] = RGBToImVec4(255, 83, 83);

    // Input Text
    colors[ImGuiCol_InputText] = RGBToImVec4(255, 83, 83);
    
    colors[ImGuiCol_TextSelectedBg] = RGBToImVec4(225, 63, 63, 150);

}
