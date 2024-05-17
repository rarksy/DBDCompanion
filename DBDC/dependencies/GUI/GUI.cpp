#include "GUI.h"
#include  "..\..\menu\menu.hpp"
#include "..\..\features\config_editor\config_editor.hpp"
#include "miscLIB/miscLIB.hpp"


bool gui::begin_hamburger_menu(bool& open, float& width, float& height, const ImColor* color)
{
    if (open || width > 0.F)
    {
        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(
            {5, 5},
            {width, height},
            *color,
            ImColor(15, 13, 13),
            ImColor(15, 13, 13),
            ImColor(15, 13, 13)
        );

        ImGui::GetWindowDrawList()->AddRect(
            {5, 5},
            {width, height},
            menu::styling::menu_accent.to_imcolor(),
            2.F,
            0,
            2.F
        );

        ImGui::PushClipRect(
            {5, 5},
            {width, height},
            false
        );
    }

    return (open || width > 0.F);
}

void gui::end_hamburger_menu(bool& open, int& tab, float& width, float& height)
{
    if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft, false) && !ImGui::IsMouseHoveringRect({5, 5}, {width, height}))
        open = false;

    static int previous_displayed_tab = 0;
    if (previous_displayed_tab != tab && !ImGui::IsMouseHoveringRect({5, 5}, {width, height}))
    {
        open = false;
        previous_displayed_tab = tab;
    }

    if (open && width < 200)
        width += 10;
    else if (!open && width > 0)
        width -= 10;

    if (open || width > 0.F)
        ImGui::PopClipRect();
}

void gui::draw_hamburger_menu(bool& open, ImColor* color)
{
    ImGui::GetWindowDrawList()->AddRectFilled({8, 10}, {38, 15}, *color, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({8, 20}, {38, 25}, *color, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({8, 30}, {38, 35}, *color, 4.F);

    ImGui::SetCursorPos({6, 6});
    if (ImGui::InvisibleButton("hamburgermenu", {40, 34}))
        open = !open;

    *color = ImGui::GetColorU32(ImGui::IsItemHovered() ? ImGui::IsItemActive() ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered : ImGuiCol_Button);
}

bool gui::checkbox(const char* label, config_editor::setting& setting, bool invert)
{
    ImGui::SetNextItemWidth(static_cast<float>(menu::styling::item_width));

    bool isChecked = invert ? !setting.value : setting.value;

    const bool valueChanged = ImGui::Checkbox(label, &isChecked);

    if (valueChanged)
    {
        setting.value = invert ? !isChecked : isChecked;
        setting.set_value();
    }

    return valueChanged;
}

bool gui::checkbox(const char* label, config_editor::setting& setting, int disabledValue, int enabledValue, bool invert)
{
    ImGui::SetNextItemWidth(static_cast<float>(menu::styling::item_width));

    bool isChecked = setting.value == enabledValue;
    const bool valueChanged = ImGui::Checkbox(label, &isChecked);

    if (valueChanged)
    {
        setting.value = isChecked ? invert ? disabledValue : enabledValue : disabledValue;
        setting.set_value();
    }

    return valueChanged;
}

bool gui::image_button(const char* label, GLuint image_texture, const ImVec2& size)
{
    const auto cursor_pos = ImGui::GetCursorPos();
    ImGui::Image(reinterpret_cast<void*>(image_texture), size);

    ImGui::SetCursorPos(cursor_pos);
    return ImGui::InvisibleButton(label, size);
}

bool gui::slider(const char* label, config_editor::setting& setting, int minValue, int maxValue, bool clampMinMax)
{
    ImGui::SetNextItemWidth(static_cast<float>(menu::styling::item_width));

    const bool valueChanged = ImGui::SliderInt(label, &setting.value, minValue, maxValue, "%d", clampMinMax ? ImGuiSliderFlags_AlwaysClamp : 0);

    if (valueChanged)
        setting.set_value();

    return valueChanged;
}

bool gui::drop_down_box(const char* label, int& index, std::vector<std::string> items, bool useIndex, float widgetSize, std::string caption, std::vector<unsigned*> textures,
                        ImVec2 textureSize)
{
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1F, 0.1F, 0.1F, 1.F));
    ImGui::SetNextItemWidth(widgetSize);

    auto it = std::ranges::find_if(items, [&index](const auto& v)
    {
        return std::to_string(index) == v;
    });

    bool itemSelected = false;

    if (ImGui::BeginCombo(label, useIndex ? items[index].c_str() : it->c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < items.size(); i++)
        {
            const bool isSelected = (index == i);
            if (ImGui::Selectable(items[i].c_str(), isSelected))
            {
                index = useIndex ? i : std::atoi(items[i].c_str());
                itemSelected = true;
            }

            if (!textures.empty())
                gui::tool_tip(caption, *textures[i], textureSize);
        }

        ImGui::EndCombo();
    }
    ImGui::PopStyleColor();

    return itemSelected;
}

