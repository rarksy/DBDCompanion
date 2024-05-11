#include "crosshair.h"
#include "backend/backend.hpp"
#include <Windows.h>

void Crosshair::Setup()
{
    cvars.true_screen_center_point = ImVec2(backend::screen_width / 2, backend::screen_height / 2);
    all_center_points.push_back(cvars.true_screen_center_point);
}

void Crosshair::DrawCrosshairs()
{
    if (cvars.useDynamicCenterPoint)
        ModifyDynamicCenterPoint();
    else if (all_center_points.size() > 0)
        for (int i = 1; i < all_center_points.size(); i++)
            all_center_points.erase(all_center_points.begin() + i);

    for (int i = 0; i < all_center_points.size(); i++)
    {
        const ImVec2 centerPoint = all_center_points[i];

        if (cvars.enableCenterDot)
            DrawCenterDot(centerPoint);

        if (cvars.enableLines)
        {
            if (cvars.enableOutline)
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
    const int thickness = cvars.lineThickness;
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
            drawLine = cvars.enableTopLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness, centerPoint.y - cvars.lineGap - cvars.lineLength);
                endPos = ImVec2(centerPoint.x + halfThickness + (thickness % 2), centerPoint.y - cvars.lineGap);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus, (centerPoint.y + 0.51F) - cvars.lineGap);
                endPos = ImVec2(centerPoint.x + numAdd, (centerPoint.y + 0.51F) - cvars.lineGap - cvars.lineLength);
            }
            break;

        case 1:
            drawLine = cvars.enableBottomLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness, centerPoint.y + cvars.lineGap);
                endPos = ImVec2(centerPoint.x + halfThickness + (thickness % 2),
                                centerPoint.y + cvars.lineGap + cvars.lineLength);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus, centerPoint.y + cvars.lineGap);
                endPos = ImVec2(centerPoint.x + numAdd, centerPoint.y + cvars.lineGap + cvars.lineLength);
            }
            break;

        case 2:
            drawLine = cvars.enableLeftLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x - cvars.lineGap - cvars.lineLength, centerPoint.y - halfThickness);
                endPos = ImVec2(centerPoint.x - cvars.lineGap, centerPoint.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2((centerPoint.x + 0.51F) - cvars.lineGap, centerPoint.y - numMinus);
                endPos = ImVec2((centerPoint.x + 0.51F) - cvars.lineGap - cvars.lineLength, centerPoint.y + numAdd);
            }
            break;

        case 3:
            drawLine = cvars.enableRightLine;

            if (isEven(thickness))
            {
                startPos = ImVec2(centerPoint.x + cvars.lineGap, centerPoint.y - halfThickness);
                endPos = ImVec2(centerPoint.x + cvars.lineGap + cvars.lineLength,
                                centerPoint.y + halfThickness + (thickness % 2));
            }
            else
            {
                startPos = ImVec2(centerPoint.x + cvars.lineGap, centerPoint.y - numMinus);
                endPos = ImVec2(centerPoint.x + cvars.lineGap + cvars.lineLength, centerPoint.y + numAdd);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, cvars.lineColor);
    }
}

void Crosshair::DrawOutline(const ImVec2& centerPoint)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    const int lineThickness = cvars.lineThickness;
    const int outlineThickness = cvars.outlineThickness;
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
            drawLine = cvars.enableTopLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness - outlineThickness,
                                  centerPoint.y - cvars.lineGap - cvars.lineLength - outlineThickness);
                endPos = ImVec2(centerPoint.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                centerPoint.y - cvars.lineGap + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus - outlineThickness,
                                  (centerPoint.y + 0.51F) - cvars.lineGap + outlineThickness);
                endPos = ImVec2(centerPoint.x + numAdd + outlineThickness,
                                (centerPoint.y + 0.51F) - cvars.lineGap - cvars.lineLength - outlineThickness);
            }
            break;

        case 1:
            drawLine = cvars.enableBottomLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x - halfThickness - outlineThickness,
                                  centerPoint.y + cvars.lineGap - outlineThickness);
                endPos = ImVec2(centerPoint.x + halfThickness + (lineThickness % 2) + outlineThickness,
                                centerPoint.y + cvars.lineGap + cvars.lineLength + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x - numMinus - outlineThickness,
                                  centerPoint.y + cvars.lineGap - outlineThickness);
                endPos = ImVec2(centerPoint.x + numAdd + outlineThickness,
                                centerPoint.y + cvars.lineGap + cvars.lineLength + outlineThickness);
            }
            break;

        case 2:
            drawLine = cvars.enableLeftLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x - cvars.lineGap - cvars.lineLength - outlineThickness,
                                  centerPoint.y - halfThickness - outlineThickness);
                endPos = ImVec2(centerPoint.x - cvars.lineGap + outlineThickness,
                                centerPoint.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2((centerPoint.x + 0.51F) - cvars.lineGap + outlineThickness,
                                  centerPoint.y - numMinus - outlineThickness);
                endPos = ImVec2((centerPoint.x + 0.51F) - cvars.lineGap - cvars.lineLength - outlineThickness,
                                centerPoint.y + numAdd + outlineThickness);
            }
            break;

        case 3:
            drawLine = cvars.enableRightLine;

            if (isEven(lineThickness))
            {
                startPos = ImVec2(centerPoint.x + cvars.lineGap - outlineThickness,
                                  centerPoint.y - halfThickness - outlineThickness);
                endPos = ImVec2(centerPoint.x + cvars.lineGap + cvars.lineLength + outlineThickness,
                                centerPoint.y + halfThickness + (lineThickness % 2) + outlineThickness);
            }
            else
            {
                startPos = ImVec2(centerPoint.x + cvars.lineGap - outlineThickness,
                                  centerPoint.y - numMinus - outlineThickness);
                endPos = ImVec2(centerPoint.x + cvars.lineGap + cvars.lineLength + outlineThickness,
                                centerPoint.y + numAdd + outlineThickness);
            }
        }

        if (drawLine)
            drawList->AddRectFilled(startPos, endPos, cvars.outlineColor);
    }
}

