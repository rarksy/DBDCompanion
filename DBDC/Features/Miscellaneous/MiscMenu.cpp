#include "MiscMenu.h"

#include "ImGui/imgui.h"
#include "OnScreenTimers/OnScreenTimers.hpp"

void misc_menu::render_ui()
{
    ImGui::Columns(3, nullptr, false);
    ImGui::SeparatorText("On Screen Timers");
    onscreen_timers::render_ui();

    // click + button, new line appears with hotkey, and dropdown for associated timer
}
