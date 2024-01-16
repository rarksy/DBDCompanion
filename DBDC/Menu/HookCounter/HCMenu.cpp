#include "HCMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../../Config/Config.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "../Misc/Misc.hpp"
#include "HookCounter.h"

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

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 240);


    ImGui::SeparatorText("Options");

    ImGui::Checkbox("Track 1st Stage Hooks", &HCVars.track1stStage);
    ImGui::Checkbox("Track 2nd Stage Hooks", &HCVars.track2ndStage);

    ImGui::Checkbox("Play Sound On Hook", &HCVars.playSoundOnHook);
    GUI::ToolTip("LOUD LOUD LOUD LOUD LOUD", false);

    ImGui::NextColumn();

    ImGui::SeparatorText("Customization");

    ImGui::BeginDisabled(!HCVars.playSoundOnHook);
    ImGui::SetNextItemWidth(140);
    ImGui::InputText("Sound Path", HCVars.soundFilePath, sizeof HCVars.soundFilePath);
    GUI::ToolTip("Only .wav files are supported in this version.");
    ImGui::EndDisabled();

    ImGui::NextColumn();


    ImGui::SeparatorText("Settings");

    GUI::DropDownBox("Menu UI Scale", HCMenu::UIScales, HCVars.MenuScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"UI Scale\" Setting Is Set To In Game.");

    GUI::DropDownBox("In-Game UI Scale", HCMenu::UIScales, HCVars.HudScaleFactor, true);
    GUI::ToolTip("Set This To Whatever Your \"In-Game UI Scale\" Setting Is Set To In Game.");

    ImGui::EndColumns();
}


    
    //     if (TemplateMatch(frame, hookImage, 0.9, foundLocation))
    //     {
    //         if (Internal::survivorLocations.empty())
    //         {
    //             Internal::survivorLocations.push_back({ImVec2(foundLocation.x, foundLocation.y), true});
    //             
    //             if (HCVars.playSoundOnHook)
    //                 PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
    //             
    //             continue;
    //         }
    //
    //         bool addSurvivor = true;
    //         const int survivorLocationsSize = Internal::survivorLocations.size();
    //         for (int i = 0; i < survivorLocationsSize; i++)
    //         {
    //             const auto survivor = Internal::survivorLocations[i];
    //
    //             if (foundLocation.y > survivor.first.y - 15 && foundLocation.y < survivor.first.y + 15)
    //             {
    //                 addSurvivor = false;
    //                 break;
    //             }
    //         }
    //
    //         if (addSurvivor)
    //         {
    //             Internal::survivorLocations.push_back({ImVec2(foundLocation.x, foundLocation.y), true});
    //
    //             if (HCVars.playSoundOnHook)
    //                 PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
    //         }
    //     }
    //
    //     if (HCVars.track2ndStage && TemplateMatch(frame, stage2Image, 0.9, foundLocation))
    //     {
    //         if (Internal::survivorLocationsStage2.empty())
    //         {
    //             Internal::survivorLocationsStage2.push_back({ImVec2(foundLocation.x, foundLocation.y), true});
    //
    //             if (HCVars.playSoundOnHook)
    //                 PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
    //             
    //             continue;
    //         }
    //
    //         bool addSurvivor = true;
    //         const int survivorLocationsSize = Internal::survivorLocationsStage2.size();
    //         for (int i = 0; i < survivorLocationsSize; i++)
    //         {
    //             const auto survivor = Internal::survivorLocationsStage2[i];
    //
    //             if (foundLocation.y > survivor.first.y - 15 && foundLocation.y < survivor.first.y + 15)
    //             {
    //                 addSurvivor = false;
    //                 break;
    //             }
    //         }
    //
    //         if (addSurvivor)
    //         {
    //             Internal::survivorLocationsStage2.push_back({ImVec2(foundLocation.x, foundLocation.y), true});
    //
    //             if (HCVars.playSoundOnHook)
    //                 PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
    //         }
    //         
    //     }
    //
    //     const auto endTime = std::chrono::steady_clock::now();
    //     const std::chrono::duration<double> elapsedTime = endTime - startTime;
    //     const auto sleepDuration = targetFrameDuration - elapsedTime;
    //
    //     if (sleepDuration > std::chrono::duration<double>(0))
    //     {
    //         std::this_thread::sleep_until(startTime + targetFrameDuration);
    //     }
    // }
