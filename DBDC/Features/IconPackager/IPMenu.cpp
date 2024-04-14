#include "IPMenu.hpp"

#include "IconPackager.hpp"
#include "../../Misc/Misc.hpp"
#include "GUI/GUI.h"
#include "Images/Images.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

void ip_menu::setup()
{
    reload_packages();
}

void ip_menu::render_ui()
{
    ImGui::Columns(4, nullptr, false);
    ImGui::SetCursorPosY(45.F);
    ImGui::SeparatorText("Filtering");
    gui::drop_down_box("Role Filter", _internal::character_filter[_internal::character_filter_index], _internal::character_filter_index, _internal::character_filter, 80.F);
    gui::drop_down_box("Type Filter", _internal::type_filter[_internal::type_filter_index], _internal::type_filter_index, _internal::type_filter, 80.F);
    ImGui::SetNextItemWidth(170.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_text, IM_ARRAYSIZE(_internal::searched_text));

    ImGui::SeparatorText("Packaging");
    ImGui::PushStyleColor(ImGuiCol_Border, menu::styling::menu_accent.to_imvec4());
    ImGui::SetNextItemWidth(170.F);
    if (ImGui::ListBox("##Packages", &_internal::package_selector::loaded_package,
                       [](void* data, int idx, const char** outText)
                       {
                           *outText = _internal::package_selector::all_packages.at(idx).c_str();
                           return true;
                       },
                       NULL, (int)_internal::package_selector::all_packages.size(), 3))
    {
        if (_internal::package_selector::loaded_package < _internal::package_selector::all_packages.size())
        {
            _internal::package_selector::loaded_package_name = _internal::package_selector::all_packages.at(
                _internal::package_selector::loaded_package);

            perk_packager::_internal::package_data = ml::json_get_data_from_file(
                backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                _internal::package_selector::loaded_package_name + ".json"
            );

            perk_packager::clear_images();

            perk_packager::reload();
        }
    }

    ImGui::SetNextItemWidth(170.F);
    ImGui::InputTextWithHint("##PackageName", "Package Name", &_internal::package_selector::input_package_name);

    if (ImGui::Button("Create Package", {170.F, 0}))
    {
        auto lastNonSpace = std::find_if_not(_internal::package_selector::input_package_name.rbegin(), _internal::package_selector::input_package_name.rend(), ::isspace);
        _internal::package_selector::input_package_name.resize(_internal::package_selector::input_package_name.rend() - lastNonSpace);

        if (std::ranges::find(_internal::package_selector::all_packages, _internal::package_selector::input_package_name) == _internal::package_selector::all_packages.end())
        {
            const std::filesystem::path file_path = backend::exe_directory.string() + backend::settings_directory +
                _internal::package_selector::package_data_directory + _internal::package_selector::input_package_name + ".json";

            if (_internal::package_selector::input_package_name.size() > 0 &&
                !ml::file_or_directory_exists(file_path.string()))
            {
                if (!exists(file_path.parent_path()))
                    ml::create_directory(file_path.parent_path().string());

                ml::create_file(file_path.string());

                _internal::package_selector::input_package_name = "";

                reload_packages();
            }
        }
    }
    ImGui::PopStyleColor();
    gui::tool_tip("Press Enter To Create Package profile", 500, false);

    ImGui::Spacing();

    if (ImGui::Button("Compile Package", {170, 0}))
    {
        const std::string root_directory = backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_directory +
            _internal::package_selector::loaded_package_name + "\\DeadByDaylight\\Content\\";

        ml::create_directory(root_directory);

        for (const auto& p : perk_packager::_internal::package_data)
        {
            const std::string source = p["local_file_path"];
            std::filesystem::path destination = p["game_file_path"];
            std::string destination_string = destination.string();

            std::replace(destination_string.begin(), destination_string.end(), '/', '\\');

            ml::create_directory(root_directory + destination.parent_path().string());

            std::filesystem::copy_file(source, root_directory + destination.string(), std::filesystem::copy_options::overwrite_existing);
        }

        ml::open_directory(backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_directory);
    }

    ImGui::Spacing();

    if (ImGui::Button("Apply Package", {170, 0}))
    {
        const auto result = MessageBoxA(NULL, "This Will Apply All Currently Selected Icons To Your Game, Continue?", "Note...", MB_YESNO);

        if (result == IDYES)
        {
            const std::string root_directory = misc::get_game_root_directory() + "\\DeadByDaylight\\Content\\";

            for (const auto& p : perk_packager::_internal::package_data)
            {
                const std::string source = p["local_file_path"];
                std::filesystem::path destination = p["game_file_path"];
                std::string destination_string = destination.string();

                std::replace(destination_string.begin(), destination_string.end(), '/', '\\');

                ml::create_directory(root_directory + destination.parent_path().string());

                std::filesystem::copy_file(source, root_directory + destination.string(), std::filesystem::copy_options::overwrite_existing);
            }   
        }
    }

    ImGui::Spacing();

    if (ImGui::Button("Delete Package", {170, 0}))
    {
        const auto result = MessageBoxA(NULL, "Are You Sure?", "Note...", MB_YESNO);

        if (result == IDYES)
        {
            const auto it = std::ranges::find(_internal::package_selector::all_packages, _internal::package_selector::loaded_package_name);
            _internal::package_selector::all_packages.erase(it);

            const std::filesystem::path file_path = backend::exe_directory.string() + backend::settings_directory +
                _internal::package_selector::package_data_directory + _internal::package_selector::loaded_package_name + ".json";

            std::filesystem::remove(file_path);   
        }
    }

    ImGui::EndColumns();
    ImGui::SetCursorPos({195, 10});
    gui::begin_group_box("perk display", ImVec2({540, 0}), NULL);

    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 1)
        display_base_item(perk_packager::all_perks);
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 2)
        display_base_item(perk_packager::all_items);
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 3)
        display_base_item(perk_packager::all_offerings);
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 4)
        display_base_item(perk_packager::all_addons);
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 5)
        display_base_item(perk_packager::all_portraits);
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 6)
        display_base_item(perk_packager::all_status_effects);

    gui::end_group_box();
}

