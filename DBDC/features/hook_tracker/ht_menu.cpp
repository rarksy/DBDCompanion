#include "ht_menu.h"
#include "ImGui/imgui.h"
#include "menu/menu.hpp"
#include "features/crosshair/crosshair.h"
#include "hook_tracker.hpp"
#include "..\config_editor\config_editor.hpp"
#include "GUI/GUI.h"
#include "miscLIB/miscLIB.hpp"
#include "nlohmann/json.hpp"
#include "overlay/overlay.hpp"

void ht_menu::setup()
{
    config_editor::initialize_config();

    hook_tracker::ht_vars::in_game_ui_scale.load_value();

    const auto color_data = ml::json_get_data_from_file(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "settings.json");

    if (color_data.contains("hook_tracker"))
    {
        stage_1_color.r = color_data["hook_tracker"]["stage_1_color"]["r"];
        stage_1_color.g = color_data["hook_tracker"]["stage_1_color"]["g"];
        stage_1_color.b = color_data["hook_tracker"]["stage_1_color"]["b"];
        stage_1_color.a = color_data["hook_tracker"]["stage_1_color"]["a"];

        stage_2_color.r = color_data["hook_tracker"]["stage_2_color"]["r"];
        stage_2_color.g = color_data["hook_tracker"]["stage_2_color"]["g"];
        stage_2_color.b = color_data["hook_tracker"]["stage_2_color"]["b"];
        stage_2_color.a = color_data["hook_tracker"]["stage_2_color"]["a"];
    }

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
    gui::tool_tip("Sets the position that tracked hooks will be shown at\n\nNote: This SHOULD be automatically set to what your in game value is set to.");

    const auto save_color = [](std::string setting, color col)
    {
        nlohmann::json data;

        data["hook_tracker"][setting]["r"] = stage_2_color.r;
        data["hook_tracker"][setting]["g"] = stage_2_color.g;
        data["hook_tracker"][setting]["b"] = stage_2_color.b;
        data["hook_tracker"][setting]["a"] = stage_2_color.a;

        ml::json_write_data(backend::exe_directory.string() + backend::settings_directory + backend::data_directory + "settings.json", data);
    };

    if (gui::color_picker("Stage 1 Color", &stage_1_color, true))
        save_color("stage_1_color", stage_1_color);
    gui::tool_tip("Sets the color that first stage hooks will be shown as.");
    
    if (gui::color_picker("Stage 2 Color", &stage_2_color, true))
        save_color("stage_2_color", stage_2_color);
    gui::tool_tip("Sets the color that second stage hooks will be shown as.");

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
