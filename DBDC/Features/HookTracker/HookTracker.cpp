#include <chrono>
#include <Windows.h>

#include "HookTracker.hpp"
#include "HTMenu.h"
#include "Images/HookTracker/Hook.hpp"
#include "Images/HookTracker/Stage2.hpp"
#include "../Backend/Backend.hpp"
#include "../Misc/Misc.hpp"

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