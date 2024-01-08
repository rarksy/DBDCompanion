#include "CMenu.h"

#include "Crosshair.h"
#include "../Menu.h"
#include "../../Config/Config.h"
#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

void CMenu::RenderUI()
{
    ImGui::Spacing();
    ImGui::Spacing();
    
    if (ImGui::Checkbox("Enable", &Crosshair::masterSwitch))
    {
        if (Crosshair::masterSwitch)
        {
            Menu::Overlay::windowWidth = GetSystemMetrics(SM_CXSCREEN);
            
            if (!Menu::Overlay::IsOverlayCreated())
            {
                Menu::Overlay::CreateOverlay();
                ImGui::SetCurrentContext(Menu::mainContext);
                glfwMakeContextCurrent(Menu::mainWindow);
            }
    
            Menu::Overlay::windowHeight = GetSystemMetrics(SM_CYSCREEN);
    
            glfwSetWindowSize(Menu::Overlay::window, Menu::Overlay::windowWidth, Menu::Overlay::windowHeight);
        }
        else
        {
            Menu::Overlay::windowWidth = GetSystemMetrics(SM_CXSCREEN) / 3;
    
            Menu::Overlay::DestroyOverlay();
        }
    }
        
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
        
    ImGui::BeginDisabled(!Crosshair::masterSwitch);
    {
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnWidth(0, 260);
        ImGui::SetColumnWidth(1, 260);
        
        ImGui::CheckboxWithColorPicker("Lines", "Line Color", &Crosshair::enableLines, Crosshair::lineColor);
        ImGui::BeginDisabled(!Crosshair::enableLines);
        {
            if (ImGui::BeginCombo("##Lines", "Lines"))
            {
                ImGui::Selectable("Top", &Crosshair::enableTopLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Left", &Crosshair::enableLeftLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Right", &Crosshair::enableRightLine, ImGuiSelectableFlags_DontClosePopups);
                ImGui::Selectable("Bottom", &Crosshair::enableBottomLine, ImGuiSelectableFlags_DontClosePopups);
        
                ImGui::EndCombo();
            }
            ImGui::SliderInt("Length", &Crosshair::lineLength, 1, 100);
            ImGui::SliderInt("Thickness", &Crosshair::lineThickness, 1, 10);
            ImGui::SliderInt("Gap", &Crosshair::lineGap, 0, 100);
        
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
        
            ImGui::CheckboxWithColorPicker("Outline", "Outline Color", &Crosshair::enableOutline, Crosshair::outlineColor);
            ImGui::BeginDisabled(!Crosshair::enableOutline);
            {
                ImGui::SliderInt("Thickness##", &Crosshair::outlineThickness, 1, 10);
            }
            ImGui::EndDisabled();
        }
        ImGui::EndDisabled();
        
        ImGui::NextColumn();
        
        ImGui::CheckboxWithColorPicker("Center Dot", "Center Dot Color", &Crosshair::enableCenterDot,
                                       Crosshair::centerDotColor);
        ImGui::BeginDisabled(!Crosshair::enableCenterDot);
        {
            ImGui::Checkbox("Filled", &Crosshair::filledCenterDot);
            ImGui::SliderInt("Size", &Crosshair::centerDotSize, 1, 20);
            ImGui::BeginDisabled(Crosshair::filledCenterDot);
            ImGui::SliderInt("Thickness###", &Crosshair::centerDotThickness, 1, 5);
            ImGui::EndDisabled();
            ImGui::SliderInt("Segments", &Crosshair::centerDotSegments, 4, 48);
            ImGui::Spacing();
        }
        ImGui::EndDisabled();
        ImGui::EndColumns();
    }
    ImGui::EndDisabled();
}
