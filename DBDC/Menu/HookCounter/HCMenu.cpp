#include "HCMenu.h"
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../../Config/Config.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"

void HCMenu::Setup()
{
    Config::LoadSetting(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings, HCVars.MenuScaleFactor);
    Config::LoadSetting(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings, HCVars.HudScaleFactor);
}

void HCMenu::RenderUI()
{
    if (ImGui::Checkbox("Enable", &HCVars.enabled))
    {
        if (HCVars.enabled)
        {
            if (!Menu::Overlay::IsOverlayCreated())
            {
                Menu::Overlay::CreateOverlay();
                ImGui::SetCurrentContext(Menu::mainContext);
                glfwMakeContextCurrent(Menu::mainWindow);
            }
        }
        else if (!CVars.enabled)
            Menu::Overlay::DestroyOverlay();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 240);

    GUI::DropDownBox("Menu UI Scale", HCMenu::UIScales, HCVars.MenuScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");
    
    GUI::DropDownBox("In-Game UI Scale", HCMenu::UIScales, HCVars.MenuScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::EndColumns();
}

void HCMenu::CreateStyle()
{
}
