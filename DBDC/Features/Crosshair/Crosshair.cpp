#include "Crosshair.h"
#include "../../Backend/Backend.hpp"
#include <Windows.h>

void Crosshair::Setup()
{
    CVars.trueScreenCenterPoint = ImVec2(backend::screen_width / 2, backend::screen_height / 2);
    allCenterPoints.push_back(CVars.trueScreenCenterPoint);
}

void Crosshair::DrawCrosshairs()
{
    if (CVars.useDynamicCenterPoint)
        ModifyDynamicCenterPoint();
    else if (allCenterPoints.size() > 0)
        for (int i = 1; i < allCenterPoints.size(); i++)
            allCenterPoints.erase(allCenterPoints.begin() + i);

    for (int i = 0; i < allCenterPoints.size(); i++)
    {
        const ImVec2 centerPoint = allCenterPoints[i];

        if (CVars.enableCenterDot)
            DrawCenterDot(centerPoint);

        if (CVars.enableLines)
        {
            if (CVars.enableOutline)
                DrawOutline(centerPoint);

            DrawLines(centerPoint);
        }
    }
}

bool isEven(int number)
{
    return (!(number & 1));
}

void Crosshair::DrawLines(const ImVec2& centerPoint)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    const int thickness = CVars.lineThickness;
    const float halfThickness = thickness / 2;

    const float numMinus = thickness / 2;
    const float numAdd = thickness - numMinus;

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
                startPos = ImVec2(centerPoint.x - halfThickness, centerPoint.y - CVars.lineGap - CVars.lineLength);
                endPos = ImVec2(centerPoint.x + halfThickness + (thickness % 2), centerPoint.y - CVars.lineGap);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus, (centerPoint.y + 0.51F) - CVars.lineGap);
                endPos = ImVec2(centerPoint.x + numAdd, (centerPoint.y + 0.51F) - CVars.lineGap - CVars.lineLength);
            }
            break;

        case 1:
            drawLine = CVars.enableBottomLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness, centerPoint.y + CVars.lineGap);
                endPos = ImVec2(centerPoint.x + halfThickness + (thickness % 2),
                                centerPoint.y + CVars.lineGap + CVars.lineLength);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus, centerPoint.y + CVars.lineGap);
                endPos = ImVec2(centerPoint.x + numAdd, centerPoint.y + CVars.lineGap + CVars.lineLength);
            }
            break;

        case 2:
            drawLine = CVars.enableLeftLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x - CVars.lineGap - CVars.lineLength, centerPoint.y - halfThickness);
                endPos = ImVec2(centerPoint.x - CVars.lineGap, centerPoint.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2((centerPoint.x + 0.51F) - CVars.lineGap, centerPoint.y - numMinus);
                endPos = ImVec2((centerPoint.x + 0.51F) - CVars.lineGap - CVars.lineLength, centerPoint.y + numAdd);
            }
            break;

        case 3:
            drawLine = CVars.enableRightLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x + CVars.lineGap, centerPoint.y - halfThickness);
                endPos = ImVec2(centerPoint.x + CVars.lineGap + CVars.lineLength,
                                centerPoint.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2(centerPoint.x + CVars.lineGap, centerPoint.y - numMinus);
                endPos = ImVec2(centerPoint.x + CVars.lineGap + CVars.lineLength, centerPoint.y + numAdd);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, CVars.lineColor);
    }
}

