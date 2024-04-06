#include "PerkPackager.h"
#include "PPMenu.h"
#include "../../Backend/Backend.hpp"
#include "Images/Images.h"

void perk_packager::setup()
{
    ml::create_directory(backend::exe_directory.string() + backend::settings_directory + "data");
    
    const std::string character_data_file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "character_data.json";
    const std::string perk_data_file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "perk_data.json";
    const std::string item_data_file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "item_data.json";
    const std::string offering_data_file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "offering_data.json";

    const int time_since_cache = ml::get_seconds_since_file_modified(character_data_file_path);
    if (time_since_cache > 86400 /* 1d */ || time_since_cache == -1)
    {
        _internal::all_characters_data = ml::json_get_from_url("https://dbd.tricky.lol/api/characters");
        _internal::all_perks_data = ml::json_get_from_url("https://dbd.tricky.lol/api/perks");
        _internal::all_items_data = ml::json_get_from_url("https://dbd.tricky.lol/api/items");
        _internal::all_offerings_data = ml::json_get_from_url("https://dbd.tricky.lol/api/offerings");

        if (_internal::all_characters_data.empty() || _internal::all_perks_data.empty() || _internal::all_items_data.empty() || _internal::all_offerings_data.empty())
        {
            _internal::unavailable = true;
            return;
        }
        
        ml::json_write_data(character_data_file_path, _internal::all_characters_data);
        ml::json_write_data(perk_data_file_path, _internal::all_perks_data);
        ml::json_write_data(item_data_file_path, _internal::all_items_data);
        ml::json_write_data(offering_data_file_path, _internal::all_offerings_data);
    }
    else
    {
        _internal::all_characters_data = ml::json_get_data_from_file(character_data_file_path);
        _internal::all_perks_data = ml::json_get_data_from_file(perk_data_file_path);
        _internal::all_items_data = ml::json_get_data_from_file(item_data_file_path);
        _internal::all_offerings_data = ml::json_get_data_from_file(offering_data_file_path);
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
            p.game_file_path = _internal::all_perks_data[p.id]["image"];

            c.perks.push_back(p);
        }

        all_characters.push_back(c);
    }

    for (const auto& surv_general_id : _internal::all_survivor_general_perks)
    {
        character c;

        c.name = _internal::all_perks_data[surv_general_id]["name"];
        c.id = surv_general_id;
        c.role = "survivor";

        perk p;
        p.id = c.id;
        p.name = c.name;
        p.role = c.role;
        p.game_file_path = _internal::all_perks_data[p.id]["image"];

        c.perks.push_back(p);

        all_characters.push_back(c);
    }

    for (const auto& killer_general_id : _internal::all_killer_general_perks)
    {
        character c;

        c.name = _internal::all_perks_data[killer_general_id]["name"];
        c.id = killer_general_id;
        c.role = "killer";

        perk p;
        p.id = c.id;
        p.name = c.name;
        p.role = c.role;
        p.game_file_path = _internal::all_perks_data[p.id]["image"];

        c.perks.push_back(p);

        all_characters.push_back(c);
    }

    for (const auto& i : _internal::all_items_data)
    {
        if (i["name"].is_null() || i["role"].is_null() || i["image"].is_null())
            continue;
        
        item _item;
    
        _item.name = i["name"];
        _item.role = i["role"];
        _item.game_file_path = i["image"];
    
        all_items.push_back(_item);
    }

    for (const auto& data : _internal::all_offerings_data)
    {
        if (data["name"].is_null() || data["role"].is_null())
            continue;

        offering o;

        o.name = data["name"];
        o.role = data["role"];
        o.game_file_path = data["image"];

        all_offerings.push_back(o);
    }
}

void perk_packager::clear_images()
{
    for (auto& chr : all_characters)
    {
        for (auto& p : chr.perks)
        {
            p.has_selected_image = false;
            p.image = 0;
            p.local_image_path = "";
            p.game_file_path = "";
        }
    }

    for (auto& i : all_items)
    {
        i.has_selected_image = false;
        i.image = 0;
        i.local_image_path = "";
        i.game_file_path = "";
    }

    for (auto& o : all_offerings)
    {
        o.name = "";
        o.role = "";
        o.game_file_path = "";
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
                p.local_image_path = it.value()["local_file_path"];
                p.game_file_path = it.value()["game_file_path"];
                images::load_texture_from_file(p.local_image_path, &p.image);
                p.has_selected_image = true;
            }
        }
    }

    for (auto& i : all_items)
    {
        auto it = _internal::package_data.find(i.name);
        if (it != _internal::package_data.end())
        {
            i.local_image_path = it.value()["local_file_path"];
            i.game_file_path = it.value()["game_file_path"];
            images::load_texture_from_file(i.local_image_path, &i.image);
        }
    }

    for (auto& o : all_offerings)
    {
        auto it = _internal::package_data.find(o.name);
        if (it != _internal::package_data.end())
        {
            o.name = it.value()["name"];
            o.role = it.value()["role"];
            o.game_file_path = it.value()["image"];
        }
    }
}
