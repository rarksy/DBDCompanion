#include "Crosshair.h"
#include "../../Backend/Backend.hpp"
#include <Windows.h>

void Crosshair::Setup()
{
    CVars.screenCenterPoint = ImVec2(Backend::screenWidth / 2, Backend::screenHeight / 2);
    CVars.trueScreenCenterPoint = CVars.screenCenterPoint;
}

void Crosshair::DrawCrosshair()
{
    if (CVars.useDynamicCenterPoint)
        ModifyDynamicCenterPoint();

    if (CVars.enableCenterDot)
        DrawCenterDot();

    if (CVars.enableLines)
    {
        if (CVars.enableOutline)
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
    ImVec2 center = CVars.screenCenterPoint;
    int thickness = CVars.lineThickness;
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
            drawLine = CVars.enableTopLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - halfThickness, center.y - CVars.lineGap - CVars.lineLength);
                endPos = ImVec2(center.x + halfThickness + (thickness % 2), center.y - CVars.lineGap);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus, (center.y + 0.51F) - CVars.lineGap);
                endPos = ImVec2(center.x + numAdd, (center.y + 0.51F) - CVars.lineGap - CVars.lineLength);
            }
            break;

        case 1:
            drawLine = CVars.enableBottomLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - halfThickness, center.y + CVars.lineGap);
                endPos = ImVec2(center.x + halfThickness + (thickness % 2),
                                center.y + CVars.lineGap + CVars.lineLength);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus, center.y + CVars.lineGap);
                endPos = ImVec2(center.x + numAdd, center.y + CVars.lineGap + CVars.lineLength);
            }
            break;

        case 2:
            drawLine = CVars.enableLeftLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x - CVars.lineGap - CVars.lineLength, center.y - halfThickness);
                endPos = ImVec2(center.x - CVars.lineGap, center.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2((center.x + 0.51F) - CVars.lineGap, center.y - numMinus);
                endPos = ImVec2((center.x + 0.51F) - CVars.lineGap - CVars.lineLength, center.y + numAdd);
            }
            break;

        case 3:
            drawLine = CVars.enableRightLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(center.x + CVars.lineGap, center.y - halfThickness);
                endPos = ImVec2(center.x + CVars.lineGap + CVars.lineLength,
                                center.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2(center.x + CVars.lineGap, center.y - numMinus);
                endPos = ImVec2(center.x + CVars.lineGap + CVars.lineLength, center.y + numAdd);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, CVars.lineColor);
    }
}

void Crosshair::DrawOutline()
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 center = CVars.screenCenterPoint;
    int lineThickness = CVars.lineThickness;
    int outlineThickness = CVars.outlineThickness;
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
            drawLine = CVars.enableTopLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - halfThickness - outlineThickness,
                                  center.y - CVars.lineGap - CVars.lineLength - outlineThickness);
                endPos = ImVec2(center.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                center.y - CVars.lineGap + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus - outlineThickness,
                                  (center.y + 0.51F) - CVars.lineGap + outlineThickness);
                endPos = ImVec2(center.x + numAdd + outlineThickness,
                                (center.y + 0.51F) - CVars.lineGap - CVars.lineLength - outlineThickness);
            }
            break;

        case 1:
            drawLine = CVars.enableBottomLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - halfThickness - outlineThickness,
                                  center.y + CVars.lineGap - outlineThickness);
                endPos = ImVec2(center.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                center.y + CVars.lineGap + CVars.lineLength + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x - numMinus - outlineThickness,
                                  center.y + CVars.lineGap - outlineThickness);
                endPos = ImVec2(center.x + numAdd + outlineThickness,
                                center.y + CVars.lineGap + CVars.lineLength + outlineThickness);
            }
            break;

        case 2:
            drawLine = CVars.enableLeftLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x - CVars.lineGap - CVars.lineLength - outlineThickness,
                                  center.y - halfThickness - outlineThickness);
                endPos = ImVec2(center.x - CVars.lineGap + outlineThickness,
                                center.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2((center.x + 0.51F) - CVars.lineGap + outlineThickness,
                                  center.y - numMinus - outlineThickness);
                endPos = ImVec2((center.x + 0.51F) - CVars.lineGap - CVars.lineLength - outlineThickness,
                                center.y + numAdd + outlineThickness);
            }
            break;

        case 3:
            drawLine = CVars.enableRightLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(center.x + CVars.lineGap - outlineThickness,
                                  center.y - halfThickness - outlineThickness);
                endPos = ImVec2(center.x + CVars.lineGap + CVars.lineLength + outlineThickness,
                                center.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2(center.x + CVars.lineGap - outlineThickness,
                                  center.y - numMinus - outlineThickness);
                endPos = ImVec2(center.x + CVars.lineGap + CVars.lineLength + outlineThickness,
                                center.y + numAdd + outlineThickness);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, CVars.outlineColor);
    }
}


void Crosshair::DrawCenterDot()
{
    // Get Relevant Variables 
    ImVec2 center = CVars.screenCenterPoint;
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    if (CVars.filledCenterDot) // Draw A Filled Circle Since The Filled Checkbox Is Ticked
        drawList->AddCircleFilled(center, CVars.centerDotSize, CVars.centerDotColor,
                                  CVars.centerDotSegments);
    else // No Fill
        drawList->AddCircle(center, CVars.centerDotSize, CVars.centerDotColor,
                            CVars.centerDotSegments, CVars.centerDotThickness);
}

void Crosshair::ModifyDynamicCenterPoint()
{
    static std::chrono::steady_clock::time_point startTime;
    static bool isRButtonDown;

    switch (CVars.dynamicCenterPointIndex)
    {
    default:
        CVars.screenCenterPoint = CVars.trueScreenCenterPoint;
        break;

    case 0: // Huntress

        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }

            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();

            if (CVars.screenCenterPoint.y < CVars.trueScreenCenterPoint.y + 30 && duration >= 2000)
                CVars.screenCenterPoint.y++;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            CVars.screenCenterPoint = CVars.trueScreenCenterPoint;
        }
        break;

    case 1: // Deathslinger

        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();

            if (duration > 400)
                CVars.screenCenterPoint.y = 9999; // off screen;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            CVars.screenCenterPoint.x = Backend::screenWidth / 2 + 3;
            CVars.screenCenterPoint.y = CVars.trueScreenCenterPoint.y;
        }

        break;
    }
}
