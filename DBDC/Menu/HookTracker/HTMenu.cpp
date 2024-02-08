#include "HTMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "HookCounter.h"
#include "../ConfigEditor/ConfigEditor.hpp"

void HTMenu::Setup()
{
    ConfigEditor::InitializeConfig();
    HTVars.menuScaleFactor.LoadValue();
    HTVars.hudScaleFactor.LoadValue();

    mINI::INIFile file(Backend::exeDirectory.string() + "\\DBDC\\Settings");
    mINI::INIStructure ini;
    file.read(ini);

    strcpy_s(HTVars.soundFilePath, ini["HookCounter"]["SoundPath"].c_str());
}

void HTMenu::RenderUI()
{
    ImGui::SetCursorPosY(45);
    
    if (ImGui::Checkbox("Enable", &HTVars.enabled))
    {
        if (HTVars.enabled)
        {
            if (!Menu::Overlay::IsOverlayCreated())
            {
                Menu::Overlay::CreateOverlay();
                ImGui::SetCurrentContext(Menu::mainContext);
                glfwMakeContextCurrent(Menu::mainWindow);
            }

            std::thread loopThread(HookTracker::DetectionLoop);
            loopThread.detach();
        }
        else
        {
            if (!CVars.enabled)
                Menu::Overlay::DestroyOverlay();

            HookTracker::Internal::survivorLocationsStage1.clear();
            HookTracker::Internal::survivorLocationsStage2.clear();
        }
    }

    ImGui::BeginDisabled(!HTVars.enabled);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 240);
    ImGui::SetColumnWidth(1, 245);

    ImGui::SeparatorText("Options");

    ImGui::Checkbox("Track 1st Stage Hooks", &HTVars.track1stStage);
    ImGui::Checkbox("Track 2nd Stage Hooks", &HTVars.track2ndStage);

    ImGui::Checkbox("Play Sound On Hook", &HTVars.playSoundOnHook);
    GUI::ToolTip("LOUD LOUD LOUD LOUD LOUD", false);

    ImGui::NextColumn();

    ImGui::SeparatorText("Customization");
    
    ImGui::BeginDisabled(!HTVars.playSoundOnHook);
    ImGui::SetNextItemWidth(140);
    if (ImGui::InputTextWithHint("Sound Path", "C:/Path/To/File.wav", HTVars.soundFilePath, sizeof HTVars.soundFilePath))
    {
        mINI::INIFile file(Backend::exeDirectory.string() + "\\DBDC\\Settings");
        mINI::INIStructure ini;
        file.read(ini);

        ini["HookCounter"]["SoundPath"] = HTVars.soundFilePath;
        file.write(ini);
    }
    GUI::ToolTip("Only .wav files are supported in this version.");
    ImGui::EndDisabled();

    ImGui::NextColumn();

    ImGui::SeparatorText("Settings");

    GUI::DropDownBox("Menu UI Scale", HTVars.menuScaleFactor, UIScales, false);
    GUI::ToolTip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");

    GUI::DropDownBox("In-Game UI Scale", HTVars.hudScaleFactor, UIScales, false);
    GUI::ToolTip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::SliderFloat("1st Threshold", &HTVars.firstThreshold, 0.0F, 1.F, "%.1F");
    ImGui::SliderFloat("2nd Threshold", &HTVars.secondThreshold, 0.0F, 1.F, "%.1F");

    ImGui::EndDisabled();

    ImGui::EndColumns();
}
