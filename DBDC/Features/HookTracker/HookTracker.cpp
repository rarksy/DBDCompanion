#include <chrono>
#include <Windows.h>
#include "HookTracker.hpp"
#include "../Backend/Backend.hpp"
#include "../Misc/Misc.hpp"
#include "Images/HookTracker/Hook.hpp"

double calculatePercentage(double X, double P) {
    return X * (P / 100.0); 
}
void hook_tracker::setup()
{
    in_game_ui_scale.load_value();
    const double resize_percentage = 1.0 + (100.0 - (double)in_game_ui_scale.value / 100);

    for (int i = 0; i < 4; i++)
    {
        hook_tracker::survivor surv;

        surv.index = i;

        if (backend::screen_height == 1440)
        {
            const auto region = hook_tracker::_internal::survivor_regions_1440[i];
            double y = region.y * resize_percentage;
            surv.location.x = region.x;
            surv.location.y = y;
            surv.size = hook_tracker::_internal::vec2(300, 100);
        }

        hook_tracker::all_survivors.push_back(surv);
    }
}

void hook_tracker::free()
{
    all_survivors.clear();
}

bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
{
    auto result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);

    cv::matchTemplate(Frame, ElementToFind, result, cv::TM_CCOEFF_NORMED);
    cv::threshold(result, result, Threshold, 1.0, cv::THRESH_TOZERO);
    double AccuracyValue;
    cv::minMaxLoc(result, NULL, &AccuracyValue, NULL, &Detectedlocation);
    return AccuracyValue >= Threshold;
}

void hook_tracker::detection_loop()
{
    using hook_tracker::_internal::vec2;

    cv::Mat hook_image;

    {
        const std::vector hook_data(hookRawData, hookRawData + sizeof hookRawData);
        hook_image = cv::imdecode(hook_data, cv::IMREAD_GRAYSCALE);
    }

    while (ht_vars::enabled)
    {
        for (int i = 0; i < 4; i++)
        {
            auto& surv = all_survivors[i];
            const cv::Rect area_to_scan(surv.location.x, surv.location.y, surv.size.x, surv.size.y);
            const auto frame = misc::get_screenshot(area_to_scan);

            cv::Point detected_location;
            const bool found_hooked_survivor = TemplateMatch(frame, hook_image, 0.9, detected_location);
            
            if (found_hooked_survivor && !surv.currently_hooked)
            {
                surv.currently_hooked = true;
                surv.hook_stage++;
            }
            else if (!found_hooked_survivor && surv.currently_hooked)
                surv.currently_hooked = false;
        }
    }
}

void hook_tracker::render()
{
    for (int i = 0; i < 4; i++)
    {
        const auto surv = hook_tracker::all_survivors[i];
        ImGui::GetBackgroundDrawList()->AddRect(surv.location.to_imvec2(), (surv.location + surv.size).to_imvec2(), ImColor(255, 0, 0));

        switch (surv.hook_stage)
        {
        default:
            break;

        case 1:
            ImGui::GetBackgroundDrawList()->AddText(surv.location.to_imvec2(), ImColor(255, 0, 0), "1");
            break;
        case 2:
            ImGui::GetBackgroundDrawList()->AddText(surv.location.to_imvec2(), ImColor(255, 0, 0), "2");
            break;
        }
    }
}
