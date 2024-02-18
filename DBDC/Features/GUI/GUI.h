#pragma once
#include <vector>
#include <string>
#include "ImGui/imgui.h"
#include "../ConfigEditor/ConfigEditor.hpp"
#include "../Features/Menu.h"

struct Color;

namespace GUI
{
    bool BeginHamburgerMenu(bool& open, float& width, float& height, ImColor* color);
    void EndhamburgerMenu(bool& open, int& tab, float& width, float& height, ImColor* color);
    void DrawHamburger(bool& open, ImColor* color);
    
    bool Checkbox(const char* label, ConfigEditor::Setting& setting, bool invert = false);
    bool Checkbox(const char* label, ConfigEditor::Setting& setting, int disabledValue, int enabledValue, bool invert = false);
    bool Slider(const char* label, ConfigEditor::Setting& setting, int minValue, int maxValue, bool clampMinMax = true);
    bool DropDownBox(const char* label, ConfigEditor::Setting& setting, std::vector<std::string> items, bool useIndex = true, float widgetSize = 100.F,
                     std::string caption = "", std::vector<unsigned*> textures = {}, ImVec2 textureSize = ImVec2(250, 100));
    bool DropDownBox(const char* label, int& index, std::vector<std::string> items, bool useIndex = true, float widgetSize = 100.F, std::string caption = "", std::vector<unsigned*> textures = {},
                 ImVec2 textureSize = ImVec2(250, 100));
    bool InputInt(const char* label, ConfigEditor::Setting& setting, float widgetWidth = 100.F);

    void ToolTip(std::string message, bool holdRightClick = true);
    void ToolTip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick = true);

    bool ColorPicker(const char* label, Color& col);
}
