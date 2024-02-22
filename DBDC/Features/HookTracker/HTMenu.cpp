#include "HTMenu.h"
#include <ranges>
#include "ImGui/imgui.h"
#include "../Menu.h"
#include "../Crosshair/Crosshair.h"
#include "../GUI/GUI.h"
#include "HookTracker.hpp"
#include "../ConfigEditor/ConfigEditor.hpp"

void HTMenu::Setup()
{
    config_editor::initialize_config();
}

void HTMenu::RenderUI()
{
    ImGui::SetCursorPosY(45);
}