template <typename T>
void ip_menu::display_base_item(std::vector<T>& vec_obj)
{
    const static auto start_cursor_pos = ImGui::GetCursorPos();
    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;

    for (int i = 0; i < vec_obj.size(); i++)
    {
        auto& obj = vec_obj[i];

        const std::string searched_item = ml::to_lower(ip_menu::_internal::searched_text);
        const bool found_searched_item = ml::to_lower(obj.name).find(ml::to_lower(searched_item)) != std::string::npos;

        if (!found_searched_item && (ml::to_lower(obj.owner).find(ml::to_lower(searched_item)) == std::string::npos))
            continue;

        const bool has_filter = ip_menu::_internal::character_filter_index != 0;
        const bool is_correct_filter = ml::to_lower(ip_menu::_internal::character_filter[ip_menu::_internal::character_filter_index]) == obj.role;

        if (has_filter && !is_correct_filter)
            continue;

        if (current_x + 94.F > remaining_width)
        {
            ImGui::NewLine();
            current_x = ImGui::GetCursorPosX();
            new_line = true;
        }

        if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
            ImGui::SameLine();

        const std::string box_name = obj.name + ml::to_string(i);
        gui::begin_group_box(box_name.c_str(), ImVec2(94.F, 94.F));


        if (obj.has_selected_image == false)
        {
            ImGui::PushFont(menu::styling::child_font);
            ImGui::TextWrapped(obj.name.c_str());
            ImGui::PopFont();

            ImGui::SetCursorPos({41, 55});
            ImGui::Text("+");

            ImGui::SetCursorPos({30, 50});
            if (ImGui::InvisibleButton("##AddImage", ImVec2(32, 32)))
            {
                obj.local_image_path = ml::open_file_dialog();
                obj.game_file_path = perk_packager::_internal::all_perks_data[obj.id]["image"];

                if (!obj.local_image_path.empty())
                {
                    images::load_texture_from_file(obj.local_image_path, &obj.image);
                    obj.has_selected_image = true;

                    perk_packager::_internal::package_data[obj.name]["game_file_path"] = obj.game_file_path;
                    perk_packager::_internal::package_data[obj.name]["local_file_path"] = obj.local_image_path;

                    if (!ip_menu::_internal::package_selector::loaded_package_name.empty())
                        ml::json_write_data(
                            backend::exe_directory.string() + backend::settings_directory + ip_menu::_internal::package_selector::package_data_directory +
                            ip_menu::_internal::package_selector::loaded_package_name + ".json",

                            perk_packager::_internal::package_data
                        );
                }
            }
        }
        else
        {
            ImGui::SetCursorPos({1, 1});
            ImGui::Image((void*)obj.image, ImVec2(91, 91));
            gui::tool_tip(obj.name, 500, false);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::SetCursorPos({80, -1});
            ImGui::Text("x");
            ImGui::SetCursorPos({79, 4});
            if (ImGui::InvisibleButton("x", ImVec2(10, 15)))
            {
                obj.has_selected_image = false;
                obj.image = -1;
                obj.local_image_path = "";

                perk_packager::_internal::package_data.erase(obj.name);
                ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + ip_menu::_internal::package_selector::package_data_directory +
                                    ip_menu::_internal::package_selector::loaded_package_name + ".json",

                                    perk_packager::_internal::package_data);
            }
            ImGui::PopStyleColor();
        }

        gui::end_group_box();

        gui::tool_tip(
            "Type: " + obj.base_type +
            "\n"
            );

        if (i == 0)
            ImGui::SameLine();

        current_x += 94.F;
        remaining_width = ImGui::GetContentRegionAvail().x;
        new_line = false;
    }
}

void ip_menu::reload_packages()
{
    const std::filesystem::path iterator_path = backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory;

    if (!exists(iterator_path))
        return;

    _internal::package_selector::all_packages.clear();

    for (const auto& entry : std::filesystem::directory_iterator(iterator_path))
    {
        if (entry.path().extension() != ".json")
            continue;

        std::string fileName = entry.path().filename().string();

        if (std::ranges::find(_internal::package_selector::all_packages, fileName) != _internal::package_selector::all_packages.end())
            continue;

        _internal::package_selector::all_packages.push_back(fileName.substr(0, fileName.find_last_of('.')));
    }
}
