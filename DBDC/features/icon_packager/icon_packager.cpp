#include "icon_packager.hpp"
#include "ip_menu.hpp"
#include "../../backend/backend.hpp"
#include "images/images.h"
#include "miscLIB/miscLIB.hpp"

bool perk_packager::setup()
{
    ml::create_directory(backend::exe_directory.string() + backend::settings_directory + "data");

    const std::vector<std::pair<std::string, nlohmann::json&>> endpoints_data =
    {
        {"offerings", _internal::all_offerings_data},
        {"addons", _internal::all_addons_data},
        {"items", _internal::all_items_data},
        {"perks", _internal::all_perks_data},
    };

    const size_t endpoints_size = endpoints_data.size();
    for (size_t i = 0; i < endpoints_size; i++)
    {
        auto& endpoint_data = endpoints_data[i];
        _internal::unavailable = !get_endpoint_data(endpoint_data.first, endpoint_data.second);
    }

    if (_internal::unavailable)
        return false;

    load_instance_data(&all_offerings, _internal::all_offerings_data);
    load_instance_data(&all_addons, _internal::all_addons_data);
    load_instance_data(&all_items, _internal::all_items_data);

    /*
     * status effects need to be handled manually
     * due to the api not having an endpoint supplying
     * details for them.
     */

    for (const auto& se : _internal::all_status_effects_data)
    {
        status_effect e;

        e.name = se.first;
        e.role = "none";
        e.game_file_path = "UI/Icons/StatusEffects/iconStatusEffects_" + se.second + ".png";

        all_status_effects.push_back(e);
    }

    _internal::unavailable = !get_endpoint_data("characters", _internal::all_characters_data);
    _internal::unavailable = !get_endpoint_data("perks", _internal::all_perks_data);

    for (const auto& character_data : _internal::all_characters_data)
    {
        portrait _portrait;

        if (character_data.contains("name") && !character_data["name"].is_null())
            _portrait.name = character_data["name"];

        if (character_data.contains("id") && !character_data["id"].is_null())
        {
            _portrait.id = character_data["id"];

            const size_t perks_size = character_data["perks"].size();
            for (int i = 0; i < perks_size; i++)
            {
                const auto perk_id = character_data["perks"][i];
                const auto perk_data = _internal::all_perks_data[perk_id];
                
                perk _perk;
                
                _perk.owner = _portrait.name;
                
                if (perk_data.contains("name") && !perk_data["name"].is_null())
                    _perk.name = perk_data["name"];
                
                if (perk_data.contains("role") && !perk_data["role"].is_null())
                    _perk.role = perk_data["role"];
                
                if (perk_data.contains("image") && !perk_data["image"].is_null())
                    _perk.game_file_path = perk_data["image"];
                
                all_perks.push_back(_perk);
            }
        }

        if (character_data.contains("role") && !character_data["role"].is_null())
            _portrait.role = character_data["role"];
        

        if (character_data.contains("image") && !character_data["image"].is_null())
            _portrait.game_file_path = character_data["image"];

        all_portraits.push_back(_portrait);
    }

    for (const auto& general_surv_perk_id : _internal::all_survivor_general_perks)
    {
        perk p;
        const auto perk_info = _internal::all_perks_data[general_surv_perk_id];
    
        p.name = perk_info["name"];
        p.role = perk_info["role"];
        p.game_file_path = perk_info["image"];
        p.id = general_surv_perk_id;
    
        all_perks.push_back(p);
    }
    
    for (const auto& general_killer_perk_id : _internal::all_killer_general_perks)
    {
        perk p;
        const auto perk_info = _internal::all_perks_data[general_killer_perk_id];
    
        p.name = perk_info["name"];
        p.role = perk_info["role"];
        p.game_file_path = perk_info["image"];
        p.id = general_killer_perk_id;
    
        all_perks.push_back(p);
    }

    return true;
}

void perk_packager::clear_images()
{
    clear_instance_images(all_offerings);
    clear_instance_images(all_addons);
    clear_instance_images(all_items);
    clear_instance_images(all_perks);
    clear_instance_images(all_portraits);
    clear_instance_images(all_status_effects);
}

void perk_packager::reload()
{
    if (_internal::package_data.empty())
        return;

    reload_package_data(all_offerings);
    reload_package_data(all_addons);
    reload_package_data(all_items);
    reload_package_data(all_perks);
    reload_package_data(all_portraits);
    reload_package_data(all_status_effects);
}

bool perk_packager::get_endpoint_data(const std::string& endpoint, nlohmann::json& data)
{
    const std::string file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + endpoint + "_data.json";

    const int time_since_cache = ml::get_seconds_since_file_modified(file_path);
    if (time_since_cache > 86400 || time_since_cache == -1)
    {
        data = ml::json_get_from_url("https://dbd.tricky.lol/api/" + endpoint);

        if (data.empty())
            return false;

        ml::json_write_data(file_path, data);
    }
    else
        data = ml::json_get_data_from_file(file_path);

    return true;
}

template <typename T>
void perk_packager::load_instance_data(std::vector<T>* vec_obj, const nlohmann::json& data)
{
    const std::vector<std::string> variables = {"owner", "name", "id", "role", "image"};

    for (const auto& entry : data)
    {
        T obj;

        for (const auto& variable : variables)
        {
            if (!entry.contains(variable) || entry[variable].is_null())
                continue;

            if (variable == "owner")
                obj.owner = entry[variable];
            
            else if (variable == "name")
                obj.name = entry[variable];

            else if (variable == "id")
                obj.id = entry[variable];

            else if (variable == "role")
                obj.role = entry[variable];

            else if (variable == "image")
                obj.game_file_path = entry[variable];
        }

        vec_obj->push_back(obj);
    }
}

template <typename T>
void perk_packager::clear_instance_images(std::vector<T>& vec_obj)
{
    for (auto& obj : vec_obj)
    {
        obj.local_image_path = "";
        obj.game_file_path = "";
        obj.image = 0;
        obj.has_selected_image = false;
    }
}

template <typename T>
void perk_packager::reload_package_data(std::vector<T>& vec_obj)
{
    for (auto& entry : vec_obj)
    {
        auto it = _internal::package_data.find(entry.name);
        if (it == _internal::package_data.end())
            continue;

        if (it.value().contains("name"))
            entry.name = it.value()["name"];

        if (it.value().contains("role"))
            entry.role = it.value()["role"];

        if (it.value().contains("game_file_path"))
            entry.game_file_path = it.value()["game_file_path"];

        if (it.value().contains("local_file_path"))
        {
            entry.has_selected_image = true;
            entry.local_image_path = it.value()["local_file_path"];
            images::load_texture_from_file(entry.local_image_path, &entry.image);
        }
    }
}
