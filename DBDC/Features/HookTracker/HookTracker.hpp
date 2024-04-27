#pragma once
#include <vector>

#include "ImGui/imgui.h"
#include "../ConfigEditor/ConfigEditor.hpp"

namespace hook_tracker
{
    namespace _internal
    {
        struct vec2
        {
            double x;
            double y;

            vec2(int _x, int _y)
            {
                this->x = _x;
                this->y = _y;
            }

            ImVec2 to_imvec2() const
            {
                return {static_cast<float>(this->x), static_cast<float>(this->y)};
            }

            vec2 operator+(const vec2& other) const
            {
                return vec2(x + other.x, y + other.y);
            }
        };

        // time to do some hardcoding >:)
        inline std::vector<vec2> survivor_render_regions_1440_100{{250, 595}, {250, 705}, {250, 815}, {250, 925}};
        inline std::vector<vec2> survivor_render_regions_1440_95{{235, 635}, {235, 745}, {235, 855}, {235, 965}};
        inline std::vector<vec2> survivor_render_regions_1440_90{{225, 685}, {225, 785}, {225, 885}, {225, 985}};
        inline std::vector<vec2> survivor_render_regions_1440_85{{210, 720}, {210, 820}, {210, 920}, {210, 1020}};
        inline std::vector<vec2> survivor_render_regions_1440_80{{210, 720}, {210, 820}, {210, 920}, {210, 1020}};
        inline std::vector<vec2> survivor_render_regions_1440_75{{190, 805}, {190, 895}, {190, 985}, {190, 1075}};
        inline std::vector<vec2> survivor_render_regions_1440_70{{175, 845}, {175, 925}, {175, 1005}, {175, 1085}};
    }


    struct survivor
    {
        _internal::vec2 location = {0, 0};
        int hook_stage = 0; // 1 = 1st stage | 2 = 2nd stage | 3 = dead
        int hotkey = 0;
        

        survivor(){}
    };

    void setup();
    void free();

    void keypress_loop();
    void render();
    
    bool save();

    inline std::vector<survivor> all_survivors;


    namespace ht_vars
    {
        using namespace config_editor;
        
        inline bool enabled = false;
        
        inline setting in_game_ui_scale(files::game_user_settings, sections::dbd_game_user_settings, "HudScaleFactor", 100);

    }
}
