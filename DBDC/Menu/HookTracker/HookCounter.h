#pragma once
#include <vector>

#include "ImGui/imgui.h"
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include "../ConfigEditor/ConfigEditor.hpp"

namespace HookTracker
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

        ConfigEditor::Setting menuScaleFactor = ConfigEditor::Setting(ConfigEditor::Files::gameUserSettings, ConfigEditor::Sections::DBDGameUserSettings, "MenuScaleFactor", 100);
        ConfigEditor::Setting hudScaleFactor = ConfigEditor::Setting(ConfigEditor::Files::gameUserSettings, ConfigEditor::Sections::DBDGameUserSettings, "HudScaleFactor", 100);

        float firstThreshold = 0.9f;
        float secondThreshold = 0.9f;
    };

    namespace Internal
    {
        inline std::vector<ImVec2> survivorLocationsStage1;
        inline std::vector<ImVec2> survivorLocationsStage2;
        inline GLuint pipIconTexture;
    };
}

inline HookTracker::Variables HTVars;
