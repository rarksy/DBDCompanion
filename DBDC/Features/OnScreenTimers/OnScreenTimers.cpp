#include "OnScreenTimers.hpp"
#include "GUI/GUI.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_stdlib.h"
#include <conio.h>
#include "Backend/Backend.hpp"
#include "Overlay/Overlay.hpp"
#include "miscLIB/miscLIB.hpp"

void onscreen_timers::add_new_timer()
{
    timer t;
    t.name = "";
    t.duration = 10;
    t.hotkey = VK_F1;

    all_timers.push_back(t);

    save_timer_profile();
}

void onscreen_timers::delete_timer(const int& index)
{
    all_timers.erase(all_timers.begin() + index);

    const std::string file_path = backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "timer_profile.json";

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
    const auto time_now = std::chrono::steady_clock::now();

    for (int i = 0; i < active_timers.size(); i++)
    {
        const timer& t = active_timers[i];
        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t.end_time - time_now);
        double seconds_with_one_decimal = duration.count() / 1000.0;

        if (seconds_with_one_decimal < 0) {
            seconds_with_one_decimal = 0;
        }

        std::string timer_string = std::string(t.name) + ": " + std::to_string(seconds_with_one_decimal);
        
        size_t pos = timer_string.find('.');
        if (pos != std::string::npos && timer_string.length() > pos + 2) 
            timer_string = timer_string.substr(0, pos + 2);

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(40, (backend::screen_height / 2.75) - (20 * i)), t.text_color.to_imcolor(), timer_string.c_str());

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

        for (int j = 0; j < 3; j++)
            data[i]["color"][j] = t.text_color[j];
        
    }

    return ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "timer_profile.json", data);
}

void onscreen_timers::load_timer_profile()
{
    nlohmann::json data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "timer_profile.json");

    for (const auto& t : data)
    {
        timer _timer;

        _timer.name = t["name"];
        _timer.duration = t["duration"];
        _timer.hotkey = t["hotkey"];

        for (int j = 0; j < 3; j++)
            _timer.text_color[j] = t["color"][j];

        all_timers.push_back(_timer);
    }
}

void onscreen_timers::render_ui()
{
    ImGui::SetCursorPosY(45.F);
    gui::begin_group_box("onscreen_timer_groupbox", ImVec2(275, 0));
    ImGui::SetCursorPosY(2.F);
    ImGui::SeparatorText("Timer Creation");

    ImGui::SetCursorPosY(25.F);
    if (ImGui::Checkbox("Enable", &onscreen_timers::enabled))
    {
        if (onscreen_timers::enabled)
        {
            if (!overlay::is_overlay_created())
            {
                overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }
        }
        else
        {
            if (!overlay::is_overlay_needed())
                overlay::destroy_overlay();

            active_timers.clear();
        }
    }
    gui::tool_tip("Allows timers to be displayed\nNote: Disabling this will disable any currently running timers");

    const size_t timers_size = all_timers.size();
    for (int i = 0; i < timers_size; i++)
    {
        timer& t = all_timers[i];

        const std::string temp_label = "##";

        const std::string hotkey_label = temp_label + "Timer" + std::to_string(i);
        if (ImGui::Hotkey(hotkey_label.c_str(), &t.hotkey))
            save_timer_profile();
        gui::tool_tip("Sets the key used to activate the timer");


        ImGui::SameLine();

        const std::string label_index = std::to_string(i);
        const std::string text_box_label = temp_label + "text_box" + label_index;
        const std::string duration_box_label = temp_label + "duration_box" + label_index;
        const std::string color_picker_label = temp_label + "color_picker" + label_index;
        
        ImGui::SetNextItemWidth(120.F);
        if (ImGui::InputTextWithHint(text_box_label.c_str(), "Timer Name", &t.name))
            save_timer_profile();
        gui::tool_tip("Sets the name that will be displayed when the timer is activated");

        
        ImGui::SameLine();
        ImGui::SetNextItemWidth(40.F);
        if (ImGui::InputInt(duration_box_label.c_str(), &t.duration, NULL, NULL))
            save_timer_profile();
        gui::tool_tip("Controls the duration of the timer (numbers only)");
        ImGui::SameLine();
        
        if (gui::color_picker(color_picker_label.c_str(), &t.text_color))
            save_timer_profile();
        gui::tool_tip("Sets the color of the text used to display the timer");

        ImGui::SameLine();
        const auto cursor_pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 2, cursor_pos.y + 1));
        ImGui::Text("X");
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 3, cursor_pos.y + 4));
        const std::string button_label = "delete_button" + std::to_string(i);

        if (ImGui::InvisibleButton(button_label.c_str(), ImVec2(14, 20)))
        {
            delete_timer(i);
            break;
        }
        gui::tool_tip("Deletes the timer");

        ImGui::Spacing();
    }
    
    if (all_timers.size() < 12)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 6));
        if (ImGui::Button("Add New Timer", ImVec2(260.F, 0.F)) && all_timers.size() < 12)
            add_new_timer();   
    }

    gui::end_group_box();
}
