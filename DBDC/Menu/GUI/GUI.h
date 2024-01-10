#pragma once
#include <vector>
#include <string>
#include "ImGui/imgui.h"

namespace GUI
{
    bool IntCheckbox(const char* label, std::pair<std::string, int>& setting);
    bool StringCheckbox(const char* label, std::pair<std::string, std::string>& setting);

    bool Slider(const char* label, std::pair<std::string, int>& setting, int vMin, int vMax, bool clamp = true);
    
    bool DropDownBox(const char* label, std::vector<std::string> listItems, std::pair<std::string, int>& setting, bool accessValue = false,
        std::string toolTipCaption = "", std::vector<unsigned*> textures = {}, ImVec2 textureSize = ImVec2(250, 100));

    void ToolTip(std::string message, bool holdRightClick = true);
    void ToolTip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick = true);
}
