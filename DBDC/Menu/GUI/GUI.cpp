#include "GUI.h"
#include  "../Menu.h"
#include "..\ConfigEditor\ConfigEditor.hpp"

bool GUI::IntCheckbox(const char* label, std::pair<std::string, int>& setting)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    bool isChecked = (setting.second != 0);
    const bool valueChanged = ImGui::Checkbox(label, (&isChecked));

    if (valueChanged)
        setting.second = static_cast<int>(isChecked);

    return valueChanged;
}

bool GUI::StringCheckbox(const char* label, std::pair<std::string, std::string>& setting)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    bool isChecked = (setting.second != vFalse);
    const bool valueChanged = ImGui::Checkbox(label, &isChecked);

    if (valueChanged)
        setting.second = isChecked ? vTrue : vFalse;

    return valueChanged;
}

bool GUI::Slider(const char* label, std::pair<std::string, int>& setting, int vMin, int vMax, bool clamp)
{
    ImGui::SetNextItemWidth(static_cast<float>(Menu::Styling::itemWidth));

    return ImGui::SliderInt(label, &setting.second, vMin, vMax, "%d", clamp ? ImGuiSliderFlags_AlwaysClamp : 0);
}

bool GUI::DropDownBox(const char* label, std::vector<std::string> listItems,
                      std::pair<std::string, int>& setting, bool accessValue, std::string toolTipCaption, std::vector<unsigned*> textures,
                      ImVec2 textureSize)
{
    ImGui::SetNextItemWidth(Menu::Styling::itemWidth);

    auto it = std::ranges::find_if(listItems, [&setting](const auto& v)
    {
        return std::to_string(setting.second) == v;
    });

    bool itemSelected = false;

    if (ImGui::BeginCombo(label, accessValue ? it->c_str() : listItems[setting.second].c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < listItems.size(); i++)
        {
            const bool isSelected = (setting.second == i);
            if (ImGui::Selectable(listItems[i].c_str(), isSelected))
            {
                setting.second = accessValue ? std::atoi(listItems[i].c_str()) : i;

                itemSelected = true;
            }

            if (!textures.empty())
                GUI::ToolTip(toolTipCaption, *textures[i], textureSize);
        }

        ImGui::EndCombo();
    }

    return itemSelected;
}

bool GUI::DropDownBox(const char* label, std::vector<std::string> listItems, int& index)
{
    bool itemSelected;
    ImGui::SetNextItemWidth(Menu::Styling::itemWidth);
    if (ImGui::BeginCombo(label, listItems[index].c_str(), ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < listItems.size(); i++)
        {
            const bool isSelected = (index == i);
            if (ImGui::Selectable(listItems[i].c_str(), isSelected))
            {
                index = i;

                itemSelected = true;
            }
        }

        ImGui::EndCombo();
    }

    return itemSelected;
}

void GUI::ToolTip(std::string message, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(message.c_str());
}

void GUI::ToolTip(std::string message, unsigned int texture, const ImVec2& size, bool holdRightClick)
{
    if (holdRightClick && (!ImGui::IsKeyDown(ImGuiKey_MouseRight)))
        return;

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::Text(message.c_str());
        ImGui::Image((void*)texture, size);
        ImGui::EndTooltip();
    }
}
