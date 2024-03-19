#include "PPMenu.h"

#include "PerkPackager.h"
#include "../GUI/GUI.h"
#include "Fonts/Rethink.hpp"
#include "Images/Images.h"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
}

void pp_menu::render_ui()
{
    const float box_width = 93.F;
    const float box_height = 93.F;

    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_perk, IM_ARRAYSIZE(_internal::searched_perk));
    ImGui::SameLine();
    gui::drop_down_box("Perk Filter", _internal::all_filters[_internal::perk_filter], _internal::perk_filter, _internal::all_filters, 80.F);

    gui::begin_group_box("perk display", ImVec2(0, 380), NULL);

    const static auto start_cursor_pos = ImGui::GetCursorPos();

    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;

    for (int i = 0; i < perk_packager::all_characters.size(); i++)
    {
        const auto character = perk_packager::all_characters[i];
        for (int j = 0; j < character.perks.size(); j++)
        {
            const std::string perk_search = ml::to_lower(_internal::searched_perk);

            const std::string box_name = character.name + ml::to_string(j);

            if (ml::to_lower(character.perks[j].name).find(perk_search) == std::string::npos)
                continue;

            if ((_internal::perk_filter != 0 && ml::to_lower(_internal::all_filters[_internal::perk_filter]) != character.perks[i].role))
                continue;

            if (current_x + box_width > remaining_width)
            {
                ImGui::NewLine();
                current_x = ImGui::GetCursorPosX();
                new_line = true;
            }
            
            if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
                ImGui::SameLine();
            
            gui::begin_group_box(box_name.c_str(), ImVec2(box_width, box_height));
            ImGui::PushFont(menu::styling::child_font);
            ImGui::TextWrapped(character.perks[j].name.c_str());
            ImGui::PopFont();

            if (!character.perks[j].has_selected_image)
            {
                ImGui::SetCursorPos({38, 55});
                if (ImGui::Button("+"))
                {
                    std::string selected_file = ml::open_file_dialog();
                    
                    images::load_texture_from_file(selected_file, &character.perks[j].image);

                    character.perks[j].has_selected_image = true;
                }   
            }
            else
            {
                ImGui::SetCursorPos({20, 20});
                ImGui::Image((void*)character.perks[j].image, ImVec2(40, 40));
            }
            
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
