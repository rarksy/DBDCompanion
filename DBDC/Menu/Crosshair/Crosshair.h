#pragma once
#include "ImGui/imgui.h"

namespace Crosshair
{
    void DrawCrosshair();
    void DrawLines();
    void DrawOutline();
    void DrawCenterDot();
    
    inline bool setCustom = false;
    
    inline bool masterSwitch = false;

    inline bool enableLines = false;
    inline bool enableTopLine = true;
    inline bool enableLeftLine = true;
    inline bool enableRightLine = true;
    inline bool enableBottomLine = true;
    inline int lineLength = 4;
    inline int lineThickness = 2;
    inline int lineGap = 3;
    inline ImColor lineColor = ImColor(255, 74, 74);

    inline bool enableOutline = true;
    inline ImColor outlineColor = ImColor(0, 0, 0);
    inline int outlineThickness = 1;
    

    inline bool enableCenterDot = false;
    inline bool filledCenterDot = false;
    inline int centerDotSize = 7;
    inline int centerDotThickness = 2;
    inline int centerDotSegments = 12;
    inline ImColor centerDotColor = ImColor(255, 74, 74);
    
}
