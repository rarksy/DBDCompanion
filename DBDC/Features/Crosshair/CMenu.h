#pragma once
#include <vector>
#include <string>

namespace CMenu
{
    void Setup();
    void RenderUI();

    inline std::vector<std::string> dynamicKillers = {"Huntress", "Deathslinger", "Trickster", "Clown", "Unknown"};
}
