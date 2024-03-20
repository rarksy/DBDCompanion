#include "PPMenu.h"

#include "PerkPackager.h"
#include "../GUI/GUI.h"
#include "Fonts/Rethink.hpp"
#include "Images/Images.h"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
    reload_packages();
}

void pp_menu::render_ui()
{
    const float box_width = 93.F;
    const float box_height = 93.F;

    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_perk, IM_ARRAYSIZE(_internal::searched_perk));
    ImGui::SameLine();

    gui::drop_down_box("Filter", _internal::all_filters[_internal::perk_filter], _internal::perk_filter, _internal::all_filters, 80.F);

    ImGui::PushStyleColor(ImGuiCol_Border, menu::styling::menu_accent.to_imvec4());

    ImGui::SameLine();
    ImGui::SetCursorPosY(6.F);
    ImGui::SetNextItemWidth(150.F);
    if (ImGui::ListBox("##Packages", &_internal::package_selector::loaded_package,
                       [](void* data, int idx, const char** outText)
                       {
                           *outText = _internal::package_selector::all_packages.at(idx).c_str();
                           return true;
                       },
                       NULL, (int)_internal::package_selector::all_packages.size(), 2))
    {
        if (_internal::package_selector::loaded_package < _internal::package_selector::all_packages.size())
        {
            _internal::package_selector::loaded_package_name = _internal::package_selector::all_packages.at(
                _internal::package_selector::loaded_package);

            //
            
            perk_packager::_internal::package_data = ml::json_get_data_from_file(
                backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                _internal::package_selector::loaded_package_name + ".json"
            );

            perk_packager::reload();
        }
    }

    ImGui::SetCursorPos({299.F, 69.F});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PackageName", "Package Name", _internal::package_selector::input_package_name,
                             IM_ARRAYSIZE(_internal::package_selector::input_package_name));

    if (ImGui::IsItemHovered() &&
        ImGui::IsKeyPressed(ImGuiKey_Enter, false))
    {
        const std::filesystem::path file_path = backend::exe_directory.string() + backend::settings_directory +
            _internal::package_selector::package_data_directory + _internal::package_selector::input_package_name + ".json";

        if (strlen(_internal::package_selector::input_package_name) > 0 &&
            !ml::file_or_directory_exists(file_path.string()))
        {
            if (!exists(file_path.parent_path()))
                ml::create_directory(file_path.parent_path().string());

            ml::create_file(file_path.string());
            
        }
    }
    ImGui::PopStyleColor();
    gui::tool_tip("Press Enter To Create Package", 500, false);

    gui::begin_group_box("perk display", ImVec2(0, 380), NULL);

    const static auto start_cursor_pos = ImGui::GetCursorPos();
    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;
    for (int i = 0; i < perk_packager::all_characters.size(); i++)
    {
        auto& character = perk_packager::all_characters[i];
        for (int j = 0; j < character.perks.size(); j++)
        {
            const std::string perk_search = ml::to_lower(_internal::searched_perk);

            auto& perk = character.perks[j];

            if (ml::to_lower(perk.name).find(perk_search) == std::string::npos)
                continue;

            if ((_internal::perk_filter != 0 && ml::to_lower(_internal::all_filters[_internal::perk_filter]) != character.perks[j].role))
                continue;

            if (current_x + box_width > remaining_width)
            {
                ImGui::NewLine();
                current_x = ImGui::GetCursorPosX();
                new_line = true;
            }

            if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
                ImGui::SameLine();

            const std::string box_name = character.name + ml::to_string(j);
            gui::begin_group_box(box_name.c_str(), ImVec2(box_width, box_height));

            if (perk.has_selected_image == false)
            {
                ImGui::PushFont(menu::styling::child_font);
                ImGui::TextWrapped(perk.name.c_str());
                ImGui::PopFont();

                ImGui::SetCursorPos({41, 55});
                ImGui::Text("+");

                ImGui::SetCursorPos({30, 50});
                if (ImGui::InvisibleButton("##AddImage", ImVec2(32, 32)))
                {
                    perk.image_path = ml::open_file_dialog();

                    if (!perk.image_path.empty())
                    {
                        images::load_texture_from_file(perk.image_path, &perk.image);
                        perk.has_selected_image = true;

                        perk_packager::_internal::package_data[perk.name]["path"] = perk.image_path;

                        if (!_internal::package_selector::loaded_package_name.empty())
                            ml::json_write_data(
                                backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                                _internal::package_selector::loaded_package_name + ".json",

                                perk_packager::_internal::package_data
                            );
                    }
                }
            }
            else
            {
                ImGui::SetCursorPos({1, 1});
                ImGui::Image((void*)perk.image, ImVec2(91, 91));
                gui::tool_tip(perk.name, 500, false);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::SetCursorPos({80, -1});
                ImGui::Text("x");
                ImGui::SetCursorPos({79, 4});
                if (ImGui::InvisibleButton("x", ImVec2(10, 15)))
                {
                    perk.has_selected_image = false;
                    perk.image = -1;
                    perk.image_path = "";

                    perk_packager::_internal::package_data.erase(perk.name);
                }
                ImGui::PopStyleColor();
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

void pp_menu::reload_packages()
{
    for (const auto& entry : std::filesystem::directory_iterator(
         backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory))
    {
        if (entry.path().extension() != ".json")
            continue;

        std::string fileName = entry.path().filename().string();

        if (std::ranges::find(_internal::package_selector::all_packages, fileName) != _internal::package_selector::all_packages.end())
            continue;

        _internal::package_selector::all_packages.push_back(fileName.substr(0, fileName.find_last_of('.')));
    }
}
