#pragma once
#include <string>
#include <vector>


namespace ht_menu
{
    void setup();
    void render_ui();

    inline int ui_scale_index = -1;
    inline std::vector<std::string> UIScales = {"70", "75", "80", "85", "90", "95", "100"};
}