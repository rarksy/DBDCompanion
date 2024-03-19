#pragma once

#include "miscLIB/miscLIB.hpp"

namespace perk_packager
{
    void setup();

    struct perk
    {
        std::string name;
        std::string id;
        std::string role;
        mutable unsigned image = -1;

        mutable bool has_selected_image = false;
    };

    struct character
    {
        std::string name;
        std::string id;
        std::string role;
        std::vector<perk> perks;
    };

    inline std::vector<character> all_characters;

    namespace _internal
    {
        inline nlohmann::json all_characters_data;
        inline nlohmann::json all_perks_data;
        
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
