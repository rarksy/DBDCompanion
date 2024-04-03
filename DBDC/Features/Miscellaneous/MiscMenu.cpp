#include "MiscMenu.h"

#include "../GUI/GUI.h"
#include "ImGui/imgui.h"
#include "OnScreenTimers/OnScreenTimers.hpp"

void misc_menu::render_ui()
{
    ImGui::SetCursorPosY(45.F);
    gui::begin_group_box("group box", ImVec2(275, 0));
    ImGui::SeparatorText("On Screen Timers");
    gui::tool_tip("Allows you to setup hotkeys to display timers on your screen for relevant information");
    onscreen_timers::render_ui();

    gui::end_group_box();
}
