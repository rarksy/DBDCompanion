
#include "HCMenu.h"

#include "ImGui/imgui.h"

#include "../Menu.h"

// not implemented yet

void HCMenu::RenderUI()
{
    ImGui::SameLine();
    if (ImGui::Button("Kill Hook Counter"))
    {
        Menu::Overlay::DestroyOverlay();
        Menu::menuToShow = 0;
    }
    
    ImGui::Text("hook counter menu");
}

void HCMenu::CreateStyle()
{
}
