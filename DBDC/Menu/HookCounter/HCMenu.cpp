
#include "HCMenu.h"

#include "ImGui/imgui.h"
#include <opencv2/opencv.hpp>

#include "../Menu.h"


void HCMenu::RenderUI()
{
    ImGui::SameLine();
    if (ImGui::Button("Kill Hook Counter"))
    {
        glfwDestroyWindow(overlayWindow);
        Menu::menuToShow = 0;
        HCMenu::hasCreatedOverlay = false;
        HCMenu::overlayWindow = nullptr;
    }
    
    ImGui::Text("hook counter menu");
}

void HCMenu::CreateStyle()
{
}
