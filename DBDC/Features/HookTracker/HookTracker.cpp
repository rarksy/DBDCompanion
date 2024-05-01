﻿#include <chrono>
#include <Windows.h>
#include "HookTracker.hpp"

#include "HTMenu.h"
#include "Backend/Backend.hpp"
#include "Menu/Menu.hpp"
#include "miscLIB/miscLIB.hpp"
#include "nlohmann/json.hpp"

double calculatePercentage(double X, double P)
{
    return X * (P / 100.0);
}

void hook_tracker::setup()
{
    if (backend::screen_height != 1080 && backend::screen_height != 1440)
    {
        MessageBoxA(nullptr, "you're not supposed to be able to get to this screen, you have an unsupported resolution", "?", MB_OK);

        menu::menu_to_show = 0;
        return;
    }
    all_survivors.clear();

    const auto it = std::find(ht_menu::UIScales.begin(), ht_menu::UIScales.end(), std::to_string(ht_vars::in_game_ui_scale.value));
    if (it != ht_menu::UIScales.end() && ht_menu::ui_scale_index == -1)
        ht_menu::ui_scale_index = it - ht_menu::UIScales.begin();

    const int ui_scale_to_int = std::atoi(ht_menu::UIScales[ht_menu::ui_scale_index].c_str());
    std::vector<_internal::vec2> render_locations = _internal::survivor_render_regions[backend::screen_height][ui_scale_to_int];


    nlohmann::json profile_data =
        ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "hook_tracker_profile.json");
    bool has_profile = !profile_data.empty();

    for (int i = 0; i < 4; i++)
    {
        int x = render_locations.size();

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


void hook_tracker::detect_keypress(const int& key)
{
    for (int j = 0; j < all_survivors.size(); j++)
    {
        if (j > all_survivors.size())
            continue;

        survivor& s = all_survivors[j];
        if (key != s.hotkey)
            continue;

        s.hook_stage == 2 ? s.hook_stage = 0 : s.hook_stage++;
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
