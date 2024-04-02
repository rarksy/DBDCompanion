#pragma once
#include <vector>
#include <string>


namespace pp_menu
{
    inline bool isSetup = false;
    
    void setup();
    void render_ui();

    void display_perks();
    void display_items();

    void reload_packages();

    namespace _internal
    {
        namespace package_selector
        {
            inline int loaded_package = -1;
            inline std::string loaded_package_name;
            inline std::vector<std::string> all_packages;
            inline static char input_package_name[16];

            inline const std::string package_directory = "Perk Packages\\";
            inline const std::string package_data_directory = "Perk Packages\\Data\\";
        }
        
        inline static char searched_text[16];

        inline int character_filter_index = 0;
        inline std::vector<std::string> character_filter = {"All", "Killer", "Survivor"};

        inline int type_filter_index = 0;
        inline std::vector<std::string> type_filter = {"All", "Perks", "Items"};
    }
    
}
