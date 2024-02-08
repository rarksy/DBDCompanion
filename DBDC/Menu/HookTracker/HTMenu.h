#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"

namespace HTMenu
{
    void Setup();
    void RenderUI();

    inline std::vector<std::string> UIScales = {"70", "75", "80", "85", "90", "95", "100"};
}