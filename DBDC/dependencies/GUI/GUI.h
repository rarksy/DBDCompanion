﻿#pragma once
#include <vector>
#include <string>
#include "ImGui/imgui.h"
#include "..\..\features\config_editor\config_editor.hpp"
#include "..\..\menu\menu.hpp"

struct color;

namespace gui
{
    bool begin_hamburger_menu(bool& open, float& width, float& height, const ImColor* color);
    void end_hamburger_menu(bool& open, int& tab, float& width, float& height);
    void draw_hamburger_menu(bool& open, ImColor* color);
    
    bool checkbox(const char* label, config_editor::setting& setting, bool invert = false);
    bool checkbox(const char* label, config_editor::setting& setting, int disabledValue, int enabledValue, bool invert = false);

    bool image_button(const char* label, GLuint image_texture, const ImVec2& size);
    
    bool slider(const char* label, config_editor::setting& setting, int minValue, int maxValue, bool clampMinMax = true);
    bool slider(const char* label, int& v, int min_value, int max_value, bool clamp_min_max = true);
    bool slider(const char* label, float& v, float minValue, float maxValue, bool clampMinMax = true);
    
    bool drop_down_box(const char* label, config_editor::setting& setting, std::vector<std::string> items, bool use_index = true, float widget_size = 0.F,
                     std::string caption = "", std::vector<unsigned*> textures = {}, ImVec2 texture_size = ImVec2(250, 100));
    bool drop_down_box(const char* label, int& index, std::vector<std::string> items, bool useIndex = true, float widgetSize = 100.F, std::string caption = "", std::vector<unsigned*> textures = {},
                 ImVec2 textureSize = ImVec2(250, 100));
    bool drop_down_box(const char* label, std::string preview_value, int& id, std::vector<std::string> items, const float& box_width = 100.F);
    
    bool input_int(const char* label, config_editor::setting& setting, float widgetWidth = 100.F);

    void tool_tip(std::string message, float text_wrap_size = 500.F, bool holdRightClick = true);
    void tool_tip(std::wstring message, float text_wrap_size = 500.F, bool holdRightClick = true);
    void tool_tip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick = true);

    bool color_picker(const char* label, color* col, bool show_label = false);


    void begin_group_box(const char* group_name, ImVec2 size = ImVec2(0, 0), bool show_box = true, ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    void end_group_box();


    bool tab(std::string label, GLuint image);
}
