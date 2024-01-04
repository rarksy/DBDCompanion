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

std::vector<std::string> Misc::GetAllLibraryDirectories()
{
    std::vector<std::string> paths;
    std::ifstream configFile("C:\\Program Files (x86)\\Steam\\steamapps\\libraryfolders.vdf");

    if (configFile.fail())
        return paths;

    std::string line;

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

    
    return "";
}

void Misc::OpenSettingsFolder()
{
    ShellExecuteA(NULL, "open", Config::SettingsFolderLocation.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
