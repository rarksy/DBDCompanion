#include "HCMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "HookCounter.h"
#include "../ConfigEditor/ConfigEditor.hpp"

void HCMenu::Setup()
{
    ConfigEditor::InitializeConfig();
    ConfigEditor::LoadSettingInt(ConfigEditor::Files::gameUserSettings, ConfigEditor::Sections::DBDGameUserSettings, HCVars.MenuScaleFactor);
    ConfigEditor::LoadSettingInt(ConfigEditor::Files::gameUserSettings, ConfigEditor::Sections::DBDGameUserSettings, HCVars.HudScaleFactor);
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

            std::thread loopThread(HookCounter::DetectionLoop);
            loopThread.detach();
        }
        else if (!CVars.enabled)
        {
            Menu::Overlay::DestroyOverlay();
            HookCounter::Internal::survivorLocationsStage1.clear();
            HookCounter::Internal::survivorLocationsStage2.clear();
        }
    }

    ImGui::BeginDisabled(!HCVars.enabled);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 240);
    ImGui::SetColumnWidth(1, 245);
    
    ImGui::SeparatorText("Options");

    ImGui::Checkbox("Track 1st Stage Hooks", &HCVars.track1stStage);
    ImGui::Checkbox("Track 2nd Stage Hooks", &HCVars.track2ndStage);

    ImGui::Checkbox("Play Sound On Hook", &HCVars.playSoundOnHook);
    GUI::ToolTip("LOUD LOUD LOUD LOUD LOUD", false);

    ImGui::NextColumn();

    ImGui::SeparatorText("Customization");

    ImGui::BeginDisabled(!HCVars.playSoundOnHook);
    ImGui::SetNextItemWidth(140);
    ImGui::InputTextWithHint("Sound Path", "C:/Path/To/File.wav", HCVars.soundFilePath, sizeof HCVars.soundFilePath);
    GUI::ToolTip("Only .wav files are supported in this version.");
    ImGui::EndDisabled();

    ImGui::NextColumn();
    
    ImGui::SeparatorText("Settings");

    GUI::DropDownBox("Menu UI Scale", HCMenu::UIScales, HCVars.MenuScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");

    GUI::DropDownBox("In-Game UI Scale", HCMenu::UIScales, HCVars.HudScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::EndDisabled();

    ImGui::EndColumns();
}