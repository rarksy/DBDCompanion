// miscLIB By Rarksy: a small header-only library containing multiple functions i use in most of my projects

#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <Windows.h>
#include <TlHelp32.h>

namespace ml
{
    template <typename... Args>
    void log(Args&&... args)
    {
        (std::cout << ... << std::forward<Args>(args)) << std::endl;
    }

    template<typename T>
    std::string to_string(T content)
    {
        return std::to_string(content);
    }
    
    inline std::string to_string(const char* content)
    {
        return {content};
    }

    template<typename T>
    std::vector<std::string> split_to_vector(T content, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(ml::to_string(content));
        std::string token;

        while (std::getline(ss, token, delimiter))
            tokens.push_back(token);

        return tokens;
    }

    inline std::filesystem::path get_exe_directory()
    {
        char pathBuffer[MAX_PATH];
        GetModuleFileNameA(NULL, pathBuffer, MAX_PATH);
        
        return std::filesystem::path(pathBuffer);
    }

    inline bool file_exists(const std::string& file_path)
    {
        return std::filesystem::exists(file_path);
    }

    inline bool create_file(const std::string& file_name)
    {
        if (file_exists(file_name))
            return false;
        
        std::ofstream file_to_create(file_name);
        file_to_create.close();

        return file_exists(file_name);
    }

    inline bool is_exe_running(const std::wstring& processName) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return false;

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hSnapshot, &pe32)) {
            CloseHandle(hSnapshot);
            return false;
        }

        do {
            if (processName == pe32.szExeFile) {
                CloseHandle(hSnapshot);
                return true;
            }
        } while (Process32Next(hSnapshot, &pe32));

        CloseHandle(hSnapshot);
        return false;
    }

    inline int get_screen_width()
    {
        return GetSystemMetrics(SM_CXSCREEN);
    }

    inline int get_screen_height()
    {
        return GetSystemMetrics(SM_CYSCREEN);
    }

    inline std::string get_clipboard_text()
    {
        std::string clipboardText;

        if (OpenClipboard(nullptr)) {
            HANDLE hClipboardData = GetClipboardData(CF_TEXT);

            if (hClipboardData != nullptr) {
                char* clipboardChars = static_cast<char*>(GlobalLock(hClipboardData));
            
                if (clipboardChars != nullptr) {
                    clipboardText = clipboardChars;
                    GlobalUnlock(hClipboardData);
                }
            }

            CloseClipboard();
        }

        return clipboardText;
    }

    inline void set_clipboard_text(const std::string& input)
    {
        if (OpenClipboard(nullptr)) {
            EmptyClipboard();

            HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, input.size() + 1);
            if (hClipboardData != nullptr) {
                char* pchData = static_cast<char*>(GlobalLock(hClipboardData));
                if (pchData != nullptr) {
                    strcpy_s(pchData, input.size() + 1, input.c_str());
                    GlobalUnlock(hClipboardData);

                    SetClipboardData(CF_TEXT, hClipboardData);
                    CloseClipboard();

                    return;
                }
            }

            CloseClipboard();
        }
    }

    inline void open_directory(const std::string& dir)
    {
        ShellExecuteA(NULL, "open", dir.c_str(), NULL, NULL, SW_SHOWDEFAULT);
    }

    inline void run_steam_gameid(std::string appid)
    {
        std::string runString = "steam://rungameid/" + appid;

        open_directory(runString);
    }

}
