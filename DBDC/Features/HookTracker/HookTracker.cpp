#include <chrono>
#include <Windows.h>
#include "HookTracker.hpp"
#include <Backend/Backend.hpp>
#include "Misc/Misc.hpp"
#include "Images/HookTracker/Hook.hpp"

#include "opencv2/xfeatures2d.hpp"

double calculatePercentage(double X, double P)
{
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
            double y = region.y;
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

bool findImage(cv::Mat resource, cv::Mat scene, float min_threshold = 0.7f, int min_matches = 10)
{
    if (resource.empty() || scene.empty())
        throw std::runtime_error("Failed to load detection resources.");

    static int min_hessian = 400;
    static cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(min_hessian);

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    detector->detectAndCompute(resource, cv::noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(scene, cv::noArray(), keypoints2, descriptors2);

    //-- Check if descriptors are empty
    if (descriptors1.empty() || descriptors2.empty())
        return false; // Return false if no descriptors

    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<cv::DMatch>> knn_matches;

    if (descriptors2.rows >= 2)
        matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
    else
        return false;
    
    //-- Filter matches using the Lowe's ratio test
    std::vector<cv::DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i].size() >= 2 && knn_matches[i][0].distance < min_threshold * knn_matches[i][1].distance)
            good_matches.push_back(knn_matches[i][0]);
    }

    return good_matches.size() > min_matches;
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

            if (surv.hook_stage == 3) // dead
                continue;

            const cv::Rect area_to_scan(surv.location.x, surv.location.y, surv.size.x, surv.size.y);
            const auto frame = misc::get_screenshot(area_to_scan, false);

            const bool found_hooked_survivor = findImage(cv::imread("C:\\Users\\rarks\\Downloads\\hook.png"), frame, 0.7F, 6);
            const auto cur_time = std::chrono::steady_clock::now();

            if (found_hooked_survivor && !surv.currently_hooked && cur_time > surv.rehook_time)
            {
                surv.currently_hooked = true;
                surv.hook_stage++;
                surv.rehook_time = cur_time + std::chrono::seconds(61);
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
            ImGui::GetBackgroundDrawList()->AddText(surv.location.to_imvec2(), ImColor(255, 255, 255), "1");
            break;
        case 2:
            ImGui::GetBackgroundDrawList()->AddText(surv.location.to_imvec2(), ImColor(0, 255, 255), "2");
            break;
        }
    }
}
