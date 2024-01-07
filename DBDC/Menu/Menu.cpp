#include "Menu.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <chrono>
#include <thread>

#include "ConfigEditor/CEMenu.hpp"
#include "HookCounter/HCMenu.h"

void Menu::RunLoop()
{
    
    while (!glfwWindowShouldClose(mainWindow))
    {
        const double startTime = glfwGetTime();

        glfwMakeContextCurrent(mainWindow);

        glfwPollEvents();

        ImGui::SetCurrentContext(Menu::mainContext);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        CreateGlobalStyle();
        RenderUI();

        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }

        glfwSwapBuffers(mainWindow);
        

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
        if (ImGui::Button("<--"))
            menuToShow = 0;

    if (menuToShow == 0)
    {
        if (ImGui::Button("Config Editor"))
            menuToShow = 1;

        if (ImGui::Button("Hook Counter"))
            menuToShow = 2;
    }

    else if (menuToShow == 1)
    {
        static std::once_flag flag;
        std::call_once(flag, CEMenu::Setup);

        CEMenu::RenderUI();
    }

    else if (menuToShow == 2)
    {
        if (!Overlay::IsOverlayCreated())
        {
            Overlay::CreateOverlay();
        }
        
        HCMenu::RenderUI();
    }

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
    colors[ImGuiCol_FrameBg] = RGBToImVec4(255, 83, 83);
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
}
