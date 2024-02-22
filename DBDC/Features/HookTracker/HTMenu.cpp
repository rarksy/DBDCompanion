#include "HTMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "HookTracker.hpp"
#include "../ConfigEditor/ConfigEditor.hpp"

void HTMenu::Setup()
{
    config_editor::initialize_config();
    HTVars.menuScaleFactor.load_value();
    HTVars.hudScaleFactor.load_value();

    mINI::INIFile file(backend::exe_directory.string() + backend::settings_directory + "Settings");
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
            if (!menu::overlay::is_overlay_created())
            {
                menu::overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }

            std::thread loopThread(HookTracker::DetectionLoop);
            loopThread.detach();
        }
        else
        {
            if (!CVars.enabled)
                menu::overlay::destroy_overlay();

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
    gui::tool_tip("LOUD LOUD LOUD LOUD LOUD", false);

    ImGui::NextColumn();

    ImGui::SeparatorText("Customization");
    
    ImGui::BeginDisabled(!HTVars.playSoundOnHook);
    ImGui::SetNextItemWidth(140);
    if (ImGui::InputTextWithHint("Sound Path", "C:/Path/To/File.wav", HTVars.soundFilePath, sizeof HTVars.soundFilePath))
    {
        mINI::INIFile file(backend::exe_directory.string() + backend::settings_directory + "Settings");
        mINI::INIStructure ini;
        file.read(ini);

        ini["HookCounter"]["SoundPath"] = HTVars.soundFilePath;
        file.write(ini);
    }
    gui::tool_tip("Only .wav files are supported in this version.");
    ImGui::EndDisabled();

    ImGui::NextColumn();

    ImGui::SeparatorText("Settings");

    gui::drop_down_box("Menu UI Scale", HTVars.menuScaleFactor, UIScales, false);
    gui::tool_tip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");

    gui::drop_down_box("In-Game UI Scale", HTVars.hudScaleFactor, UIScales, false);
    gui::tool_tip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::SliderFloat("1st Threshold", &HTVars.firstThreshold, 0.0F, 1.F, "%.1F");
    ImGui::SliderFloat("2nd Threshold", &HTVars.secondThreshold, 0.0F, 1.F, "%.1F");

    ImGui::EndDisabled();

    ImGui::EndColumns();
}