bool gui::drop_down_box(const char* label, std::string preview_value, int& id, std::vector<std::string> items, const float& box_width)
{
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1F, 0.1F, 0.1F, 1.F));
    ImGui::SetNextItemWidth(box_width);

    bool item_selected = false;

    if (ImGui::BeginCombo(label, preview_value.c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int n = 0; n < items.size(); n++)
        {
            const bool is_selected = (id == n);
            if (ImGui::Selectable(items[n].c_str(), is_selected))
            {
                id = n;
                item_selected = true;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleColor();

    return item_selected;
}

bool gui::drop_down_box(const char* label, config_editor::setting& setting, std::vector<std::string> items, bool use_index, float widget_size, std::string caption,
                        std::vector<unsigned*> textures, ImVec2 texture_size)
{
    auto it = std::max_element(items.begin(), items.end(),
                           [](const auto& a, const auto& b) {
                               return a.size() < b.size();
                           });

    const std::string longest_item = *it;
    
    ImGui::SetNextItemWidth(widget_size == 0.F ? ImGui::CalcTextSize(longest_item.c_str()).x + 10 : widget_size);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1F, 0.1F, 0.1F, 1.F));

    it = std::ranges::find_if(items, [&setting](const auto& v)
    {
        return std::to_string(setting.value) == v;
    });

    bool itemSelected = false;

    if (ImGui::BeginCombo(label, use_index ? items[setting.value].c_str() : it->c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < items.size(); i++)
        {
            const bool isSelected = (setting.value == i);
            if (ImGui::Selectable(items[i].c_str(), isSelected))
            {
                setting.value = use_index ? i : std::atoi(items[i].c_str());
                setting.set_value();
                itemSelected = true;
            }

            if (!textures.empty())
                gui::tool_tip(caption, *textures[i], texture_size);
        }

        ImGui::EndCombo();
    }

    ImGui::PopStyleColor();
    return itemSelected;
}

bool gui::input_int(const char* label, config_editor::setting& setting, float widgetWidth)
{
    ImGui::SetNextItemWidth(widgetWidth);

    const bool valueChanged = ImGui::InputInt(label, &setting.value, 0, ImGuiInputTextFlags_CharsDecimal);

    if (valueChanged)
        setting.set_value();

    return valueChanged;
}

void gui::tool_tip(std::string message, float text_wrap_size, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.F);
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(text_wrap_size);
        ImGui::TextWrapped(message.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
        ImGui::PopStyleVar();
    }
}

void gui::tool_tip(std::wstring message, float text_wrap_size, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.F);
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(text_wrap_size);
        ImGui::TextWrapped(ml::wstring_to_string(message).c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
        ImGui::PopStyleVar();
    }
}

void gui::tool_tip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("%s", message.c_str());
        ImGui::Image(reinterpret_cast<void*>(texture), size);
        ImGui::EndTooltip();
    }
}

bool gui::color_picker(const char* label, color* col, bool show_label)
{
    ImGui::PushID(label);

    ImVec4 colorVec4 = col->to_imvec4();
    auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip;
    if (!show_label)
        flags |= ImGuiColorEditFlags_NoLabel;
        
    const bool valueChanged = ImGui::ColorEdit4(label, &colorVec4.x, flags);

    if (valueChanged)
    {
        col->apply_from_imvec4(colorVec4);
    }

    ImGui::PopID();

    return valueChanged;
}

void gui::begin_group_box(const char* group_name, ImVec2 size, bool show_box, ImGuiWindowFlags flags)
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.01f, 0.01f, 0.01f, 0.01f));
    ImGui::BeginChild(group_name, size, true, flags);
}

void gui::end_group_box()
{
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

bool gui::tab(std::string label, GLuint image)
{
    ImGui::PushID(label.c_str());

    ImGui::Image(reinterpret_cast<void*>(image), {23, 23});
    ImGui::SameLine();
    const auto cursor_pos = ImGui::GetCursorPos();

    ImVec4 default_text_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    ImColor text_color = default_text_color;

    static std::unordered_map<std::string, bool> hover_states;
    static std::unordered_map<std::string, float> line_widths;
    static std::unordered_map<std::string, float> anim_timers;

    if (hover_states.find(label) == hover_states.end())
        hover_states[label] = false;
    if (line_widths.find(label) == line_widths.end())
        line_widths[label] = 0.F;
    if (anim_timers.find(label) == anim_timers.end())
        anim_timers[label] = 0.F;

    ImGui::SetCursorPos({cursor_pos.x - 32, cursor_pos.y - 1});
    const auto text_size = ImGui::CalcTextSize(label.c_str());
    const bool return_value = ImGui::InvisibleButton((label + "tab").c_str(), ImVec2(35 + text_size.x, text_size.y + 13));

    float width_scroll_speed_increase = 0.3F;
    float width_scroll_speed_decrease = 0.2F;

    const bool hovered = ImGui::IsItemHovered();
    const bool active = ImGui::IsItemActive();
    if (hovered)
    {
        hover_states[label] = true;
        text_color = ImGui::IsItemActive() ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);

        // Extend the line width to match the width of the invisible button
        if (line_widths[label] < 25 + text_size.x)
            line_widths[label] += (text_size.x / width_scroll_speed_increase) * ImGui::GetIO().DeltaTime;
    }
    else
    {
        hover_states[label] = false;
        anim_timers[label] = 0.F;
    }

    // Smoothly decrease line width if not hovered
    if (!hover_states[label] && line_widths[label] > 0.F)
    {
        if (anim_timers[label] <= width_scroll_speed_decrease) // Adjust this value to control the speed of decrease
        {
            anim_timers[label] += ImGui::GetIO().DeltaTime;
            line_widths[label] -= (text_size.x / width_scroll_speed_decrease) * ImGui::GetIO().DeltaTime;
        }
        else
        {
            line_widths[label] = 0.F;
        }
    }

    ImGui::SetCursorPos({cursor_pos.x - 2, cursor_pos.y});
    ImGui::TextColored(text_color, label.c_str());

    if (line_widths[label] > 0)
    {
        ImGui::GetWindowDrawList()->AddRectFilled({cursor_pos.x - 25, cursor_pos.y + 29}, {cursor_pos.x - 25 + line_widths[label], cursor_pos.y + 33},
                                                  ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered),
                                                  5.F);
    }
    
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::PopID();

    return return_value;
}
