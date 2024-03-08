#include "GUI.h"
#include  "../Menu.h"
#include "../ConfigEditor/ConfigEditor.hpp"


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

    return itemSelected;
}

bool gui::drop_down_box(const char* label, config_editor::setting& setting, std::vector<std::string> items, bool useIndex, float widgetSize, std::string caption,
                        std::vector<unsigned*> textures, ImVec2 textureSize)
{
    ImGui::SetNextItemWidth(widgetSize);

    auto it = std::ranges::find_if(items, [&setting](const auto& v)
    {
        return std::to_string(setting.value) == v;
    });

    bool itemSelected = false;

    if (ImGui::BeginCombo(label, useIndex ? items[setting.value].c_str() : it->c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < items.size(); i++)
        {
            const bool isSelected = (setting.value == i);
            if (ImGui::Selectable(items[i].c_str(), isSelected))
            {
                setting.value = useIndex ? i : std::atoi(items[i].c_str());
                setting.set_value();
                itemSelected = true;
            }

            if (!textures.empty())
                gui::tool_tip(caption, *textures[i], textureSize);
        }

        ImGui::EndCombo();
    }

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
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(text_wrap_size);
        ImGui::TextWrapped(message.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
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

bool gui::color_picker(const char* label, color* col)
{
    ImGui::PushID(label);

    ImVec4 colorVec4 = col->to_imvec4();
    const bool valueChanged = ImGui::ColorEdit4(label, &colorVec4.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);

    if (valueChanged)
    {
        col->apply_from_imvec4(colorVec4);
    }

    ImGui::PopID();

    return valueChanged;
}

void RenderGroupBox(float x, float y, float width, float height, ImU32 color, float rounding = 0.0f, float thickness = 1.0f)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DrawList->AddRect(ImVec2(x, y), ImVec2(x + width, y + height), color, rounding, 0, thickness);
}

void gui::begin_group_box(const char* group_name, ImVec2 size)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.1f));
    ImGui::BeginChild(group_name, size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
}

void gui::end_group_box()
{
    ImGui::EndChild();
    ImGui::PopStyleColor();
}
