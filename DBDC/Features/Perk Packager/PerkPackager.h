#pragma once

#include "miscLIB/miscLIB.hpp"

namespace perk_packager
{
    void setup();
    void clear_images();
    void reload();


    struct base
    {
        std::string name;
        std::string id;
        std::string role;

        unsigned image = -1;
        std::string local_image_path;
        std::string game_file_path;

        mutable bool has_selected_image = false;
    };

    struct perk : base
    {
    };

    struct item : base
    {
    };

    struct offering : base
    {
    };

    struct character
    {
        std::string name;
        std::string id;
        std::string role;
        std::vector<perk> perks;
    };

    inline std::vector<character> all_characters;
    inline std::vector<item> all_items;
    inline std::vector<offering> all_offerings;

    namespace _internal
    {
        inline bool unavailable = false;
        
        inline nlohmann::json all_characters_data;
        inline nlohmann::json all_perks_data;
        inline nlohmann::json all_items_data;
        inline nlohmann::json all_offerings_data;
        
        inline nlohmann::json package_data;
        
        inline std::vector<std::string> all_survivor_general_perks =
        {
            "Dark_Sense",
            "Deja_Vu",
            "Hope",
            "Kindred",
            "Lightweight",
            "No_One_Left_Behind",
            "Plunderers_Instinct",
            "Premonition",
            "Resilience",
            "Slippery_Meat",
            "Small_Game",
            "Spine_Chill",
            "This_Is_Not_Happening",
            "WellMakeIt",
        };

        inline std::vector<std::string> all_killer_general_perks =
        {
            "Bitter_Murmur",
            "Deerstalker",
            "Distressing",
            "No_One_Escapes_Death",
            "Hex_Thrill_Of_The_Hunt",
            "Insidious",
            "Iron_Grasp",
            "Monstrous_Shrine",
            "BoonDestroyer",
            "Sloppy_Butcher",
            "Spies_From_The_Shadows",
            "Unrelenting",
            "Whispers",
        };
    }
}
