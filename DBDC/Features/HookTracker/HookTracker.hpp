#pragma once
#include <vector>

#include "ImGui/imgui.h"
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include "../ConfigEditor/ConfigEditor.hpp"

namespace HookTracker
{
    void DetectionLoop();
    void HandleDetection(const cv::Point& detectedLocation, std::vector<ImVec2>& locations, int detectionRange);
    void RenderDetection();

    struct Variables
    {
        bool enabled = false;

        bool track1stStage = true;
        bool track2ndStage = true;
        bool playSoundOnHook = false;
        char soundFilePath[128];

        config_editor::setting menuScaleFactor = config_editor::setting(config_editor::files::game_user_settings, config_editor::sections::dbd_game_user_settings, "MenuScaleFactor", 100);
        config_editor::setting hudScaleFactor = config_editor::setting(config_editor::files::game_user_settings, config_editor::sections::dbd_game_user_settings, "HudScaleFactor", 100);

        float firstThreshold = 0.9f;
        float secondThreshold = 0.9f;

        int firstDetectionRange = 7;
        int secondDetectionRange = 5;

        double resizeMultiplier1080p = 1.3;
    };

    namespace Internal
    {
        inline std::vector<ImVec2> survivorLocationsStage1;
        inline std::vector<ImVec2> survivorLocationsStage2;
        inline GLuint pipIconTexture;
    };
}

inline HookTracker::Variables HTVars;
