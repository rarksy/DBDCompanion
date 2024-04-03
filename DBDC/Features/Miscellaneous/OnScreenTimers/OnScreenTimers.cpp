#include "OnScreenTimers.hpp"
#include <Windows.h>

#include "../../GUI/GUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include <conio.h>

void onscreen_timers::add_new_timer()
{
    timer t;
    t.name = all_timer_options[0].first;
    t.duration = all_timer_options[0].second;
    t.hotkey = VK_F1;

    all_timers.push_back(t);
}

void onscreen_timers::delete_timer(const int& index)
{
    all_timers.erase(all_timers.begin() + index);
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
        const std::string timer_string = t.name + ": " + duration_to_seconds;
    
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(40, (backend::screen_height / 2.75) - (20 * i)), ImColor(255, 255, 255), timer_string.c_str());
    
        if (t.end_time < time_now)
            active_timers.erase(active_timers.begin() + i);
    }
}

void onscreen_timers::render_ui()
{
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
        ImGui::Hotkey(hotkey_label.c_str(), &t.hotkey, ImVec2(30, 0));
        
        ImGui::SameLine();

        const std::string drop_down_label = temp_label + "DropDown" + std::to_string(i);

        ImGui::SetNextItemWidth(200.F);

        bool item_selected = false;

        if (ImGui::BeginCombo(drop_down_label.c_str(), all_timer_options[t.drop_down_index].first.c_str(), ImGuiComboFlags_NoArrowButton))
        {
            const size_t timer_options_size = all_timer_options.size();
            for (int n = 0; n < timer_options_size; n++)
            {
                const bool is_selected = (t.drop_down_index == n);
                if (ImGui::Selectable(all_timer_options[n].first.c_str(), is_selected))
                {
                    t.drop_down_index = n;
        
                    const auto option = all_timer_options[t.drop_down_index];
        
                    t.name = option.first;
                    t.duration = option.second;
                    item_selected = true;
                }
        
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
        ImGui::SameLine();
        const auto cursor_pos = ImGui::GetCursorPos();
        ImGui::Text("X");
        ImGui::SetCursorPos(ImVec2(cursor_pos.x - 1, cursor_pos.y + 3));
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
    
}
