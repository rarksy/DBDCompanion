#include "PPMenu.h"

#include "PerkPackager.h"
#include "../GUI/GUI.h"
#include "Fonts/Rethink.hpp"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
}

void pp_menu::render_ui()
{
    const float box_width = 94.F;
    const float box_height = 94.F;

    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_perk, IM_ARRAYSIZE(_internal::searched_perk));
    ImGui::SameLine();
    gui::drop_down_box("Perk Filter", _internal::all_filters[_internal::perk_filter], _internal::perk_filter, _internal::all_filters, 80.F);

    gui::begin_group_box("perk display", ImVec2(0, 380), NULL);

    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;

    for (int i = 0; i < perk_packager::all_characters.size(); i++)
    {
        const auto character = perk_packager::all_characters[i];
        for (int j = 0; j < 2; j++)
        {
            const std::string box_name = character.name + ml::to_string(j);
            const std::string perk_id = character.all_perks[j];
            const std::string perk_name = perk_packager::_internal::all_perks_data[perk_id]["name"];

            if (current_x + box_width > remaining_width)
            {
                ImGui::NewLine();
                current_x = ImGui::GetCursorPosX();
                new_line = true;
            }

            if (!new_line && i > 0)
                ImGui::SameLine();

            gui::begin_group_box(box_name.c_str(), ImVec2(box_width, box_height));
            ImGui::PushFont(menu::styling::child_font);
            ImGui::TextWrapped(perk_name.c_str());
            ImGui::PopFont();
            gui::end_group_box();
            
            if (i == 0)
                ImGui::SameLine();

            current_x += box_width;
            remaining_width = ImGui::GetContentRegionAvail().x;
            new_line = false;
        }
    }

    gui::end_group_box();
}
