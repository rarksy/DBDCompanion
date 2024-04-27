#include <chrono>
#include <Windows.h>
#include "HookTracker.hpp"
#include "Backend/Backend.hpp"
#include "miscLIB/miscLIB.hpp"
#include "nlohmann/json.hpp"

double calculatePercentage(double X, double P)
{
    return X * (P / 100.0);
}

void hook_tracker::setup()
{
    ht_vars::in_game_ui_scale.load_value();

    std::vector<_internal::vec2> render_locations;
    switch (ht_vars::in_game_ui_scale.value)
    {
    case 100:
        render_locations = _internal::survivor_render_regions_1440_100;
        break;
    case 95:
        render_locations = _internal::survivor_render_regions_1440_95;
        break;
    case 90:
        render_locations = _internal::survivor_render_regions_1440_90;
        break;
    case 85:
        render_locations = _internal::survivor_render_regions_1440_85;
        break;
    case 80:
        render_locations = _internal::survivor_render_regions_1440_80;
        break;
    case 75:
        render_locations = _internal::survivor_render_regions_1440_75;
        break;
    case 70:
        render_locations = _internal::survivor_render_regions_1440_70;
        break;
    }

    nlohmann::json profile_data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "hook_tracker_profile.json");
    bool has_profile = !profile_data.empty();
    
    for (int i = 0; i < 4; i++)
    {
        survivor s;
        
        s.location = render_locations[i];

        if (has_profile)
            s.hotkey = profile_data[i]["hotkey"];

        all_survivors.push_back(s);
        
    }
}

void hook_tracker::free()
{
    all_survivors.clear();
}


void hook_tracker::keypress_loop()
{
    for (int i = 0; i < 256; ++i)
    {
        if (!(GetAsyncKeyState(i) & 1))
            continue;

        for (int j = 0; j < all_survivors.size(); j++)
        {
            if (j > all_survivors.size())
                continue;

            survivor& s = all_survivors[j];
            if (i != s.hotkey)
                continue;

            s.hook_stage == 2 ? s.hook_stage = 0 : s.hook_stage++;
        }
    }
}

void hook_tracker::render()
{
    for (int i = 0; i < 4; i++)
    {
        const auto surv = hook_tracker::all_survivors[i];
        auto draw_list = ImGui::GetBackgroundDrawList();

        if (surv.hook_stage > 0 && surv.hook_stage != 3)
            draw_list->AddText(ImVec2(surv.location.x, surv.location.y), ImColor(255, 255, 255), std::to_string(surv.hook_stage).c_str());
    }
}

bool hook_tracker::save()
{
    nlohmann::json data;
    
    for (int i = 0; i < 4; i++)
    {
        survivor& s = all_survivors[i];
        
        data[i]["hotkey"] = s.hotkey;
    }

    return ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "hook_tracker_profile.json", data);
}
