#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace Misc
{
    bool IsGameRunning();
    void RestartGame();

    cv::Mat GetScreenshot(const cv::Rect& region, bool grayscale = true);

    std::vector<std::string> GetAllLibraryDirectories();
    std::string GetGameRootDirectory();

    void OpenSettingsFolder();
    
}
