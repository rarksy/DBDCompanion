#pragma once
#include <vector>
#include <string>

namespace CMenu
{
    void Setup();
    void RenderUI();


    //idea for this:
    // hillbilly curve crosshair; horizontal lines either side of actual crosshair that extend depending on mouse movement
    inline std::vector<std::string> dynamicKillers = {"Huntress", "Deathslinger", "Trickster"};
}
