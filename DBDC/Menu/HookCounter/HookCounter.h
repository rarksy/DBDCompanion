#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"
#include <opencv2/opencv.hpp>

namespace HookCounter
{
    void DetectionLoop();
    void HandleDetection(const cv::Point& detectedLocation, std::vector<ImVec2>& locations);
    void RenderDetection();

    struct Variables
    {
        bool enabled = false;

        bool track1stStage = false;
        bool track2ndStage = false;
        bool playSoundOnHook = false;
        char soundFilePath[128];
        
        // Variable Name | Value
        using intSetting = std::pair<std::string, int>;
        using stringSetting = std::pair<std::string, std::string>;

        intSetting MenuScaleFactor = {"MenuScaleFactor", 100};
        intSetting HudScaleFactor = {"HudScaleFactor", 100};
    };

    namespace Internal
    {
        inline std::vector<ImVec2> survivorLocationsStage1;
        inline std::vector<ImVec2> survivorLocationsStage2;
    };
}

inline HookCounter::Variables HCVars;
