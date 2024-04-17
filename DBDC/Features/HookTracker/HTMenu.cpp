#include "HTMenu.h"
#include "ImGui/imgui.h"
#include "..\..\Menu\Menu.hpp"
#include "../Crosshair/Crosshair.h"
#include "HookTracker.hpp"
#include "../ConfigEditor/ConfigEditor.hpp"

void ht_menu::setup()
{
    config_editor::initialize_config();
}

void ht_menu::render_ui()
{
    ImGui::SetCursorPosY(45);

    if (ImGui::Checkbox("Enabled", &hook_tracker::ht_vars::enabled))
    {
        if (hook_tracker::ht_vars::enabled)
        {
            if (!menu::overlay::is_overlay_created())
            {
                menu::overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }

            hook_tracker::setup();

            std::thread detection_thread(hook_tracker::detection_loop);
            detection_thread.detach();
        }
        else
        {
            hook_tracker::free();

            if (!cvars.enabled)
                menu::overlay::destroy_overlay();
        }
    }
}
