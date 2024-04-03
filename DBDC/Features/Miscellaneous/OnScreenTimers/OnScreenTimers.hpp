#pragma once
#include <chrono>
#include <vector>
#include <string>

namespace onscreen_timers
{
    void add_new_timer();
    void keypress_loop();
    void render_timers();
    
    void render_ui();

    struct timer
    {
        std::string name;
        int duration;
        int hotkey;
        int drop_down_index = 0;

        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point end_time;
    };

    inline bool enabled;
    
    inline std::vector<timer> all_timers;
    inline std::vector<timer*> active_timers;
    inline std::vector<std::pair<std::string, double>> all_timer_options = {
        {"Unhook Endurance", 10},
        {"Decisive Strike", 60},
        {"Off The Record", 80},
    };
}