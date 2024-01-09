#include "CMenu.h"

#include "Crosshair.h"
#include "../Menu.h"
#include "../../Config/Config.h"
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

void CMenu::RenderUI()
{
    
    if (ImGui::Checkbox("Enable", &CVars.masterSwitch))
    {
        if (CVars.masterSwitch)
        {
            if (!Menu::Overlay::IsOverlayCreated())
            {
                Menu::Overlay::CreateOverlay();
                ImGui::SetCurrentContext(Menu::mainContext);
                glfwMakeContextCurrent(Menu::mainWindow);
            }
        }
        else
            Menu::Overlay::DestroyOverlay();
        
    }
        
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
        
    ImGui::BeginDisabled(!CVars.masterSwitch);
    {
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnWidth(0, 260);
        ImGui::SetColumnWidth(1, 260);

        ImGui::SeparatorText("Lines");
        
        ImGui::CheckboxWithColorPicker("Lines", "Line Color", &CVars.enableLines, CVars.lineColor);
        ImGui::BeginDisabled(!CVars.enableLines);
        {
            if (ImGui::BeginCombo("##Lines", "Lines"))
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

        ImGui::SeparatorText("Other");
        
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

        ImGui::NextColumn();

        ImGui::SeparatorText("Settings");
        if (ImGui::Button("Save Settings"))
            CVars.Save(CVars);
        
        if (ImGui::Button("Load Settings"))
            CVars.Load(CVars);

        if (ImGui::Button("Reset Settings"))
            CVars.Reset(CVars);
        
        ImGui::EndColumns();
    }
    ImGui::EndDisabled();
}
