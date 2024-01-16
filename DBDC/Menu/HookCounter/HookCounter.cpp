#include <chrono>
#include <opencv2/opencv.hpp>
#include <Windows.h>

#include "HookCounter.h"
#include "HCMenu.h"
#include "Images/HookCounter/Hook.hpp"
#include "Images/HookCounter/Stage2.hpp"
#include "../Backend/Backend.hpp"
#include "../Misc/Misc.hpp"

bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
{
    auto Result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);

    cv::matchTemplate(Frame, ElementToFind, Result, cv::TM_CCOEFF_NORMED);
    cv::threshold(Result, Result, Threshold, 1.0, cv::THRESH_TOZERO);
    double AccuracyValue;
    cv::minMaxLoc(Result, NULL, &AccuracyValue, NULL, &Detectedlocation);
    return AccuracyValue >= Threshold;
}

void HookCounter::DetectionLoop()
{
    constexpr int targetFramerate = 3;
    constexpr std::chrono::duration<double> targetFrameDuration(1.0 / targetFramerate);

    cv::Mat stage1Image;
    cv::Mat stage2Image;

    {
        const std::vector stage1ByteArray(hookRawData, hookRawData + sizeof hookRawData);
        const std::vector stage2ByteArray(stage2RawData, stage2RawData + sizeof stage2RawData);

        stage1Image = cv::imdecode(stage1ByteArray, cv::IMREAD_GRAYSCALE);
        stage2Image = cv::imdecode(stage2ByteArray, cv::IMREAD_GRAYSCALE);
    }

    if (Backend::screenHeight == 1080)
    {
        cv::resize(stage1Image, stage1Image, cv::Size(), 1.0 / 1.3, 1.0 / 1.3, cv::INTER_AREA);
        cv::resize(stage2Image, stage2Image, cv::Size(), 1.0 / 1.3, 1.0 / 1.3, cv::INTER_AREA);
    }

    cv::resize(stage1Image, stage1Image, cv::Size(
                   static_cast<int>(stage1Image.cols * HCMenu::HudScaleFactor.second / 100.0),
                   static_cast<int>(stage1Image.rows * HCMenu::HudScaleFactor.second / 100.0))
    );

    cv::resize(stage2Image, stage2Image, cv::Size(
                   static_cast<int>(stage2Image.cols * HCMenu::HudScaleFactor.second / 100.0),
                   static_cast<int>(stage2Image.rows * HCMenu::HudScaleFactor.second / 100.0))
    );
    
    cv::Rect region(0, 0, Backend::screenWidth / 3, Backend::screenHeight);
    cv::Mat frame;
    cv::Point detectedLocation;
    double minThreshold = 0.9; // 0-1
    
    while (HCVars.enabled)
    {
        const auto frameStartTime = std::chrono::steady_clock::now();

        frame = Misc::GetScreenshot(region);

        if (HCVars.track1stStage)
        {
            if (TemplateMatch(frame, stage1Image, minThreshold, detectedLocation))
                HandleDetection(detectedLocation, Internal::survivorLocationsStage1);
        }

        if (HCVars.track2ndStage)
        {
            if (TemplateMatch(frame, stage2Image, minThreshold, detectedLocation))
                HandleDetection(detectedLocation, Internal::survivorLocationsStage2);
        }

        const auto frameEndTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsedTime = frameEndTime - frameStartTime;
        const auto timeToSleep = targetFrameDuration - elapsedTime;

        if (timeToSleep > std::chrono::duration<double>(0))
            std::this_thread::sleep_until(frameStartTime + targetFrameDuration);
        
    }
}

void HookCounter::HandleDetection(const cv::Point& detectedLocation, std::vector<ImVec2>& locations)
{
    bool addSurvivor = true;
                
    const int stage1LocationsSize = locations.size();
    for (int i = 0; i < stage1LocationsSize; i++)
    {
        const auto survivor = locations[i];

        if (detectedLocation.y > survivor.y - 15 &&
            detectedLocation.y < survivor.y + 15)
        {
            addSurvivor = false;
            break;
        }
    }

    if (addSurvivor || locations.empty())
    {
        locations.push_back(ImVec2(detectedLocation.x, detectedLocation.y));

        if (HCVars.playSoundOnHook)
            PlaySoundA(HCVars.soundFilePath, nullptr, SND_ASYNC);
    }
}

void HookCounter::RenderDetection()
{
    for (const auto& location : Internal::survivorLocationsStage1)
    {
        if (location.x != 0.F && location.y != 0.F)
            ImGui::GetBackgroundDrawList()->AddRect(location, ImVec2(location.x + 10, location.y + 10),
                                                    ImColor(255, 0, 0));
    }

    for (const auto& location : Internal::survivorLocationsStage2)
    {
        if (location.x != 0.F && location.y != 0.F)
            ImGui::GetBackgroundDrawList()->AddRect(location, ImVec2(location.x + 10, location.y + 10),
                                                    ImColor(255, 0, 0));
    }
}
