#pragma once
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
        double duration;
        int hotkey;
        int drop_down_index = 0;

        double start_time = 0;
        double end_time = 0;
    };

    inline int t_k = 0;
    
    inline std::vector<timer> all_timers;
    inline std::vector<timer> active_timers;
    inline std::vector<std::pair<std::string, double>> all_timer_options = {
        {"Unhook Endurance", 10.0},
        {"Decisive Strike", 60.0},
        {"Off The Record", 80.0},
    };
}