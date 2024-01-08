#include "Crosshair.h"

#include <random>
#include <Windows.h>

#include "../../Backend/Backend.hpp"

void Crosshair::DrawCrosshair()
{
    if (Crosshair::enableCenterDot)
        DrawCenterDot();

    if (Crosshair::enableLines)
    {
        if (Crosshair::enableOutline)
            DrawOutline();
        
        DrawLines();
    }
}

bool isEven(int number)
{
    return (!(number & 1));
}

void Crosshair::DrawLines()
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 center = ImVec2(Backend::screenWidth / 2, Backend::screenHeight / 2);
    int thickness = Crosshair::lineThickness;
    float halfThickness = thickness / 2;

    float numMinus = thickness / 2;
    float numAdd = thickness - numMinus;

    for (int i = 0; i < 4; i++)
    {
        bool drawLine = false;
        ImVec2 startPos;
        ImVec2 endPos;

        switch (i)
        {
        case 0:
            drawLine = Crosshair::enableTopLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - halfThickness, center.y - Crosshair::lineGap - Crosshair::lineLength);
                endPos = ImVec2(center.x + halfThickness + (thickness % 2), center.y - Crosshair::lineGap);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus, (center.y + 0.51F) - Crosshair::lineGap);
                endPos = ImVec2(center.x + numAdd, (center.y + 0.51F) - Crosshair::lineGap - Crosshair::lineLength);
            }
            break;

        case 1:
            drawLine = Crosshair::enableBottomLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - halfThickness, center.y + Crosshair::lineGap);
                endPos = ImVec2(center.x + halfThickness + (thickness % 2),
                                center.y + Crosshair::lineGap + Crosshair::lineLength);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus, center.y + Crosshair::lineGap);
                endPos = ImVec2(center.x + numAdd, center.y + Crosshair::lineGap + Crosshair::lineLength);
            }
            break;

        case 2:
            drawLine = Crosshair::enableLeftLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - Crosshair::lineGap - Crosshair::lineLength, center.y - halfThickness);
                endPos = ImVec2(center.x - Crosshair::lineGap, center.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2((center.x + 0.51F) - Crosshair::lineGap, center.y - numMinus);
                endPos = ImVec2((center.x + 0.51F) - Crosshair::lineGap - Crosshair::lineLength, center.y + numAdd);
            }
            break;

        case 3:
            drawLine = Crosshair::enableRightLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x + Crosshair::lineGap, center.y - halfThickness);
                endPos = ImVec2(center.x + Crosshair::lineGap + Crosshair::lineLength,
                                center.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2(center.x + Crosshair::lineGap, center.y - numMinus);
                endPos = ImVec2(center.x + Crosshair::lineGap + Crosshair::lineLength, center.y + numAdd);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, Crosshair::lineColor);
    }
}

void Crosshair::DrawOutline()
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 center = ImVec2(Backend::screenWidth / 2, Backend::screenHeight / 2);
    int lineThickness = Crosshair::lineThickness;
    int outlineThickness = Crosshair::outlineThickness;
    int halfThickness = lineThickness / 2;

    int numMinus = lineThickness / 2;
    int numAdd = lineThickness - numMinus;

    for (int i = 0; i < 4; i++)
    {
        bool drawLine = false;
        ImVec2 startPos;
        ImVec2 endPos;

        switch (i)
        {
        case 0:
            drawLine = enableTopLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - halfThickness - outlineThickness,
                                  center.y - Crosshair::lineGap - Crosshair::lineLength - outlineThickness);
                endPos = ImVec2(center.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                center.y - Crosshair::lineGap + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus - outlineThickness,
                                  (center.y + 0.51F) - Crosshair::lineGap + outlineThickness);
                endPos = ImVec2(center.x + numAdd + outlineThickness,
                                (center.y + 0.51F) - Crosshair::lineGap - Crosshair::lineLength - outlineThickness);
            }
            break;

        case 1:
            drawLine = enableBottomLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - halfThickness - outlineThickness,
                                  center.y + Crosshair::lineGap - outlineThickness);
                endPos = ImVec2(center.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                center.y + Crosshair::lineGap + Crosshair::lineLength + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus - outlineThickness,
                                  center.y + Crosshair::lineGap - outlineThickness);
                endPos = ImVec2(center.x + numAdd + outlineThickness,
                                center.y + Crosshair::lineGap + Crosshair::lineLength + outlineThickness);
            }
            break;

        case 2:
            drawLine = enableLeftLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - Crosshair::lineGap - Crosshair::lineLength - outlineThickness,
                                  center.y - halfThickness - outlineThickness);
                endPos = ImVec2(center.x - Crosshair::lineGap + outlineThickness,
                                center.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2((center.x + 0.51F) - Crosshair::lineGap + outlineThickness,
                                  center.y - numMinus - outlineThickness);
                endPos = ImVec2((center.x + 0.51F) - Crosshair::lineGap - Crosshair::lineLength - outlineThickness,
                                center.y + numAdd + outlineThickness);
            }
            break;

        case 3:
            drawLine = enableRightLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x + Crosshair::lineGap - outlineThickness,
                                  center.y - halfThickness - outlineThickness);
                endPos = ImVec2(center.x + Crosshair::lineGap + Crosshair::lineLength + outlineThickness,
                                center.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x + Crosshair::lineGap - outlineThickness,
                                  center.y - numMinus - outlineThickness);
                endPos = ImVec2(center.x + Crosshair::lineGap + Crosshair::lineLength + outlineThickness,
                                center.y + numAdd + outlineThickness);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, Crosshair::outlineColor);
    }
}


void Crosshair::DrawCenterDot()
{
    // Get Relevant Variables 
    ImVec2 center = ImVec2(Backend::screenWidth / 2, Backend::screenHeight / 2);
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    if (Crosshair::filledCenterDot) // Draw A Filled Circle Since The Filled Checkbox Is Ticked
        drawList->AddCircleFilled(center, Crosshair::centerDotSize, Crosshair::centerDotColor,
                                  Crosshair::centerDotSegments);
    else // No Fill
        drawList->AddCircle(center, Crosshair::centerDotSize, Crosshair::centerDotColor,
                            Crosshair::centerDotSegments, Crosshair::centerDotThickness);
}