void Crosshair::DrawCenterDot(const ImVec2& centerPoint)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    if (cvars.filledCenterDot)
        drawList->AddCircleFilled(centerPoint, cvars.centerDotSize, cvars.centerDotColor,
                                  cvars.centerDotSegments);
    else
        drawList->AddCircle(centerPoint, cvars.centerDotSize, cvars.centerDotColor,
                            cvars.centerDotSegments, cvars.centerDotThickness);
}

// custom center point stuff

int get_y_level(int minY, int maxY, int currentDuration, int maxDuration)
{
    if (currentDuration <= 0)
    {
        return minY;
    }
    else if (currentDuration >= maxDuration)
    {
        return maxY;
    }
    else
    {
        double percentage = static_cast<double>(currentDuration) / maxDuration;
        return static_cast<int>(minY + percentage * (maxY - minY));
    }
}

double calculate_percentage(int currentValue, int minValue, int maxValue)
{
    if (currentValue <= minValue)
    {
        return 0.0;
    }
    else if (currentValue >= maxValue)
    {
        return 100.0;
    }
    else
    {
        return static_cast<double>(currentValue - minValue) / (maxValue - minValue) * 100.0;
    }
}

void render_charge_bar(double percentage)
{
    auto& allCenterPoints = Crosshair::all_center_points;

    // Ensure percentage is within bounds [0, 1]
    percentage = std::clamp(percentage, 0.0, 1.0);

    // Calculate filled rectangle height based on percentage
    float filledRectHeight = 30.0f * percentage; // Assuming the total height of the bar is 30. Adjust as needed.

    ImGui::GetBackgroundDrawList()->AddRect(
        {allCenterPoints[0].x + cvars.lineGap + cvars.lineLength + 5, allCenterPoints[0].y + 15},
        {allCenterPoints[0].x + cvars.lineGap + cvars.lineLength + 15, allCenterPoints[0].y - 15},
        ImColor(255, 255, 255, 100)
    );

    ImGui::GetBackgroundDrawList()->AddRectFilled(
        {allCenterPoints[0].x + cvars.lineGap + cvars.lineLength + 5, allCenterPoints[0].y + 15},
        {allCenterPoints[0].x + cvars.lineGap + cvars.lineLength + 15, allCenterPoints[0].y + 15 - filledRectHeight},
        ImColor(255, 255, 255, 100)
    );
}

void Crosshair::ModifyDynamicCenterPoint()
{
    static std::chrono::steady_clock::time_point startTime;
    static bool isRButtonDown;

    static double pull_up_time = 0.0;
    static double max_duration = 0.0;

    if (cvars.dynamicCenterPointIndex == 0) // Huntress
    {
        if (pull_up_time != 500)
            pull_up_time = 500;

        if (max_duration != 2000)
            max_duration = 2000;

        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }

            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();

            if (duration > pull_up_time)
            {
                const auto y_level = get_y_level(cvars.true_screen_center_point.y, cvars.true_screen_center_point.y + 30, duration - pull_up_time, max_duration);
                all_center_points[0].y = y_level;

                if (cvars.show_dynamic_charge_bar)
                {
                    const auto percentage = static_cast<double>(duration - pull_up_time) / max_duration; // Assuming max duration is 2000 ms
                    render_charge_bar(percentage);
                }
            }
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            all_center_points[0] = ImVec2(backend::screen_width / 2, backend::screen_height / 2);
        }
    }
    else if (cvars.dynamicCenterPointIndex == 1) // Deathslinger
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
                all_center_points[0].y = 9999; // off screen;
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            all_center_points[0].x = backend::screen_width / 2 + 3;
            all_center_points[0].y = cvars.true_screen_center_point.y;
        }
    }
    else if (cvars.dynamicCenterPointIndex == 2) // Trickster
    {
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }

            if (all_center_points.size() < 2)
                all_center_points.push_back(ImVec2(backend::screen_width / 2, backend::screen_height / 2));

            if (cvars.true_screen_center_point.x - all_center_points[0].x < 15)
            {
                all_center_points[0].x--;
                all_center_points[1].x++;
            }
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;

            if (cvars.true_screen_center_point.x - all_center_points[0].x > 0)
            {
                all_center_points[0].x++;
                all_center_points[1].x--;
            }
            else
            {
                for (int i = 1; i < all_center_points.size(); i++)
                    all_center_points.erase(all_center_points.begin() + i);

                all_center_points[0] = cvars.true_screen_center_point;
            }
        }
    }
    else if (cvars.dynamicCenterPointIndex == 3) // Clown
    {
        if (pull_up_time != 300)
            pull_up_time = 300;

        if (max_duration != 1100)
            max_duration = 1100;

        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (!isRButtonDown)
            {
                startTime = std::chrono::steady_clock::now();
                isRButtonDown = true;
            }
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();

            if (cvars.show_dynamic_charge_bar && duration > pull_up_time)
            {
                const auto percentage = static_cast<double>(duration - pull_up_time) / max_duration;
                render_charge_bar(percentage);
            }
        }
        else
        {
            if (isRButtonDown)
                isRButtonDown = false;
        }
    }
}
