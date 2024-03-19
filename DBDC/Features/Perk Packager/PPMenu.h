#pragma once
#include <vector>
#include <string>


namespace pp_menu
{
    inline bool isSetup = false;
    
    void setup();
    void render_ui();

    namespace _internal
    {
        inline int perk_filter = 0;
        inline static char searched_perk[16];

        inline std::vector<std::string> all_filters = {"All", "Killer", "Survivor"};
    }
    
}
