﻿#include "PPMenu.h"

#include "../GUI/GUI.h"
#include "ImGui/imgui.h"

void PPMenu::Setup()
{
    
}

void PPMenu::RenderUI()
{
    if (!isSetup)
    {
        ImGui::Text("Loading Data...");

        return;
    }
}
