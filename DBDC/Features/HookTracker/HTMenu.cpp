#include "HTMenu.h"
#include "ImGui/imgui.h"
#include "Menu/Menu.hpp"
#include "Features/Crosshair/Crosshair.h"
#include "HookTracker.hpp"
#include "Features/ConfigEditor/ConfigEditor.hpp"
#include "Overlay/Overlay.hpp"

void ht_menu::setup()
{
    config_editor::initialize_config();
    hook_tracker::setup();
}

void ht_menu::render_ui()
{
    ImGui::SetCursorPosY(45);

    if (ImGui::Checkbox("Enabled", &hook_tracker::ht_vars::enabled))
    {
        if (hook_tracker::ht_vars::enabled)
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
            for (int i = 0; i < hook_tracker::all_survivors.size(); i++)
                hook_tracker::all_survivors[i].hook_stage = 0;
            
            if (!overlay::is_overlay_needed())
                overlay::destroy_overlay();
        }
    }

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
}
