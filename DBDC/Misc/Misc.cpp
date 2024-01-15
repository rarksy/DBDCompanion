#include "Misc.hpp"

#include <regex>
#include <Windows.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include "../Config/Config.h"
#include <fstream>

LPCWSTR exe = L"DeadByDaylight-Win64-Shipping.exe";

DWORD GetProcessId(LPCWSTR ProcessName)
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt))
    {
        do
        {
            if (!wcscmp(pt.szExeFile, ProcessName))
            {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        }
        while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}

bool Misc::IsGameRunning()
{
    DWORD pid = GetProcessId(exe);

    return pid != 0;
}

void Misc::RestartGame()
{
    if (IsGameRunning())
    {
        if (MessageBox(nullptr, L"This Will Close And Reopen Dead By Daylight.", L"Continue?", MB_YESNO) == IDNO)
            return;

        HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, GetProcessId(exe));
        if (handle != NULL)
        {
            TerminateProcess(handle, 0);
            CloseHandle(handle);
        }
    }

    ShellExecuteA(NULL, "open", "\"steam://rungameid/381210\"", NULL, NULL, SW_SHOWDEFAULT);
}

cv::Mat Misc::GetScreenshot(const cv::Rect& region, bool grayscale)
{
    const int regionWidth = region.width;
    const int regionHeight = region.height;

    const auto hdc = GetDC(HWND_DESKTOP);

    const auto hbitmap = CreateCompatibleBitmap(hdc, regionWidth, regionHeight);

    const auto memdc = CreateCompatibleDC(hdc);

    const auto oldbmp = SelectObject(memdc, hbitmap);

    BitBlt(memdc, 0, 0, regionWidth, regionHeight, hdc, region.x, region.y, SRCCOPY);
    
    cv::Mat screenshot(regionHeight, regionWidth, CV_8UC4);

    BITMAPINFOHEADER bi = {sizeof(bi), regionWidth, -regionHeight, 1, 32, BI_RGB};
    GetDIBits(hdc, hbitmap, 0, regionHeight, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    
    SelectObject(memdc, oldbmp);
    DeleteDC(memdc);
    DeleteObject(hbitmap);
    ReleaseDC(HWND_DESKTOP, hdc);
    
    if (grayscale)
    {
        cv::Mat grayScreenshot;
        cv::cvtColor(screenshot, grayScreenshot, cv::COLOR_BGR2GRAY);
        return grayScreenshot;
    }

    return screenshot;
}

std::vector<std::string> Misc::GetAllLibraryDirectories()
{
    std::vector<std::string> paths;
    std::ifstream configFile("C:\\Program Files (x86)\\Steam\\steamapps\\libraryfolders.vdf"); // HATE this

    if (configFile.fail())
        return paths;

    std::string line;

    // i have no clue how regular expressions work, thanks chatgpt
    // Define a regex pattern for extracting paths
    std::regex pathPattern("\"path\"\\s+\"([^\"]+)\"");

    while (std::getline(configFile, line))
    {
        std::smatch match;

        // Check if the line matches the pattern
        if (std::regex_search(line, match, pathPattern))
        {
            // Extract the path from the match
            std::string path = match[1].str();

            std::string::size_type pos = 0;
            while ((pos = path.find("\\\\", pos)) != std::string::npos)
            {
                path.replace(pos, 2, "\\");
                pos += 1; // Move past the replaced backslash
            }

            paths.push_back(path);
        }
    }

    return paths;
}


std::string Misc::GetGameRootDirectory()
{
    // library paths are located in:
    // C:\Program Files (x86)\Steam\steamapps/libraryfolders.vdf
    // use it to check all steam libraries for dbd root directory

    const std::vector<std::string> allPaths = GetAllLibraryDirectories();

    for (const auto& path : allPaths)
    {
        std::filesystem::path gameRootDir(path + "\\steamapps\\common\\Dead By Daylight\\");

        if (!std::filesystem::exists(gameRootDir))
            continue;

        return gameRootDir.string();
    }

    return "";
}

void Misc::OpenSettingsFolder()
{
    ShellExecuteA(NULL, "open", Config::SettingsFolderLocation.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
