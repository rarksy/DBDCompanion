#include "HCMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../../Config/Config.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "../Misc/Misc.hpp"
#include "Images/HookCounter/Hook.hpp"
#include "Images/HookCounter/Stage2.hpp"
#include <Windows.h>

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

            std::thread loopThread(HCMenu::DetectionLoop);
            loopThread.detach();
        }
        else if (!CVars.enabled)
        {
            Menu::Overlay::DestroyOverlay();
            Internal::survivorLocations.clear();
            Internal::survivorLocationsStage2.clear();
        }
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 240);


    ImGui::SeparatorText("Options");

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

bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
{
    auto Result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);

    cv::matchTemplate(Frame, ElementToFind, Result, cv::TM_CCOEFF_NORMED);
    cv::threshold(Result, Result, Threshold, 1.0, cv::THRESH_TOZERO);
    double AccuracyValue;
    cv::minMaxLoc(Result, NULL, &AccuracyValue, NULL, &Detectedlocation);
    return AccuracyValue >= Threshold;
}

void HCMenu::DetectionLoop()
{
    cv::Rect regionToCapture(0, 0, Backend::screenWidth / 3, Backend::screenHeight);

    constexpr int desiredFrameRate = 3;
    constexpr std::chrono::duration<double> targetFrameDuration(1.0 / desiredFrameRate);

    const std::vector hookByteArray(hookRawData, hookRawData + sizeof(hookRawData));
    const std::vector stage2ByteArray(stage2RawData, stage2RawData + sizeof(stage2RawData));
    cv::Mat hookImage = cv::imdecode(hookByteArray, cv::IMREAD_GRAYSCALE);
    cv::Mat stage2Image = cv::imdecode(stage2ByteArray, cv::IMREAD_GRAYSCALE);

    if (Backend::screenHeight == 1080)
        cv::resize(hookImage, hookImage, cv::Size(), 1.0 / 1.3, 1.0 / 1.3, cv::INTER_AREA);

    cv::resize(hookImage, hookImage, cv::Size(static_cast<int>(hookImage.cols * HCVars.HudScaleFactor.second / 100.0),
                                              static_cast<int>(hookImage.rows * HCVars.HudScaleFactor.second / 100.0)));

    while (HCVars.enabled)
    {
        const auto startTime = std::chrono::steady_clock::now();

        const cv::Mat frame = Misc::GetScreenshot(regionToCapture);

        cv::Point foundLocation;
        if (TemplateMatch(frame, hookImage, 0.9, foundLocation))
        {
            if (Internal::survivorLocations.empty())
            {
                Internal::survivorLocations.push_back({ImVec2(foundLocation.x, foundLocation.y), true});
                
                if (HCVars.playSoundOnHook)
                    PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
                
                continue;
            }

            bool addSurvivor = true;
            const int survivorLocationsSize = Internal::survivorLocations.size();
            for (int i = 0; i < survivorLocationsSize; i++)
            {
                const auto survivor = Internal::survivorLocations[i];

                if (foundLocation.y > survivor.first.y - 15 && foundLocation.y < survivor.first.y + 15)
                {
                    addSurvivor = false;
                    break;
                }
            }

            if (addSurvivor)
            {
                Internal::survivorLocations.push_back({ImVec2(foundLocation.x, foundLocation.y), true});

                if (HCVars.playSoundOnHook)
                    PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
            }
        }

        if (HCVars.track2ndStage && TemplateMatch(frame, stage2Image, 0.9, foundLocation))
        {
            if (Internal::survivorLocationsStage2.empty())
            {
                Internal::survivorLocationsStage2.push_back({ImVec2(foundLocation.x, foundLocation.y), true});

                if (HCVars.playSoundOnHook)
                    PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
                
                continue;
            }

            bool addSurvivor = true;
            const int survivorLocationsSize = Internal::survivorLocationsStage2.size();
            for (int i = 0; i < survivorLocationsSize; i++)
            {
                const auto survivor = Internal::survivorLocationsStage2[i];

                if (foundLocation.y > survivor.first.y - 15 && foundLocation.y < survivor.first.y + 15)
                {
                    addSurvivor = false;
                    break;
                }
            }

            if (addSurvivor)
            {
                Internal::survivorLocationsStage2.push_back({ImVec2(foundLocation.x, foundLocation.y), true});

                if (HCVars.playSoundOnHook)
                    PlaySoundA(HCVars.soundFilePath, NULL, SND_ASYNC);
            }
            
        }

        const auto endTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsedTime = endTime - startTime;
        const auto sleepDuration = targetFrameDuration - elapsedTime;

        if (sleepDuration > std::chrono::duration<double>(0))
        {
            std::this_thread::sleep_until(startTime + targetFrameDuration);
        }
    }
}

void HCMenu::RenderDetection()
{
    for (const auto& location : HCMenu::Internal::survivorLocations | std::views::keys)
    {
        if (location.x != 0.F && location.y != 0.F)
            ImGui::GetBackgroundDrawList()->AddRect(location, ImVec2(location.x + 10, location.y + 10),
                                                    ImColor(255, 0, 0));
    }

    for (const auto& location : HCMenu::Internal::survivorLocationsStage2 | std::views::keys)
    {
        if (location.x != 0.F && location.y != 0.F)
            ImGui::GetBackgroundDrawList()->AddRect(location, ImVec2(location.x + 10, location.y + 10),
                                                    ImColor(255, 0, 0));
    }
}
