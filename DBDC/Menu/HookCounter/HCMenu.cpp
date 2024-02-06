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
    HCVars.menuScaleFactor.LoadValue();
    HCVars.hudScaleFactor.LoadValue();

    if (!std::filesystem::exists(Backend::exeDirectory.string() + "\\DBDC\\HookCounterSettings"))
    {
        std::ofstream fileToCreate(Backend::exeDirectory.string() + "\\DBDC\\HookCounterSettings");
        fileToCreate.close();
    }

    mINI::INIFile file(Backend::exeDirectory.string() + "\\DBDC\\HookCounterSettings");
    mINI::INIStructure ini;
    file.read(ini);

    strcpy_s(HCVars.soundFilePath, ini["Settings"]["FilePath"].c_str());
}

void HCMenu::RenderUI()
{
    ImGui::SetCursorPosY(45);
    
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
        else
        {
            if (!CVars.enabled)
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
    if (ImGui::InputTextWithHint("Sound Path", "C:/Path/To/File.wav", HCVars.soundFilePath, sizeof HCVars.soundFilePath))
    {
        mINI::INIFile file(Backend::exeDirectory.string() + "\\DBDC\\HookCounterSettings");
        mINI::INIStructure ini;
        file.read(ini);

        ini["Settings"]["FilePath"] = HCVars.soundFilePath;
        file.write(ini);
    }
    GUI::ToolTip("Only .wav files are supported in this version.");
    ImGui::EndDisabled();

    ImGui::NextColumn();

    ImGui::SeparatorText("Settings");

    GUI::DropDownBox("Menu UI Scale", HCVars.menuScaleFactor, UIScales, false);
    GUI::ToolTip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");

    GUI::DropDownBox("In-Game UI Scale", HCVars.hudScaleFactor, UIScales, false);
    GUI::ToolTip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::SliderFloat("1st Threshold", &HCVars.firstThreshold, 0.0F, 1.F, "%.1F");
    ImGui::SliderFloat("2nd Threshold", &HCVars.secondThreshold, 0.0F, 1.F, "%.1F");

    ImGui::EndDisabled();

    ImGui::EndColumns();
}
