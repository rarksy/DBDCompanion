#include "PerkPackager.h"

#include "PPMenu.h"
#include "../../Backend/Backend.hpp"

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

    for (const auto& character : _internal::all_characters_data)
    {
        Character c;

        c.name = character["name"];
        c.id = character["id"];
        c.role = character["role"];

        for (int i = 0; i < character["perks"].size(); i++)
            c.all_perks.push_back(character["perks"][i]);

        all_characters.push_back(c);
    }
}
