#include "ShrineOfSecrets.hpp"

#include "../../Misc/Misc.hpp"
#include "../GUI/GUI.h"
#include "Images/Images.h"
#include "ImGui/imgui.h"
#include "miscLIB/miscLIB.hpp"

void shrine_of_secrets::init()
{
    shrine_data = ml::json_get("https://dbd.tricky.lol/api/shrine?includeperkinfo=1");

    if (!shrine_data["error"].is_null())
        unavailable = true;
    else
    {
        for (int i = 0; i < 4; i++)
        {
            const auto perk_info = ml::json_get("https://dbd.tricky.lol/api/perkinfo?perk=" + shrine_data["perks"][i]["id"].get_ref<std::string&>());

            perk _perk;

            _perk.id = perk_info["id"];
            _perk.name = perk_info["name"];
            _perk.description = perk_info["description"];

            all_perks.push_back(_perk);

            const std::string perk_path = perk_info["image"];
            const std::string full_image_path = misc::get_game_root_directory() + "DeadByDaylight/Content/" + perk_path;
            images::load_texture_from_file(full_image_path, &perk_images[i]);
            
            std::cout << "tid: " << perk_images[i] << std::endl;
        }
    }

    reset_time_end = shrine_data["end"];

    is_ready = true;
}

shrine_of_secrets::perk shrine_of_secrets::get_perk(const int& perk_index)
{
    if (is_ready)
        return all_perks[perk_index];

    return {};
}

void shrine_of_secrets::render_ui()
{
    ImGui::BeginGroup();

    ImGui::SeparatorText(
    shrine_of_secrets::is_ready
        ? (std::string("Shrine Of Secrets: Resets In ") + ml::unix_get_remaining_time(reset_time_end)).c_str()
        : "Shrine Of Secrets: Loading Data...");
    
    ImGui::Columns(4, nullptr, true);

    for (int i = 0; i < 4; i++)
    {
        const auto perk_info = get_perk(i);
    
        const std::string perk_name = perk_info.name;
        const std::string perk_id = perk_info.id;
        const std::string perk_description = perk_info.description;

        const float column_width = ImGui::GetColumnWidth();
        const ImVec2 image_size = ImVec2(100, 100);
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        
        const float image_x_position = ImGui::GetCursorPosX() + (column_width - image_size.x) / 2.0f - ImGui::GetScrollX() - spacing;
        
        // ImGui::SetCursorPosX(image_x_position);
        // ImGui::Image((void*)shrine_of_secrets::perk_images[i], image_size);
        // gui::tool_tip(perk_description.c_str(), 250.F);
        
        const float text_width = ImGui::CalcTextSize(perk_name.c_str()).x;
        
        const float text_x_position = ImGui::GetCursorPosX() + (column_width - text_width) / 2.0f - ImGui::GetScrollX() - spacing;
        
        ImGui::SetCursorPosX(text_x_position);
        ImGui::Text("%s", perk_name.c_str());
        gui::tool_tip(perk_description.c_str(), 250.F);
        
        
        if (i != 3)
            ImGui::NextColumn();
    }

    ImGui::EndGroup();
}
