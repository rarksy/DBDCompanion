#pragma once

#include "miscLIB/miscLIB.hpp"

namespace perk_packager
{
    void setup();

    struct Character
    {
        std::string name;
        std::string id;
        std::string role;
        std::vector<std::string> all_perks;
    };
    inline std::vector<Character> all_characters;

    namespace _internal
    {
        inline nlohmann::json all_characters_data;
        inline nlohmann::json all_perks_data;
    }
}
