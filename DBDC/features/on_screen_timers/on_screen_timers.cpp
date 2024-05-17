#include "on_screen_timers.hpp"
#include "GUI/GUI.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_stdlib.h"
#include <conio.h>
#include "backend/backend.hpp"
#include "overlay/overlay.hpp"
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

    data["timers"].erase(index);

    ml::json_write_data(file_path, data);
}

void onscreen_timers::detect_keypress(const int& key)
{
    const size_t all_timers_size = all_timers.size();
    for (int j = 0; j < all_timers_size; j++)
    {
        if (j > all_timers.size())
            continue;

        timer t = all_timers[j];
        if (key != t.hotkey)
            continue;

        t.start_time = std::chrono::steady_clock::now();
        t.end_time = t.start_time + std::chrono::seconds(t.duration);

        active_timers.push_back(t);
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

        if (seconds_with_one_decimal < 0)
        {
            seconds_with_one_decimal = 0;
        }

        std::string timer_string = std::string(t.name) + ": " + std::to_string(seconds_with_one_decimal);

        size_t pos = timer_string.find('.');
        if (pos != std::string::npos && timer_string.length() > pos + 2)
            timer_string = timer_string.substr(0, pos + 2);

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x_render_location, y_render_location - (20 * i)), t.text_color.to_imcolor(), timer_string.c_str());

        if (t.end_time < time_now)
            active_timers.erase(active_timers.begin() + i);
    }

    if (show_temp_timer && active_timers.empty())
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x_render_location, y_render_location), ImColor(255, 255, 255), "Timer: 15.0");
}

bool onscreen_timers::save_timer_profile()
{
    nlohmann::json data;

    data["render_x_location"] = x_render_location;
    data["render_y_location"] = y_render_location;

    const size_t timers_size = all_timers.size();
    for (int i = 0; i < timers_size; i++)
    {
        timer& t = all_timers[i];

        data["timers"][i]["name"] = t.name;
        data["timers"][i]["duration"] = t.duration;
        data["timers"][i]["hotkey"] = t.hotkey;

        for (int j = 0; j < 3; j++)
            data["timers"][i]["color"][j] = t.text_color[j];
    }

    return ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "timer_profile.json", data);
}

void onscreen_timers::load_timer_profile()
{
    nlohmann::json data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "timer_profile.json");

    if (data.contains("render_x_location"))
        onscreen_timers::x_render_location = data["render_x_location"];

    if (data.contains("render_y_location"))
        onscreen_timers::y_render_location = data["render_y_location"];
    else
        y_render_location = backend::screen_height / 2.75;


    for (const auto& t : data["timers"])
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
    gui::begin_group_box("onscreen_timer_main_groupbox", ImVec2(250, 0));

    ImGui::SeparatorText("Main");

    if (ImGui::Checkbox("Enable", &onscreen_timers::enabled))
    {
        if (onscreen_timers::enabled)
        {
            if (!overlay::is_overlay_active())
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

    if (ImGui::Button("Disable Active Timers", {184.F, 0.F}))
        active_timers.clear();
    gui::tool_tip("Will Disable Any Currently Running Timers");

    if (ImGui::SliderInt("Timer X Location", &x_render_location, 0, backend::screen_width))
        save_timer_profile();

    if (ImGui::SliderInt("Timer Y Location", &y_render_location, 0, backend::screen_height))
        save_timer_profile();

    show_temp_timer = (ImGui::GetID("Timer X Location") == ImGui::GetActiveID() || ImGui::GetID("Timer Y Location") == ImGui::GetActiveID());


    gui::end_group_box();

    ImGui::SameLine();

    gui::begin_group_box("onscreen_timer_creation_group_box", {275.F, 0});

    ImGui::SeparatorText("Timer Creation");

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
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 1, cursor_pos.y + 7));

        if (gui::image_button(std::string("##delete_timer_button" + std::to_string(i)).c_str(), menu::icons::x_icon, {13, 13}))
        {
            delete_timer(i);
            break;
        }
        gui::tool_tip("Deletes the timer");

        for (int j = 0; j < 3; j++)
            ImGui::Spacing();
    }

    if (all_timers.size() < 11)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 6));
        if (ImGui::Button("Add New Timer", ImVec2(260.F, 0.F)))
            add_new_timer();
    }

    gui::end_group_box();
}
