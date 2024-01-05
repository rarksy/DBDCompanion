#include "Menu.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <chrono>
#include <thread>

#include "ConfigEditor/CEMenu.hpp"
#include "HookCounter/HCMenu.h"
#include <Windows.h>

void Menu::RunLoop()
{
    
    while (!glfwWindowShouldClose(window))
    {
        const double startTime = glfwGetTime();

        glfwMakeContextCurrent(window);

        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        RenderUI();
        
        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        const double entTime = glfwGetTime();
        const double elapsedTime = entTime - startTime;

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

    // Main Menu Is Disabled At The Moment Due To Hook Counter Not Being Implemented
    
    // if (menuToShow != 0)
    //     if (ImGui::Button("<--"))
    //         menuToShow = 0;

    if (menuToShow == 0)
    {
        if (ImGui::Button("Config Editor"))
            menuToShow = 1;

        if (ImGui::Button("Hook Counter"))
            menuToShow = 2;
    }

    else if (menuToShow == 1)
    {
        // Setup Config Editor Menu
        static std::once_flag flag;
        std::call_once(flag, CEMenu::Setup);

        CEMenu::RenderUI();
    }

    else if (menuToShow == 2)
    {
        if (!HCMenu::hasCreatedOverlay)
        {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
            glfwWindowHint(GLFW_DECORATED, false);
            glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);
            glfwWindowHint(GLFW_FLOATING, true);
            HCMenu::overlayWindow = glfwCreateWindow(screenWidth / 3, screenHeight, "Hook Counter", nullptr, nullptr);
            glfwSetWindowPos(HCMenu::overlayWindow, 0, 0);
            glfwFocusWindow(window);

            HCMenu::overlayContext = ImGui::CreateContext();
            
            HCMenu::hasCreatedOverlay = true;
        }
        
        HCMenu::RenderUI();
    }

    ImGui::End();
}
