#pragma once
#include <chrono>
#include <vector>
#include <string>

#include "color/color.hpp"

namespace onscreen_timers
{
    void add_new_timer();
    void delete_timer(const int& index);
    void detect_keypress(const int& key);
    void render_timers();

    bool save_timer_profile();
    void load_timer_profile();
    
    void render_ui();

    struct timer
    {
        std::string name;
        int duration;
        int hotkey;
        color text_color = color(255, 255, 255);

        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point end_time;
    };

    inline bool enabled;
    inline bool show_temp_timer = false;
    
    inline int x_render_location = 40;
    inline int y_render_location = 0;
    
    inline std::vector<timer> all_timers;
    inline std::vector<timer> active_timers;
}
