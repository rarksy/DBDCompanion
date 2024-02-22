#pragma once
#include <vector>

#include "ImGui/imgui.h"
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>

#include "../../Backend/Backend.hpp"
#include "../ConfigEditor/ConfigEditor.hpp"

// use math to get portrait x & y 
/*
    create hardcoded value for portrait locations ONLY @ 100% UI scale FOR EACH resolution
    Y found = subtract the difference of (100 - UI scale) from the coordinate
    X found = hardcode (hate it) 
*/

// create a scanning region from x, y -> desired w, h
// scan for hook icon
// if hook icon IS NOT found and red bar IS NOT found: no hook detected
// if hook icon found but red bar IS NOT half way: 1st hook is detected
// if hook icon found AND red bar IS half way: 2nd hook detected
// profit

namespace HookTracker
{
    namespace _internal
    {
        struct vec2
        {
            int x;
            int y;

            vec2(int _x, int _y)
            {
                this->x = _x;
                this->y = _y;
            }

            ImVec2 to_imvec2()
            {
                return {static_cast<float>(this->x), static_cast<float>(this->y)};
            }

            vec2 operator+(const vec2& other) const
            {
                return vec2(x + other.x, y + other.y);
            }
        };

        inline std::vector<vec2> survivor_regions_1440{{120, 580}, {120, 700}, {120, 820}, {120, 940}};
    }

    inline config_editor::setting in_game_ui_scale(config_editor::files::game_user_settings, config_editor::sections::dbd_game_user_settings, "HudScaleFactor", 100);

    struct survivor
    {
        int index;
        _internal::vec2 location = {0, 0};
        _internal::vec2 size = {0, 0};

        survivor(){}
    };

    inline std::vector<survivor> all_survivors;
}
