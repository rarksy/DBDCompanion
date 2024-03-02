﻿#pragma once
#include <vector>
#include "nlohmann/json.hpp"
#include <GLFW/glfw3.h>

namespace shrine_of_secrets
{
    struct perk
    {
        std::string id;
        std::string name;
        std::string description;
        std::string image_path;
    };

    inline nlohmann::json shrine_data;
    inline std::vector<perk> all_perks;
    
    inline unsigned int reset_time_end;

    inline bool unavailable = false;
    inline bool is_ready = false;

    bool is_cache_valid();
    void cache();
    bool load_cache();
    
    void init();
    perk get_perk(const int& perk_index);
    
    void render_ui();
}
