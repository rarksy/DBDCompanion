#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"

namespace HCMenu
{
    void Setup();
    void RenderUI();

    // Variable Name | Value
    using intSetting = std::pair<std::string, int>;
    using stringSetting = std::pair<std::string, std::string>;

    inline intSetting MenuScaleFactor = {"MenuScaleFactor", 100};
    inline intSetting HudScaleFactor = {"HudScaleFactor", 100};


    inline std::vector<std::string> UIScales = {"70", "75", "80", "85", "90", "95", "100"};
}