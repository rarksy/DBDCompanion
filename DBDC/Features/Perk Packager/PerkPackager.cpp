#include "PerkPackager.h"
#include <ranges>
#include "PPMenu.h"
#include "../../Backend/Backend.hpp"
#include "Images/Images.h"

void perk_packager::setup()
{
    const std::string character_data_file_path = backend::exe_directory.string() + backend::settings_directory + "character_data.json";
    const std::string perk_data_file_path = backend::exe_directory.string() + backend::settings_directory + "perk_data.json";

    const int time_since_cache = ml::get_seconds_since_file_modified(character_data_file_path);
    if (time_since_cache > 86400 /* 1d */ || time_since_cache == -1)
    {
        _internal::all_characters_data = ml::json_get_from_url("https://dbd.tricky.lol/api/characters");
        _internal::all_perks_data = ml::json_get_from_url("https://dbd.tricky.lol/api/perks");
        ml::json_write_data(character_data_file_path, _internal::all_characters_data);
        ml::json_write_data(perk_data_file_path, _internal::all_perks_data);
    }
    else
    {
        _internal::all_characters_data = ml::json_get_data_from_file(character_data_file_path);
        _internal::all_perks_data = ml::json_get_data_from_file(perk_data_file_path);
    }

    for (const auto& ch : _internal::all_characters_data)
    {
        character c;

        c.name = ch["name"];
        c.id = ch["id"];
        c.role = ch["role"];

        for (int i = 0; i < ch["perks"].size(); i++)
        {
            perk p;
            p.id = ch["perks"][i];
            p.name = _internal::all_perks_data[p.id]["name"];
            p.role = _internal::all_perks_data[p.id]["role"];

            c.perks.push_back(p);
        }

        all_characters.push_back(c);
    }

    for (const auto& surv_general_id : _internal::all_survivor_general_perks)
    {
        character c;

        c.name = _internal::all_perks_data[surv_general_id]["name"];
        c.id = "null";
        c.role = "survivor";

        perk p;
        p.id = c.id;
        p.name = c.name;
        p.role = c.role;

        c.perks.push_back(p);

        all_characters.push_back(c);
    }

    for (const auto& killer_general_id : _internal::all_killer_general_perks)
    {
        character c;

        c.name = _internal::all_perks_data[killer_general_id]["name"];
        c.id = "null";
        c.role = "killer";

        perk p;
        p.id = c.id;
        p.name = c.name;
        p.role = c.role;

        c.perks.push_back(p);

        all_characters.push_back(c);
    }
}

void perk_packager::reload()
{
    if (_internal::package_data.empty())
        return;
    
    for (auto& chr : all_characters)
    {
        for (auto& p : chr.perks)
        {
            auto it = _internal::package_data.find(p.name);
            if (it != _internal::package_data.end())
            {
                p.image_path = it.value()["path"];
                images::load_texture_from_file(p.image_path, &p.image);
                p.has_selected_image = true;
            }
        }
    }
}
