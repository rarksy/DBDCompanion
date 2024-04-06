#include "PPMenu.h"

#include "PerkPackager.h"
#include "GUI/GUI.h"
#include "Images/Images.h"
#include "ImGui/imgui.h"

void pp_menu::setup()
{
    reload_packages();
}

void pp_menu::render_ui()
{
    ImGui::SetCursorPos({10, 55});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PerkSearch", "Perk Search", _internal::searched_text, IM_ARRAYSIZE(_internal::searched_text));
    ImGui::SameLine();

    ImGui::SetCursorPosY(20.F);
    gui::drop_down_box("Character Filter", _internal::character_filter[_internal::character_filter_index], _internal::character_filter_index, _internal::character_filter, 80.F);
    ImGui::SetCursorPos({168.F, 55.f});
    gui::drop_down_box("Type Filter", _internal::type_filter[_internal::type_filter_index], _internal::type_filter_index, _internal::type_filter, 80.F);

    ImGui::PushStyleColor(ImGuiCol_Border, menu::styling::menu_accent.to_imvec4());

    ImGui::SameLine();
    ImGui::SetCursorPos({429.F, 6.F});
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

            perk_packager::clear_images();

            perk_packager::_internal::package_data = ml::json_get_data_from_file(
                backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                _internal::package_selector::loaded_package_name + ".json"
            );

            perk_packager::reload();
        }
    }

    ImGui::SetCursorPos({429.F, 69.F});
    ImGui::SetNextItemWidth(150.F);
    ImGui::InputTextWithHint("##PackageName", "Package Name", _internal::package_selector::input_package_name,
                             IM_ARRAYSIZE(_internal::package_selector::input_package_name));

    if (ImGui::GetIO().WantCaptureKeyboard &&
        ImGui::IsKeyPressed(ImGuiKey_Enter, false))
    {
        if (std::ranges::find(_internal::package_selector::all_packages, _internal::package_selector::input_package_name) == _internal::package_selector::all_packages.end())
        {
            const std::filesystem::path file_path = backend::exe_directory.string() + backend::settings_directory +
                _internal::package_selector::package_data_directory + _internal::package_selector::input_package_name + ".json";

            if (strlen(_internal::package_selector::input_package_name) > 0 &&
                !ml::file_or_directory_exists(file_path.string()))
            {
                if (!exists(file_path.parent_path()))
                    ml::create_directory(file_path.parent_path().string());

                ml::create_file(file_path.string());

                ZeroMemory(_internal::package_selector::input_package_name, sizeof _internal::package_selector::input_package_name);

                reload_packages();
            }
        }
    }
    ImGui::PopStyleColor();
    gui::tool_tip("Press Enter To Create Package", 500, false);

    ImGui::SameLine();
    if (ImGui::Button("Create Package"))
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
    }

    gui::begin_group_box("perk display", ImVec2(0, 380), NULL);


    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 1)
        display_perks();

    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 2)
        display_items();
    
    if (_internal::type_filter_index == 0 || _internal::type_filter_index == 3)
        display_offerings();
    
    
    gui::end_group_box();
}

void pp_menu::display_perks()
{
    const static auto start_cursor_pos = ImGui::GetCursorPos();
    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;
    for (int i = 0; i < perk_packager::all_characters.size(); i++)
    {
        auto& character = perk_packager::all_characters[i];
        for (int j = 0; j < character.perks.size(); j++)
        {
            const std::string perk_search = ml::to_lower(_internal::searched_text);

            auto& perk = character.perks[j];

            const bool searched_perk = ml::to_lower(perk.name).find(perk_search) != std::string::npos;
            const bool searched_character = ml::to_lower(character.name).find(perk_search) != std::string::npos;

            if (!searched_perk && !searched_character)
                continue;

            const bool has_filter = _internal::character_filter_index != 0;
            const bool is_correct_filter = ml::to_lower(_internal::character_filter[_internal::character_filter_index]) == character.perks[j].role;

            if (has_filter && !is_correct_filter)
                continue;

            if (current_x + 93.F > remaining_width)
            {
                ImGui::NewLine();
                current_x = ImGui::GetCursorPosX();
                new_line = true;
            }

            if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
                ImGui::SameLine();

            const std::string box_name = character.name + ml::to_string(j);
            gui::begin_group_box(box_name.c_str(), ImVec2(93.F, 93.F));

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
                    perk.local_image_path = ml::open_file_dialog();
                    perk.game_file_path = perk_packager::_internal::all_perks_data[perk.id]["image"];

                    if (!perk.local_image_path.empty())
                    {
                        images::load_texture_from_file(perk.local_image_path, &perk.image);
                        perk.has_selected_image = true;

                        perk_packager::_internal::package_data[perk.name]["game_file_path"] = perk.game_file_path;
                        perk_packager::_internal::package_data[perk.name]["local_file_path"] = perk.local_image_path;

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
                    perk.local_image_path = "";

                    perk_packager::_internal::package_data.erase(perk.name);
                    ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                                        _internal::package_selector::loaded_package_name + ".json",

                                        perk_packager::_internal::package_data);
                }
                ImGui::PopStyleColor();
            }

            gui::end_group_box();

            if (i == 0)
                ImGui::SameLine();

            current_x += 93.F;
            remaining_width = ImGui::GetContentRegionAvail().x;
            new_line = false;
        }
    }
}

