#include <chrono>
#include <Windows.h>

#include "HookTracker.hpp"
#include "HTMenu.h"
#include "Images/HookTracker/Hook.hpp"
#include "Images/HookTracker/Stage2.hpp"
#include "../Backend/Backend.hpp"
#include "../Misc/Misc.hpp"

bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
{
    auto result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);

    cv::matchTemplate(Frame, ElementToFind, result, cv::TM_CCOEFF_NORMED);
    cv::threshold(result, result, Threshold, 1.0, cv::THRESH_TOZERO);
    double AccuracyValue;
    cv::minMaxLoc(result, NULL, &AccuracyValue, NULL, &Detectedlocation);
    return AccuracyValue >= Threshold;
}

void HookTracker::DetectionLoop()
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
        cv::resize(stage1Image, stage1Image, cv::Size(), 1.0 / HTVars.resizeMultiplier1080p, 1.0 / HTVars.resizeMultiplier1080p, cv::INTER_AREA);
        cv::resize(stage2Image, stage2Image, cv::Size(), 1.0 / HTVars.resizeMultiplier1080p, 1.0 / HTVars.resizeMultiplier1080p, cv::INTER_AREA);
    }

    cv::resize(stage1Image, stage1Image, cv::Size(
                   static_cast<int>(stage1Image.cols * HTVars.hudScaleFactor.value / 100.0),
                   static_cast<int>(stage1Image.rows * HTVars.hudScaleFactor.value / 100.0))
    );

    cv::resize(stage2Image, stage2Image, cv::Size(
                   static_cast<int>(stage2Image.cols * HTVars.hudScaleFactor.value / 100.0),
                   static_cast<int>(stage2Image.rows * HTVars.hudScaleFactor.value / 100.0))
    );
    
    cv::Rect region(0, 0, Backend::screenWidth / 3, Backend::screenHeight);
    cv::Mat frame;
    cv::Point detectedLocation;

    
    while (HTVars.enabled)
    {
        const auto frameStartTime = std::chrono::steady_clock::now();

        frame = Misc::GetScreenshot(region);

        if (HTVars.track1stStage)
        {
            if (TemplateMatch(frame, stage1Image, HTVars.firstThreshold, detectedLocation))
                HandleDetection(detectedLocation, Internal::survivorLocationsStage1, HTVars.firstDetectionRange);
        }

        if (HTVars.track2ndStage)
        {
            if (TemplateMatch(frame, stage2Image, HTVars.secondThreshold, detectedLocation))
                HandleDetection(detectedLocation, Internal::survivorLocationsStage2, HTVars.secondDetectionRange);
        }

        const auto frameEndTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsedTime = frameEndTime - frameStartTime;
        const auto timeToSleep = targetFrameDuration - elapsedTime;

        if (timeToSleep > std::chrono::duration<double>(0))
            std::this_thread::sleep_until(frameStartTime + targetFrameDuration);
    }
}

void HookTracker::HandleDetection(const cv::Point& detectedLocation, std::vector<ImVec2>& locations, int detectionRange)
{
    bool addSurvivor = true;

    const size_t stage1LocationsSize = locations.size();
    for (size_t i = 0; i < stage1LocationsSize; i++)
    {
        const auto survivor = locations[i];

        if (detectedLocation.y > survivor.y - detectionRange &&
            detectedLocation.y < survivor.y + detectionRange)
        {
            addSurvivor = false;
            break;
        }
    }

    if (addSurvivor || locations.empty())
    {
        locations.push_back(ImVec2(detectedLocation.x, detectedLocation.y));

        if (HTVars.playSoundOnHook)
            PlaySoundA(HTVars.soundFilePath, nullptr, SND_ASYNC);
    }
}

void HookTracker::RenderDetection()
{
    
    const size_t stage1Size = Internal::survivorLocationsStage1.size();
    const size_t stage2Size = Internal::survivorLocationsStage2.size();

    for (size_t i = 0; i < stage1Size; i++)
    {
        const auto location = Internal::survivorLocationsStage1[i];
        ImGui::GetBackgroundDrawList()->AddRect(location, ImVec2(location.x + 10, location.y + 10),
                                                ImColor(255, 0, 0));
    }

    for (size_t i = 0; i < stage2Size; i++)
    {
        const auto location = Internal::survivorLocationsStage2[i];
        ImGui::GetBackgroundDrawList()->AddRectFilled(location, ImVec2(location.x + 10, location.y + 10),
                                                ImColor(255, 0, 0));
    }
}
