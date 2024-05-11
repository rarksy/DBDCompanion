#pragma once
#include <vector>
#include <string>


namespace ip_menu
{
    inline bool isSetup = false;
    
    void setup();
    void render_ui();

    template<typename T>
    void display_base_item(std::vector<T>& vec_obj);

    void reload_packages();

    namespace _internal
    {
        namespace package_selector
        {
            inline int loaded_package = -1;
            inline std::string loaded_package_name;
            inline std::vector<std::string> all_packages;
            inline static std::string input_package_name;

            inline const std::string package_directory = "Icon Packages\\";
            inline const std::string package_data_directory = "Icon Packages\\Data\\";
        }
        
        inline static char searched_text[16];

        inline int role_filter_index = 0;
        inline std::vector<std::string> character_filter = {"Killer", "Survivor"};

        inline int type_filter_index = 0;
        inline std::vector<std::string> type_filter = {"Perks", "Powers", "Offerings", "Addons", "Portraits", "Status Effects"};
    }
    
    
}
