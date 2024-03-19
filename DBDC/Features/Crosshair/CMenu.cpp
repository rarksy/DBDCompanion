#include "CMenu.h"
#include "Crosshair.h"
#include "../Menu.h"
#include "../GUI/GUI.h"
#include "..\HookTracker\HookTracker.hpp"
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

void CMenu::Setup()
{
    Crosshair::CheckProfileDirectory();
    Crosshair::ReloadProfiles();
}

void CMenu::RenderUI()
{
    ImGui::SetCursorPosY(45);

    if (ImGui::Checkbox("Enable", &CVars.enabled))
    {
        if (CVars.enabled)
        {
            if (!menu::overlay::is_overlay_created())
            {
                menu::overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }
        }

        else menu::overlay::destroy_overlay();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::BeginDisabled(!CVars.enabled);
    {
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnWidth(0, 225);
        ImGui::SetColumnWidth(1, 260);

        ImGui::SeparatorText("Lines");

        ImGui::CheckboxWithColorPicker("Lines", "Line Color", &CVars.enableLines, CVars.lineColor);
        ImGui::BeginDisabled(!CVars.enableLines);
        {
            if (ImGui::BeginCombo("##Lines", "Lines", ImGuiComboFlags_NoArrowButton))
            {
                ImGui::Selectable("Top", &CVars.enableTopLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Left", &CVars.enableLeftLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Right", &CVars.enableRightLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Bottom", &CVars.enableBottomLine, ImGuiSelectableFlags_DontClosePopups);

                ImGui::EndCombo();
            }
            ImGui::SliderInt("Length", &CVars.lineLength, 1, 100);
            ImGui::SliderInt("Thickness", &CVars.lineThickness, 1, 10);
            ImGui::SliderInt("Gap", &CVars.lineGap, 0, 100);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::CheckboxWithColorPicker("Outline", "Outline Color", &CVars.enableOutline, CVars.outlineColor);
            ImGui::BeginDisabled(!CVars.enableOutline);
            {
                ImGui::SliderInt("Thickness##", &CVars.outlineThickness, 1, 10);
            }
            ImGui::EndDisabled();
        }
        ImGui::EndDisabled();

        ImGui::NextColumn();

        ImGui::SeparatorText("Center Dot");

        ImGui::CheckboxWithColorPicker("Center Dot", "Center Dot Color", &CVars.enableCenterDot,
                                       CVars.centerDotColor);
        ImGui::BeginDisabled(!CVars.enableCenterDot);
        {
            ImGui::Checkbox("Filled", &CVars.filledCenterDot);
            ImGui::SliderInt("Size", &CVars.centerDotSize, 1, 20);
            ImGui::BeginDisabled(CVars.filledCenterDot);
            ImGui::SliderInt("Thickness###", &CVars.centerDotThickness, 1, 5);
            ImGui::EndDisabled();
            ImGui::SliderInt("Segments", &CVars.centerDotSegments, 4, 48);
            ImGui::Spacing();
        }
        ImGui::EndDisabled();

        ImGui::SeparatorText("Center Point");

        ImGui::BeginDisabled(CVars.useDynamicCenterPoint);

        if (ImGui::Button("Reset"))
            Crosshair::allCenterPoints[0].x = backend::screen_width / 2;
        ImGui::SameLine();
        ImGui::Button("-", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::allCenterPoints[0].x--;
        ImGui::SameLine();
        ImGui::Button("+", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::allCenterPoints[0].x++;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80);
        ImGui::SliderFloat("X", &Crosshair::allCenterPoints[0].x, 0, backend::screen_width, "%.0f");


        if (ImGui::Button("Reset##"))
            Crosshair::allCenterPoints[0].y = backend::screen_height / 2;
        ImGui::SameLine();
        ImGui::Button("-##", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::allCenterPoints[0].y--;
        ImGui::SameLine();
        ImGui::Button("+##", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::allCenterPoints[0].y++;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80);
        ImGui::SliderFloat("Y", &Crosshair::allCenterPoints[0].y, 0, backend::screen_height, "%.0f");

        ImGui::EndDisabled();

        if (ImGui::Checkbox("Dynamic Killer Crosshair", &CVars.useDynamicCenterPoint))
        {
            if (CVars.useDynamicCenterPoint)
                CVars.savedScreenCenterPoint = Crosshair::allCenterPoints[0];
            else
                Crosshair::allCenterPoints[0] = CVars.savedScreenCenterPoint;
        }

        ImGui::BeginDisabled(!CVars.useDynamicCenterPoint);

        ImGui::Checkbox("Show Charge Bar", &CVars.show_dynamic_charge_bar);
        gui::tool_tip("Will show a bar next to crosshair to indicate power charge");

        gui::drop_down_box("Killer", CVars.dynamicCenterPointIndex, dynamicKillers);

        ImGui::EndDisabled();

        ImGui::NextColumn();

        ImGui::SeparatorText("Profiles");

        ImGui::Text("Current Profile: %s", Crosshair::ProfileHandling::loadedProfileName.c_str());

        ImGui::PushStyleColor(ImGuiCol_Border, menu::styling::menu_accent.to_imvec4());
        if (ImGui::ListBox("##Profiles", &Crosshair::ProfileHandling::currentSelectedProfile,
                           [](void* data, int idx, const char** outText)
                           {
                               *outText = Crosshair::ProfileHandling::allProfiles.at(idx).c_str();
                               return true;
                           },
                           NULL, (int)Crosshair::ProfileHandling::allProfiles.size(), 5))
        {
            if (Crosshair::ProfileHandling::currentSelectedProfile < Crosshair::ProfileHandling::allProfiles.size())
            {
                Crosshair::ProfileHandling::selectedProfileName = Crosshair::ProfileHandling::allProfiles.at(
                    Crosshair::ProfileHandling::currentSelectedProfile);

                Crosshair::LoadProfile(Crosshair::ProfileHandling::selectedProfileName, CVars);
            }
        }
        ImGui::PopStyleColor();

        static char inputProfileName[32];

        ImGui::InputTextWithHint("##Name", "Profile Name...", inputProfileName, IM_ARRAYSIZE(inputProfileName));

        if (ImGui::Button("Reset Current Profile", ImVec2(169, 0)))
            Crosshair::ReloadProfiles();

        if (ImGui::Button("Save", ImVec2(169, 0)))
            Crosshair::SaveProfile(Crosshair::ProfileHandling::selectedProfileName, CVars);

        if (ImGui::Button("Create", ImVec2(80, 0)))
            Crosshair::CreateProfile(inputProfileName);

        ImGui::SameLine();

        if (ImGui::Button("Delete", ImVec2(81, 0)))
            Crosshair::DeleteProfile(Crosshair::ProfileHandling::selectedProfileName);
        

        // ImGui::SameLine();
        //
        // if (ImGui::Button("Load", ImVec2(81, 0)))
        //     Crosshair::LoadProfile(Crosshair::ProfileHandling::selectedProfileName, CVars);

        ImGui::EndColumns();
    }
    ImGui::EndDisabled();
}
