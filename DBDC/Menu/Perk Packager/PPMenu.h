#pragma once
#include <string>
#include <vector>

// perk packager:
//
// filtering / listing:
// get all characters as json
// character.name = json.name
// character.perk1 = json.perks.0
// character.perk2 = json.perks.1
// character.perk3 = json.perks.2

namespace PPMenu
{
    void Setup();
    void RenderUI();

    struct Character
    {
        std::string name;
        std::vector<std::string> perks;

        Character(std::string _name, std::vector<std::string> _perks) : name(_name), perks(_perks)
        {
        }
    };
    
}
