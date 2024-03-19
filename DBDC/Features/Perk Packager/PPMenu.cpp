#include "PPMenu.h"

#include "../GUI/GUI.h"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
    
}

void pp_menu::render_ui()
{
    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search" ,_internal::searched_perk, IM_ARRAYSIZE(_internal::searched_perk));
    ImGui::SameLine();
    gui::drop_down_box("Perk Filter", _internal::all_filters[_internal::perk_filter], _internal::perk_filter, _internal::all_filters, 80.F);

    gui::begin_group_box("perk display", ImVec2(0, 380));
    
    gui::end_group_box();
}