void Crosshair::DrawOutline(const ImVec2& centerPoint)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    const int lineThickness = CVars.lineThickness;
    const int outlineThickness = CVars.outlineThickness;
    const int halfThickness = lineThickness / 2;

    const int numMinus = lineThickness / 2;
    const int numAdd = lineThickness - numMinus;

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
                startPos = ImVec2(centerPoint.x - halfThickness - outlineThickness,
                                  centerPoint.y - CVars.lineGap - CVars.lineLength - outlineThickness);
                endPos = ImVec2(centerPoint.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                centerPoint.y - CVars.lineGap + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus - outlineThickness,
                                  (centerPoint.y + 0.51F) - CVars.lineGap + outlineThickness);
                endPos = ImVec2(centerPoint.x + numAdd + outlineThickness,
                                (centerPoint.y + 0.51F) - CVars.lineGap - CVars.lineLength - outlineThickness);
            }
            break;

        case 1:
            drawLine = CVars.enableBottomLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness - outlineThickness,
                                  centerPoint.y + CVars.lineGap - outlineThickness);
                endPos = ImVec2(centerPoint.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                centerPoint.y + CVars.lineGap + CVars.lineLength + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus - outlineThickness,
                                  centerPoint.y + CVars.lineGap - outlineThickness);
                endPos = ImVec2(centerPoint.x + numAdd + outlineThickness,
                                centerPoint.y + CVars.lineGap + CVars.lineLength + outlineThickness);
            }
            break;

        case 2:
            drawLine = CVars.enableLeftLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x - CVars.lineGap - CVars.lineLength - outlineThickness,
                                  centerPoint.y - halfThickness - outlineThickness);
                endPos = ImVec2(centerPoint.x - CVars.lineGap + outlineThickness,
                                centerPoint.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2((centerPoint.x + 0.51F) - CVars.lineGap + outlineThickness,
                                  centerPoint.y - numMinus - outlineThickness);
                endPos = ImVec2((centerPoint.x + 0.51F) - CVars.lineGap - CVars.lineLength - outlineThickness,
                                centerPoint.y + numAdd + outlineThickness);
            }
            break;

        case 3:
            drawLine = CVars.enableRightLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x + CVars.lineGap - outlineThickness,
                                  centerPoint.y - halfThickness - outlineThickness);
                endPos = ImVec2(centerPoint.x + CVars.lineGap + CVars.lineLength + outlineThickness,
                                centerPoint.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x + CVars.lineGap - outlineThickness,
                                  centerPoint.y - numMinus - outlineThickness);
                endPos = ImVec2(centerPoint.x + CVars.lineGap + CVars.lineLength + outlineThickness,
                                centerPoint.y + numAdd + outlineThickness);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, CVars.outlineColor);
    }
}

void Crosshair::DrawCenterDot(const ImVec2& centerPoint)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    if (CVars.filledCenterDot)
        drawList->AddCircleFilled(centerPoint, CVars.centerDotSize, CVars.centerDotColor,
                                  CVars.centerDotSegments);
    else
        drawList->AddCircle(centerPoint, CVars.centerDotSize, CVars.centerDotColor,
                            CVars.centerDotSegments, CVars.centerDotThickness);
}

void Crosshair::ModifyDynamicCenterPoint()
{
    static std::chrono::steady_clock::time_point startTime;
    static bool isRButtonDown;

    if (CVars.dynamicCenterPointIndex == 0) // Huntress
    {
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }

            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();

            if (allCenterPoints[0].y < CVars.trueScreenCenterPoint.y + 30 && duration >= 2000)
                allCenterPoints[0].y++;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            allCenterPoints[0] = ImVec2(backend::screen_width / 2, backend::screen_height / 2);
        }
    }
    else if (CVars.dynamicCenterPointIndex == 1) // Deathslinger
    {
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
                allCenterPoints[0].y = 9999; // off screen;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            allCenterPoints[0].x = backend::screen_width / 2 + 3;
            allCenterPoints[0].y = CVars.trueScreenCenterPoint.y;
        }
    }
    else if (CVars.dynamicCenterPointIndex == 2) // Trickster
    {
        allCenterPoints[0] = ImVec2(backend::screen_width / 2 - 15, backend::screen_height / 2);
        if (allCenterPoints.size() < 2)
            allCenterPoints.push_back(ImVec2(backend::screen_width / 2 + 15, backend::screen_height / 2));
    }
    else if (CVars.dynamicCenterPointIndex == 3) // Clown
    {
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();


            if (duration > 200 && allCenterPoints[0].y > 480)
                allCenterPoints[0].y -= 4;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;
            
            allCenterPoints[0].y = 890;
        }
    }
    else if (CVars.dynamicCenterPointIndex == 4) // The Unknown
    {
        if(allCenterPoints[0].y != 770)
            allCenterPoints[0].y = 770;
    }

    if (CVars.dynamicCenterPointIndex != 2)
        if (allCenterPoints.size() > 0)
            for (int i = 1; i < allCenterPoints.size(); i++)
                allCenterPoints.erase(allCenterPoints.begin() + i);
}
