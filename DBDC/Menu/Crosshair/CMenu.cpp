#include "CMenu.h"
#include "Crosshair.h"
#include "../Menu.h"
#include "../GUI/GUI.h"
#include "../HookCounter/HookCounter.h"
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
            if (!Menu::Overlay::IsOverlayCreated())
            {
                Menu::Overlay::CreateOverlay();
                ImGui::SetCurrentContext(Menu::mainContext);
                glfwMakeContextCurrent(Menu::mainWindow);
            }
        }
        else if (!HCVars.enabled)
            Menu::Overlay::DestroyOverlay();
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
            Crosshair::allCenterPoints[0].x = Backend::screenWidth / 2;
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
        ImGui::SliderFloat("X", &Crosshair::allCenterPoints[0].x, 0, Backend::screenWidth, "%.0f");


        if (ImGui::Button("Reset##"))
            Crosshair::allCenterPoints[0].y = Backend::screenHeight / 2;
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
        ImGui::SliderFloat("Y", &Crosshair::allCenterPoints[0].y, 0, Backend::screenHeight, "%.0f");

        ImGui::EndDisabled();

        if (ImGui::Checkbox("Dynamic Killer Crosshair", &CVars.useDynamicCenterPoint))
        {
            if (CVars.useDynamicCenterPoint)
                CVars.savedScreenCenterPoint = Crosshair::allCenterPoints[0];
            else
                Crosshair::allCenterPoints[0] = CVars.savedScreenCenterPoint;
        }

        ImGui::BeginDisabled(!CVars.useDynamicCenterPoint);

        GUI::DropDownBox("Killer", CVars.dynamicCenterPointIndex, dynamicKillers);

        ImGui::EndDisabled();

        ImGui::NextColumn();

        ImGui::SeparatorText("Profiles");

        ImGui::Text("Current Profile: %s", Crosshair::ProfileHandling::loadedProfileName.c_str());
        
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor(255, 83, 83).Value);
        if (ImGui::ListBox("##Profiles", &Crosshair::ProfileHandling::currentSelectedProfile,
                           [](void* data, int idx, const char** outText)
                           {
                               *outText = Crosshair::ProfileHandling::allProfiles.at(idx).c_str();
                               return true;
                           },
                           NULL, (int)Crosshair::ProfileHandling::allProfiles.size(), 5))
        {
            if (Crosshair::ProfileHandling::currentSelectedProfile < Crosshair::ProfileHandling::allProfiles.size())
                Crosshair::ProfileHandling::selectedProfileName = Crosshair::ProfileHandling::allProfiles.at(
                    Crosshair::ProfileHandling::currentSelectedProfile);
        }
        ImGui::PopStyleColor();

        static char inputProfileName[32];

        ImGui::InputTextWithHint("##Name", "Profile Name...", inputProfileName, IM_ARRAYSIZE(inputProfileName));

        if (ImGui::Button("Reset Current Profile", ImVec2(169, 0)))
            Crosshair::ReloadProfiles();

        if (ImGui::Button("Create", ImVec2(80, 0)))
            Crosshair::CreateProfile(inputProfileName);

        ImGui::SameLine();

        if (ImGui::Button("Delete", ImVec2(81, 0)))
            Crosshair::DeleteProfile(Crosshair::ProfileHandling::selectedProfileName);
        
        if (ImGui::Button("Save", ImVec2(80, 0)))
            Crosshair::SaveProfile(Crosshair::ProfileHandling::selectedProfileName, CVars);

        ImGui::SameLine();

        if (ImGui::Button("Load", ImVec2(81, 0)))
            Crosshair::LoadProfile(Crosshair::ProfileHandling::selectedProfileName, CVars);

        ImGui::EndColumns();
    }
    ImGui::EndDisabled();
}
