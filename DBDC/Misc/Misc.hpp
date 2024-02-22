#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace misc
{
    void RestartGame(bool dx12 = false);

    cv::Mat GetScreenshot(const cv::Rect& region, bool grayscale = true);

    std::vector<std::string> GetAllLibraryDirectories();
    std::string get_game_root_directory();

    void OpenSettingsFolder();

    bool SetClipboardText(std::string input);
    std::string GetClipboardText();

    std::string Base64Encode(std::string input);
    std::string Base64Decode(std::string input);
    std::string CompressString(const std::string& input);
    std::string DecompressString(const std::string& input);
    
}
