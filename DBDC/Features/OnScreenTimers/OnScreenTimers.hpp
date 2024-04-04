#pragma once
#include <chrono>
#include <vector>
#include <string>

namespace onscreen_timers
{
    void add_new_timer();
    void delete_timer(const int& index);
    void keypress_loop();
    void render_timers();

    bool save_timer_profile();
    void load_timer_profile();
    
    void render_ui();

    struct timer
    {
        std::string name;
        int duration;
        int hotkey;

        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point end_time;
    };

    inline bool enabled;
    
    inline std::vector<timer> all_timers;
    inline std::vector<timer> active_timers;
}