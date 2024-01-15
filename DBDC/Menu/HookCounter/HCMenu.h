#pragma once
#include <map>
#include <string>
#include <vector>

#include "ImGui/imgui.h"

namespace HCMenu
{
    void Setup();
    void RenderUI();

    void DetectionLoop();

    void RenderDetection();

    struct Variables
    {
        bool enabled = false;

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
        inline std::vector<std::pair<ImVec2, bool>> survivorLocations;
        inline std::vector<std::pair<ImVec2, bool>> survivorLocationsStage2;
    };


    inline std::vector<std::string> UIScales = {"70", "75", "80", "85", "90", "95", "100"};
}

inline HCMenu::Variables HCVars;
