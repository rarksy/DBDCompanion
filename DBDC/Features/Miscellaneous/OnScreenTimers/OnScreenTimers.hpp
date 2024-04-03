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
        {"Unhook Endurance (10s)", 10},
        {"Exhausted (40s)", 40},
        {"Exhausted (50s)", 50},
        {"Exhausted (60s)", 60},
        {"Decisive Strike (60s)", 60},
        {"Off The Record (80s)", 80},
    };
}