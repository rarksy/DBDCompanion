#include "OnScreenTimers.hpp"
#include <Windows.h>

#include "../../GUI/GUI.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

void onscreen_timers::add_new_timer()
{
    timer t;
    t.name = all_timer_options[0].first;
    t.duration = all_timer_options[0].second;
    t.hotkey = VK_F1;

    all_timers.push_back(t);
}

void onscreen_timers::keypress_loop()
{
    const size_t size = all_timers.size();
    for (int i = 0; i < size; i++)
    {
        timer& t = all_timers[i];
        if (GetAsyncKeyState(t.hotkey))
        {
            //t.start_time = std::chrono::system_clock::now();
        }
            
    }
}

void onscreen_timers::render_timers()
{
    
}

void onscreen_timers::render_ui()
{
    const size_t timers_size = all_timers.size();
    
    for (int i = 0; i < timers_size; i++)
    {
        timer& t = all_timers[i];

        const std::string temp_label = "##";
        
        const std::string hotkey_label = temp_label + "Timer" + std::to_string(i);
        ImGui::Hotkey(hotkey_label.c_str(), &t.hotkey);
    
        ImGui::SameLine();
    
        const std::string drop_down_label = temp_label + "DropDown" + std::to_string(i);
    
        ImGui::SetNextItemWidth(170.F);
    
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
                    item_selected = true;
                }
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    
    if (ImGui::Button("Add New Timer", ImVec2(230.F, 0.F)))
        add_new_timer();
}
