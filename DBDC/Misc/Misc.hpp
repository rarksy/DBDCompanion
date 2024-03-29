#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <Windows.h>

namespace misc
{
    void restart_game();

    cv::Mat get_screenshot(const cv::Rect& region, bool grayscale = true);

    std::vector<std::string> get_all_steam_library_directories();
    std::string get_game_root_directory();

    void OpenSettingsFolder();

    bool SetClipboardText(std::string input);
    std::string GetClipboardText();

    std::string Base64Encode(std::string input);
    std::string Base64Decode(std::string input);
    std::string CompressString(const std::string& input);
    std::string DecompressString(const std::string& input);
    
}
