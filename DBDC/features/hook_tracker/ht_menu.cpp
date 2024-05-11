#include "ht_menu.h"
#include "ImGui/imgui.h"
#include "menu/menu.hpp"
#include "features/crosshair/crosshair.h"
#include "hook_tracker.hpp"
#include "..\config_editor\config_editor.hpp"
#include "GUI/GUI.h"
#include "nlohmann/json.hpp"
#include "overlay/overlay.hpp"

void ht_menu::setup()
{
    config_editor::initialize_config();

    hook_tracker::ht_vars::in_game_ui_scale.load_value();

    hook_tracker::setup();
}

void ht_menu::render_ui()
{
    ImGui::SetCursorPosY(45.F);
    gui::begin_group_box("hook_tracker_groupbox", ImVec2(275, 0));
    ImGui::SetCursorPosY(2.F);
    ImGui::SeparatorText("Main");

    if (ImGui::Checkbox("Enabled", &hook_tracker::ht_vars::enabled))
    {
        if (hook_tracker::ht_vars::enabled)
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
            for (int i = 0; i < hook_tracker::all_survivors.size(); i++)
                hook_tracker::all_survivors[i].hook_stage = 0;

            if (!overlay::is_overlay_needed())
                overlay::destroy_overlay();
        }
    }

    if (gui::drop_down_box("In-Game UI Scale", ui_scale_index, UIScales, true, 40))
        hook_tracker::setup();

    ImGui::SeparatorText("Keybinds");


    const size_t survivors_size = hook_tracker::all_survivors.size();
    for (int i = 0; i < survivors_size; i++)
    {
        hook_tracker::survivor& s = hook_tracker::all_survivors[i];

        const std::string temp_label = "##";
        const std::string index = std::to_string(i + 1);
        const std::string hotkey_label = temp_label + "hotkey" + index;
        if (ImGui::Hotkey(hotkey_label.c_str(), &s.hotkey))
            hook_tracker::save();

        ImGui::SameLine();
        ImGui::Text("%s", ("Survivor " + index).c_str());
    }

    gui::end_group_box();
}
