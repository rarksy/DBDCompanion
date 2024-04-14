#include "IconPackager.hpp"
#include "IPMenu.hpp"
#include "../../Backend/Backend.hpp"
#include "Images/Images.h"

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
    for (int i = 0; i < endpoints_size; i++)
    {
        const auto& pair = endpoints_data[i];
        _internal::unavailable = !get_endpoint_data(pair.first, pair.second);

        if (_internal::unavailable)
            return false;

        if (pair.first == "offerings")
            load_instance_data(&all_offerings, pair.second);
        else if (pair.first == "addons")
            load_instance_data(&all_addons, pair.second);
        else if (pair.first == "items")
            load_instance_data(&all_items, pair.second);
        else if (pair.first == "perks")
        {
            _internal::unavailable = !get_endpoint_data("characters", _internal::all_characters_data);
            _internal::unavailable = !get_endpoint_data("perks", _internal::all_perks_data);

            for (const auto& character : _internal::all_characters_data)
            {
                if (character["name"].is_null() || character["role"].is_null() || character["image"].is_null())
                    continue;

                portrait po;
                po.name = character["name"];
                po.game_file_path = character["image"];
                po.role = character["role"];

                all_portraits.push_back(po);

                for (int j = 0; j < 3; j++)
                {
                    const auto perk_info = _internal::all_perks_data[character["perks"][j]];
                    perk p;
                    p.owner = character["name"];
                    p.name = perk_info["name"];
                    p.id = character["perks"][j];
                    p.game_file_path = perk_info["image"];
                    p.role = perk_info["role"];

                    all_perks.push_back(p);
                }
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
        }
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
    for (const auto& entry : data)
    {
        if (entry["name"].is_null() || entry["role"].is_null() || entry["image"].is_null())
            continue;

        T obj;

        if (entry.contains("name"))
            obj.name = entry["name"];

        if (entry.contains("id"))
            obj.id = entry["id"];

        if (entry.contains("role"))
            obj.role = entry["role"];

        if (entry.contains("image"))
            obj.game_file_path = entry["image"];

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