void pp_menu::display_items()
{
    const static auto start_cursor_pos = ImGui::GetCursorPos();
    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;
    for (int i = 0; i < perk_packager::all_items.size(); i++)
    {
        auto& item = perk_packager::all_items[i];

        const std::string searched_item = ml::to_lower(_internal::searched_text);

        const bool found_searched_item = ml::to_lower(item.name).find(ml::to_lower(searched_item)) != std::string::npos;

        if (!found_searched_item)
            continue;

        const bool has_filter = _internal::character_filter_index != 0;
        const bool is_correct_filter = ml::to_lower(_internal::character_filter[_internal::character_filter_index]) == item.role;


        if (has_filter && !is_correct_filter)
            continue;

        if (current_x + 93.F > remaining_width)
        {
            ImGui::NewLine();
            current_x = ImGui::GetCursorPosX();
            new_line = true;
        }

        if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
            ImGui::SameLine();

        const std::string box_name = item.name + ml::to_string(i);
        gui::begin_group_box(box_name.c_str(), ImVec2(93.F, 93.F));


        if (item.has_selected_image == false)
        {
            ImGui::PushFont(menu::styling::child_font);
            ImGui::TextWrapped(item.name.c_str());
            ImGui::PopFont();

            ImGui::SetCursorPos({41, 55});
            ImGui::Text("+");

            ImGui::SetCursorPos({30, 50});
            if (ImGui::InvisibleButton("##AddImage", ImVec2(32, 32)))
            {
                item.local_image_path = ml::open_file_dialog();
                item.game_file_path = perk_packager::_internal::all_perks_data[item.id]["image"];

                if (!item.local_image_path.empty())
                {
                    images::load_texture_from_file(item.local_image_path, &item.image);
                    item.has_selected_image = true;

                    perk_packager::_internal::package_data[item.name]["game_file_path"] = item.game_file_path;
                    perk_packager::_internal::package_data[item.name]["local_file_path"] = item.local_image_path;

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
            ImGui::Image((void*)item.image, ImVec2(91, 91));
            gui::tool_tip(item.name, 500, false);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::SetCursorPos({80, -1});
            ImGui::Text("x");
            ImGui::SetCursorPos({79, 4});
            if (ImGui::InvisibleButton("x", ImVec2(10, 15)))
            {
                item.has_selected_image = false;
                item.image = -1;
                item.local_image_path = "";

                perk_packager::_internal::package_data.erase(item.name);
                ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                                    _internal::package_selector::loaded_package_name + ".json",

                                    perk_packager::_internal::package_data);
            }
            ImGui::PopStyleColor();
        }

        gui::end_group_box();

        if (i == 0)
            ImGui::SameLine();

        current_x += 93.F;
        remaining_width = ImGui::GetContentRegionAvail().x;
        new_line = false;
    }
}

void pp_menu::display_offerings()
{
    const static auto start_cursor_pos = ImGui::GetCursorPos();
    float remaining_width = ImGui::GetContentRegionAvail().x;
    float current_x = ImGui::GetCursorPosX();
    bool new_line = false;
    for (int i = 0; i < perk_packager::all_offerings.size(); i++)
    {
        auto& offering = perk_packager::all_offerings[i];

        const std::string searched_item = ml::to_lower(_internal::searched_text);

        const bool found_searched_item = ml::to_lower(offering.name).find(ml::to_lower(searched_item)) != std::string::npos;

        if (!found_searched_item)
            continue;

        const bool has_filter = _internal::character_filter_index != 0;
        const bool is_correct_filter = ml::to_lower(_internal::character_filter[_internal::character_filter_index]) == offering.role;


        if (has_filter && !is_correct_filter)
            continue;

        if (current_x + 93.F > remaining_width)
        {
            ImGui::NewLine();
            current_x = ImGui::GetCursorPosX();
            new_line = true;
        }

        if (!new_line && i > 0 || ImGui::GetCursorPos().x == start_cursor_pos.x)
            ImGui::SameLine();

        const std::string box_name = offering.name + ml::to_string(i);
        gui::begin_group_box(box_name.c_str(), ImVec2(93.F, 93.F));


        if (offering.has_selected_image == false)
        {
            ImGui::PushFont(menu::styling::child_font);
            ImGui::TextWrapped(offering.name.c_str());
            ImGui::PopFont();

            ImGui::SetCursorPos({41, 55});
            ImGui::Text("+");

            ImGui::SetCursorPos({30, 50});
            if (ImGui::InvisibleButton("##AddImage", ImVec2(32, 32)))
            {
                offering.local_image_path = ml::open_file_dialog();
                offering.game_file_path = perk_packager::_internal::all_perks_data[offering.id]["image"];

                if (!offering.local_image_path.empty())
                {
                    images::load_texture_from_file(offering.local_image_path, &offering.image);
                    offering.has_selected_image = true;

                    perk_packager::_internal::package_data[offering.name]["game_file_path"] = offering.game_file_path;
                    perk_packager::_internal::package_data[offering.name]["local_file_path"] = offering.local_image_path;

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
            ImGui::Image((void*)offering.image, ImVec2(91, 91));
            gui::tool_tip(offering.name, 500, false);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::SetCursorPos({80, -1});
            ImGui::Text("x");
            ImGui::SetCursorPos({79, 4});
            if (ImGui::InvisibleButton("x", ImVec2(10, 15)))
            {
                offering.has_selected_image = false;
                offering.image = -1;
                offering.local_image_path = "";

                perk_packager::_internal::package_data.erase(offering.name);
                ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + _internal::package_selector::package_data_directory +
                                    _internal::package_selector::loaded_package_name + ".json",

                                    perk_packager::_internal::package_data);
            }
            ImGui::PopStyleColor();
        }

        gui::end_group_box();

        if (i == 0)
            ImGui::SameLine();

        current_x += 93.F;
        remaining_width = ImGui::GetContentRegionAvail().x;
        new_line = false;
    }
}

void pp_menu::reload_packages()
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
