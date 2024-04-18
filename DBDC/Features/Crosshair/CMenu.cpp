#include "CMenu.h"
#include "Crosshair.h"
#include "..\..\Menu\Menu.hpp"
#include "GUI/GUI.h"
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "Overlay/Overlay.hpp"

void CMenu::Setup()
{
    Crosshair::CheckProfileDirectory();
    Crosshair::ReloadProfiles();
}

void CMenu::RenderUI()
{
    ImGui::SetCursorPos({50.F, 12.F});

    if (ImGui::Checkbox("Enable", &cvars.enabled))
    {
        if (cvars.enabled)
        {
            if (!overlay::is_overlay_created())
            {
                overlay::create_overlay();
                ImGui::SetCurrentContext(menu::main_context);
                glfwMakeContextCurrent(menu::main_window);
            }
        }
        else if (!overlay::is_overlay_needed())
            overlay::destroy_overlay();
    }

    ImGui::BeginDisabled(!cvars.enabled);
    {
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnWidth(0, 225);
        ImGui::SetColumnWidth(1, 260);

        ImGui::SeparatorText("Lines");

        ImGui::CheckboxWithColorPicker("Lines", "Line Color", &cvars.enableLines, cvars.lineColor);
        ImGui::BeginDisabled(!cvars.enableLines);
        {
            if (ImGui::BeginCombo("##Lines", "Lines", ImGuiComboFlags_NoArrowButton))
            {
                ImGui::Selectable("Top", &cvars.enableTopLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Left", &cvars.enableLeftLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Right", &cvars.enableRightLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Bottom", &cvars.enableBottomLine, ImGuiSelectableFlags_DontClosePopups);

                ImGui::EndCombo();
            }
            ImGui::SliderInt("Length", &cvars.lineLength, 1, 100);
            ImGui::SliderInt("Thickness", &cvars.lineThickness, 1, 10);
            ImGui::SliderInt("Gap", &cvars.lineGap, 0, 100);

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::CheckboxWithColorPicker("Outline", "Outline Color", &cvars.enableOutline, cvars.outlineColor);
            ImGui::BeginDisabled(!cvars.enableOutline);
            {
                ImGui::SliderInt("Thickness##", &cvars.outlineThickness, 1, 10);
            }
            ImGui::EndDisabled();
        }
        ImGui::EndDisabled();

        ImGui::NextColumn();

        ImGui::SeparatorText("Center Dot");

        ImGui::CheckboxWithColorPicker("Center Dot", "Center Dot Color", &cvars.enableCenterDot,
                                       cvars.centerDotColor);
        ImGui::BeginDisabled(!cvars.enableCenterDot);
        {
            ImGui::Checkbox("Filled", &cvars.filledCenterDot);
            ImGui::SliderInt("Size", &cvars.centerDotSize, 1, 20);
            ImGui::BeginDisabled(cvars.filledCenterDot);
            ImGui::SliderInt("Thickness###", &cvars.centerDotThickness, 1, 5);
            ImGui::EndDisabled();
            ImGui::SliderInt("Segments", &cvars.centerDotSegments, 4, 48);
            ImGui::Spacing();
        }
        ImGui::EndDisabled();

        ImGui::SeparatorText("Center Point");

        ImGui::BeginDisabled(cvars.useDynamicCenterPoint);

        if (ImGui::Button("Reset"))
            Crosshair::all_center_points[0].x = backend::screen_width / 2;
        ImGui::SameLine();
        ImGui::Button("-", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::all_center_points[0].x--;
        ImGui::SameLine();
        ImGui::Button("+", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::all_center_points[0].x++;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80);
        ImGui::SliderFloat("X", &Crosshair::all_center_points[0].x, 0, backend::screen_width, "%.0f");


        if (ImGui::Button("Reset##"))
            Crosshair::all_center_points[0].y = backend::screen_height / 2;
        ImGui::SameLine();
        ImGui::Button("-##", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::all_center_points[0].y--;
        ImGui::SameLine();
        ImGui::Button("+##", ImVec2(20, 0));
        if (ImGui::IsItemActive() && ImGui::IsItemHovered())
            Crosshair::all_center_points[0].y++;
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80);
        ImGui::SliderFloat("Y", &Crosshair::all_center_points[0].y, 0, backend::screen_height, "%.0f");

        ImGui::EndDisabled();

        if (ImGui::Checkbox("Dynamic Killer Crosshair", &cvars.useDynamicCenterPoint))
        {
            if (cvars.useDynamicCenterPoint)
                cvars.savedScreenCenterPoint = Crosshair::all_center_points[0];
            else
                Crosshair::all_center_points[0] = cvars.savedScreenCenterPoint;
        }

        ImGui::BeginDisabled(!cvars.useDynamicCenterPoint);

        ImGui::Checkbox("Show Charge Bar", &cvars.show_dynamic_charge_bar);
        gui::tool_tip("Will show a bar next to crosshair to indicate power charge");

        gui::drop_down_box("Killer", cvars.dynamicCenterPointIndex, dynamicKillers);

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

                Crosshair::LoadProfile(Crosshair::ProfileHandling::selectedProfileName, cvars);
            }
        }
        ImGui::PopStyleColor();

        static char inputProfileName[32];

        ImGui::InputTextWithHint("##Name", "Profile Name...", inputProfileName, IM_ARRAYSIZE(inputProfileName));

        if (ImGui::Button("Save", ImVec2(169, 0)))
            Crosshair::SaveProfile(Crosshair::ProfileHandling::selectedProfileName, cvars);

        if (ImGui::Button("Create", ImVec2(80, 0)))
            Crosshair::CreateProfile(inputProfileName);

        ImGui::SameLine();

        if (ImGui::Button("Delete", ImVec2(81, 0)))
            Crosshair::DeleteProfile(Crosshair::ProfileHandling::selectedProfileName);

        if (ImGui::Button("Reset Current Profile", ImVec2(169, 0)))
            Crosshair::ResetProfile(cvars);

        ImGui::EndColumns();
    }
    ImGui::EndDisabled();
}
