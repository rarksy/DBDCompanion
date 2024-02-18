#include "GUI.h"
#include  "../Menu.h"
#include "../ConfigEditor/ConfigEditor.hpp"


bool GUI::BeginHamburgerMenu(bool& open, int& tab, float& width, float& height, ImColor* color)
{
    if (open || width > 0.F)
    {
        ImGui::GetWindowDrawList()->AddRectFilledMultiColor(
            {5, 5},
            {width, height},
            *color,
            ImColor(25, 13, 13),
            ImColor(15, 13, 13),
            ImColor(15, 13, 13)
        );

        ImGui::GetWindowDrawList()->AddRect(
            {5, 5},
            {width, height},
            Menu::Styling::menuAccent.ToImColor(),
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

void GUI::EndhamburgerMenu(bool& open, int& tab, float& width, float& height, ImColor* color)
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

    ImGui::GetWindowDrawList()->AddRectFilled({8, 10}, {38, 15}, *color, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({8, 20}, {38, 25}, *color, 4.F);
    ImGui::GetWindowDrawList()->AddRectFilled({8, 30}, {38, 35}, *color, 4.F);

    ImGui::SetCursorPos({6, 6});
    if (ImGui::InvisibleButton("hamburgermenu", {40, 34}))
        open = !open;
    
    *color = ImGui::GetColorU32(ImGui::IsItemHovered() ? ImGui::IsItemActive() ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered : ImGuiCol_Button);

    ImGui::PopClipRect();
}

bool GUI::Checkbox(const char* label, ConfigEditor::Setting& setting, bool invert)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    bool isChecked = invert ? !setting.value : setting.value;

    const bool valueChanged = ImGui::Checkbox(label, &isChecked);

    if (valueChanged)
    {
        setting.value = invert ? !isChecked : isChecked;
        setting.SetValue();
    }

    return valueChanged;
}

bool GUI::Checkbox(const char* label, ConfigEditor::Setting& setting, int disabledValue, int enabledValue, bool invert)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    bool isChecked = setting.value == enabledValue;
    const bool valueChanged = ImGui::Checkbox(label, &isChecked);

    if (valueChanged)
    {
        setting.value = isChecked ? invert ? disabledValue : enabledValue : disabledValue;
        setting.SetValue();
    }

    return valueChanged;
}

bool GUI::Slider(const char* label, ConfigEditor::Setting& setting, int minValue, int maxValue, bool clampMinMax)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    const bool valueChanged = ImGui::SliderInt(label, &setting.value, minValue, maxValue, "%d", clampMinMax ? ImGuiSliderFlags_AlwaysClamp : 0);

    if (valueChanged)
        setting.SetValue();

    return valueChanged;
}

bool GUI::DropDownBox(const char* label, int& index, std::vector<std::string> items, bool useIndex, float widgetSize, std::string caption, std::vector<unsigned*> textures,
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
                GUI::ToolTip(caption, *textures[i], textureSize);
        }

        ImGui::EndCombo();
    }

    return itemSelected;
}

bool GUI::DropDownBox(const char* label, ConfigEditor::Setting& setting, std::vector<std::string> items, bool useIndex, float widgetSize, std::string caption,
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
                setting.SetValue();
                itemSelected = true;
            }

            if (!textures.empty())
                GUI::ToolTip(caption, *textures[i], textureSize);
        }

        ImGui::EndCombo();
    }

    return itemSelected;
}

bool GUI::InputInt(const char* label, ConfigEditor::Setting& setting, float widgetWidth)
{
    ImGui::SetNextItemWidth(widgetWidth);

    const bool valueChanged = ImGui::InputInt(label, &setting.value, 0, ImGuiInputTextFlags_CharsDecimal);

    if (valueChanged)
        setting.SetValue();

    return valueChanged;
}

void GUI::ToolTip(std::string message, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text("%s", message.c_str());
        ImGui::EndTooltip();
    }
}

void GUI::ToolTip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick)
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

bool GUI::ColorPicker(const char* label, Color& col)
{
    ImGui::PushID(label);

    ImVec4 colorVec4 = col.ToImVec4();
    const bool valueChanged = ImGui::ColorEdit4(label, &colorVec4.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoTooltip);

    if (valueChanged)
        col.ApplyFromImVec4(colorVec4);

    ImGui::PopID();

    return valueChanged;
}
