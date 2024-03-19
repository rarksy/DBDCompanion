#include "PPMenu.h"

#include "PerkPackager.h"
#include "../GUI/GUI.h"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
}

void pp_menu::render_ui()
{
    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_perk, IM_ARRAYSIZE(_internal::searched_perk));
    ImGui::SameLine();
    gui::drop_down_box("Perk Filter", _internal::all_filters[_internal::perk_filter], _internal::perk_filter, _internal::all_filters, 80.F);

    gui::begin_group_box("perk display", ImVec2(0, 380), NULL);

    for (int i = 0; i < perk_packager::all_characters.size(); i++)
    {
        const auto character = perk_packager::all_characters[i];
        for (int j = 0; j < 2; j++)
        {
            const std::string box_name = character.name + ml::to_string(j);
            const std::string perk_id = character.all_perks[j];
            const std::string perk_name = perk_packager::_internal::all_perks_data[perk_id]["name"];
            
            if (ImGui::GetCursorPos().x + 90 < ImGui::GetWindowWidth())
                ImGui::SameLine();
            
            gui::begin_group_box(box_name.c_str(), ImVec2(90, 90));
            
            ImGui::TextWrapped(perk_name.c_str());
            
            gui::end_group_box();
        }
    }

    gui::end_group_box();
}
