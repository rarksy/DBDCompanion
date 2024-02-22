#include <chrono>
#include <Windows.h>

#include "HookTracker.hpp"
#include "../Dependencies/miscLIB/miscLIB.hpp"

// bool TemplateMatch(cv::Mat Frame, cv::Mat ElementToFind, double Threshold, cv::Point& Detectedlocation)
// {
//     auto result = cv::Mat(Frame.rows - ElementToFind.rows + 1, Frame.cols - ElementToFind.cols + 1, CV_32FC1);
//
//     cv::matchTemplate(Frame, ElementToFind, result, cv::TM_CCOEFF_NORMED);
//     cv::threshold(result, result, Threshold, 1.0, cv::THRESH_TOZERO);
//     double AccuracyValue;
//     cv::minMaxLoc(result, NULL, &AccuracyValue, NULL, &Detectedlocation);
//     return AccuracyValue >= Threshold;
// }
void hook_tracker::setup()
{
    hook_tracker::in_game_ui_scale.load_value();

    for (int i = 0; i < 4; i++)
    {
        hook_tracker::survivor surv;

        surv.index = i;

        if (backend::screen_height == 1440)
        {
            const auto region = hook_tracker::_internal::survivor_regions_1440[i];
            
            surv.location = region;
            surv.size = hook_tracker::_internal::vec2(300, 100);
        }

        hook_tracker::all_survivors.push_back(surv);
    } 
}

void hook_tracker::free()
{
    all_survivors.clear();
}

void hook_tracker::detection_loop()
{
    
}

void hook_tracker::render()
{
    for (int i = 0; i < 4; i++)
    {
        auto surv = hook_tracker::all_survivors[i];
        ImGui::GetBackgroundDrawList()->AddRect(surv.location.to_imvec2(), (surv.location + surv.size).to_imvec2(), ImColor(255, 0, 0));
    }
}
