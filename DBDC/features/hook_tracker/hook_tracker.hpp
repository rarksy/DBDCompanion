#pragma once
#include <map>
#include <vector>

#include "ImGui/imgui.h"
#include "..\config_editor\config_editor.hpp"
#include "color/color.hpp"

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
        
        inline std::map<int, std::map<int, std::vector<vec2>>> survivor_render_regions = {
            {
                1440, {
                    {100, {{312, 594}, {312, 712}, {312, 805}, {312, 923}}},
                    {95, {{297, 637}, {297, 749}, {297, 838}, {297, 950}}},
                    {90, {{281, 679}, {281, 785}, {225, 891}, {225, 997}}},
                    {85, {{266, 721}, {266, 821}, {266, 921}, {266, 1021}}},
                    {80, {{250, 764}, {250, 858}, {250, 951}, {250, 1047}}},
                    {75, {{235, 806}, {235, 894}, {235, 982}, {235, 1070}}},
                    {70, {{219, 849}, {219, 931}, {219, 1013}, {219, 1096}}}
                }
            },
            {
                1080, {
                    {100, {{234, 445}, {234, 514}, {234, 603}, {234, 709}}},
                    {95, {{222, 477}, {222, 561}, {222, 645}, {222, 729}}},
                    {90, {{210, 508}, {210, 588}, {210, 667}, {210, 746}}},
                    {85, {{198, 539}, {198, 614}, {198, 689}, {198, 764}}},
                    {80, {{186, 571}, {186, 642}, {186, 712}, {186, 783}}},
                    {75, {{175, 604}, {175, 670}, {175, 736}, {175, 802}}},
                    {70, {{163, 635}, {163, 696}, {163, 758}, {163, 820}}}
                }
            }
        };
    }
    


    struct survivor
    {
        _internal::vec2 location = {0, 0};
        int hook_stage = 0; // 1 = 1st stage | 2 = 2nd stage | 3 = dead
        int hotkey = 0;


        survivor()
        {
        }
    };

    void setup();
    void free();

    void detect_keypress(const int& key);
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
