#include "OnScreenTimers.hpp"
#include <Windows.h>

#include "../GUI/GUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_stdlib.h"
#include <conio.h>

void onscreen_timers::add_new_timer()
{
    timer t;
    t.name = "";
    t.duration = 10;
    t.hotkey = VK_F1;

    all_timers.push_back(t);
}

void onscreen_timers::delete_timer(const int& index)
{
    all_timers.erase(all_timers.begin() + index);

    const std::string file_path = backend::exe_directory.string() + backend::settings_directory + "\\data\\timer_profile.json";

    nlohmann::json data = ml::json_get_data_from_file(file_path);

    data.erase(index);

    ml::json_write_data(file_path, data);
}

void onscreen_timers::keypress_loop()
{
    for (int i = 0; i < 256; ++i)
    {
        if (!(GetAsyncKeyState(i) & 1))
            continue;

        for (int j = 0; j < all_timers.size(); j++)
        {
            if (j > all_timers.size())
                continue;

            timer t = all_timers[j];
            if (i != t.hotkey)
                continue;

            t.start_time = std::chrono::steady_clock::now();
            t.end_time = t.start_time + std::chrono::seconds(t.duration);

            active_timers.push_back(t);
        }
    }
}

void onscreen_timers::render_timers()
{
    for (int i = 0; i < active_timers.size(); i++)
    {
        const timer t = active_timers[i];
        const auto time_now = std::chrono::steady_clock::now();
        const std::string duration_to_seconds = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(t.end_time - time_now).count());
        const std::string timer_string = std::string(t.name) + ": " + duration_to_seconds;

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(40, (backend::screen_height / 2.75) - (20 * i)), ImColor(255, 255, 255), timer_string.c_str());

        if (t.end_time < time_now)
            active_timers.erase(active_timers.begin() + i);
    }
}

bool onscreen_timers::save_timer_profile()
{
    nlohmann::json data;

    const size_t timers_size = all_timers.size();
    for (int i = 0; i < timers_size; i++)
    {
        timer& t = all_timers[i];

        data[i]["name"] = t.name;
        data[i]["duration"] = t.duration;
        data[i]["hotkey"] = t.hotkey;
    }

    return ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + "\\data\\timer_profile.json", data);
}

void onscreen_timers::load_timer_profile()
{
    nlohmann::json data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + "\\data\\timer_profile.json");

    for (const auto& t : data)
    {
        timer _timer;

        _timer.name = t["name"];
        _timer.duration = t["duration"];
        _timer.hotkey = t["hotkey"];

        all_timers.push_back(_timer);
    }
}

void onscreen_timers::render_ui()
{
    ImGui::SetCursorPosY(45.F);
    gui::begin_group_box("onscreen_timer_groupbox", ImVec2(275, 0));
    ImGui::SeparatorText("Timer Creation");
    gui::tool_tip("Allows you to setup hotkeys to display timers on your screen for relevant information");

    ImGui::SetCursorPosY(30.F);
    if (ImGui::Checkbox("Enable", &onscreen_timers::enabled))
    {
        if (onscreen_timers::enabled)
        {
            if (!menu::overlay::is_overlay_created())
            {
                menu::overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }
        }
        else
        {
            if (!menu::overlay::is_overlay_needed())
                menu::overlay::destroy_overlay();
        }
    }

    const size_t timers_size = all_timers.size();
    for (int i = 0; i < timers_size; i++)
    {
        timer& t = all_timers[i];

        const std::string temp_label = "##";

        const std::string hotkey_label = temp_label + "Timer" + std::to_string(i);
        if (ImGui::Hotkey(hotkey_label.c_str(), &t.hotkey, ImVec2(30, 0)))
            save_timer_profile();

        ImGui::SameLine();

        const std::string text_box_label = temp_label + "text_box" + std::to_string(i);
        const std::string duration_box_label = temp_label + "duration_box" + std::to_string(i);

        ImGui::SetNextItemWidth(160.F);

        if (ImGui::InputTextWithHint(text_box_label.c_str(), "Input Timer Name...", &t.name))
            save_timer_profile();
        
        ImGui::SameLine();
        ImGui::SetNextItemWidth(40.F);
        if (ImGui::InputInt(duration_box_label.c_str(), &t.duration, NULL, NULL))
            save_timer_profile();

        ImGui::SameLine();
        const auto cursor_pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 4, cursor_pos.y + 1));
        ImGui::Text("X");
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 5, cursor_pos.y + 4));
        const std::string button_label = "delete_button" + std::to_string(i);

        if (ImGui::InvisibleButton(button_label.c_str(), ImVec2(14, 20)))
        {
            delete_timer(i);
            break;
        }
        ImGui::Spacing();
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 6));
    if (ImGui::Button("Add New Timer", ImVec2(260.F, 0.F)) && all_timers.size() < 12)
        add_new_timer();

    gui::end_group_box();
}
