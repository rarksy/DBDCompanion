#pragma once
#include <string>
#include <vector>

namespace HCMenu
{
    void Setup();
    void RenderUI();
    void CreateStyle();

    struct Variables
    {
        bool enabled = false;
        
        // Variable Name | Value
        using intSetting = std::pair<std::string, int>;
        using stringSetting = std::pair<std::string, std::string>;

        intSetting MenuScaleFactor = {"MenuScaleFactor", 100};
        intSetting HudScaleFactor = {"HudScaleFactor", 100};
    };


    inline std::vector<std::string> UIScales = {"60", "65", "70", "75", "80", "85", "90", "95", "100"};
}

inline HCMenu::Variables HCVars;
