// miscLIB By Rarksy: a small header-only library containing multiple functions i use in most of my projects

#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <Windows.h>
#include <TlHelp32.h>
#include <fstream>
#include <ctime>

#include "../curl/curl.h"
#include "../nlohmann/json.hpp"

namespace ml
{
    template <typename... Args>
    void log(Args&&... args)
    {
        (std::cout << ... << std::forward<Args>(args)) << std::endl;
    }

    template <typename T>
    std::string to_string(T content)
    {
        return std::to_string(content);
    }

    inline std::string to_string(const char* content)
    {
        return {content};
    }

    template <typename T>
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

    inline bool file_or_directory_exists(const std::string& file_path)
    {
        return std::filesystem::exists(file_path);
    }

    inline bool create_file(const std::string& file_name)
    {
        if (file_or_directory_exists(file_name))
            return false;

        std::ofstream file_to_create(file_name);
        file_to_create.close();

        return file_or_directory_exists(file_name);
    }

    inline bool create_directory(const std::string& directory_path)
    {
        if (file_or_directory_exists(directory_path))
            return false;

        return std::filesystem::create_directories(directory_path);
    }

    inline DWORD get_exe_pid(const std::wstring& processName)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return 0;

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hSnapshot, &pe32))
        {
            CloseHandle(hSnapshot);
            return 0;
        }

        do
        {
            if (processName == pe32.szExeFile)
            {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        }
        while (Process32Next(hSnapshot, &pe32));

        CloseHandle(hSnapshot);
        return 0;
    }

    inline bool is_exe_running(const std::wstring& processName)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE)
            return false;

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hSnapshot, &pe32))
        {
            CloseHandle(hSnapshot);
            return false;
        }

        do
        {
            if (processName == pe32.szExeFile)
            {
                CloseHandle(hSnapshot);
                return true;
            }
        }
        while (Process32Next(hSnapshot, &pe32));

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

        if (OpenClipboard(nullptr))
        {
            HANDLE hClipboardData = GetClipboardData(CF_TEXT);

            if (hClipboardData != nullptr)
            {
                char* clipboardChars = static_cast<char*>(GlobalLock(hClipboardData));

                if (clipboardChars != nullptr)
                {
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
        if (OpenClipboard(nullptr))
        {
            EmptyClipboard();

            HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, input.size() + 1);
            if (hClipboardData != nullptr)
            {
                char* pchData = static_cast<char*>(GlobalLock(hClipboardData));
                if (pchData != nullptr)
                {
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

    inline size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* response)
    {
        response->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    using json = nlohmann::json;

    inline json json_get(const std::string& url)
    {
        curl_global_init(CURL_GLOBAL_ALL);

        CURL* curl = curl_easy_init();
        if (!curl)
            throw std::runtime_error("Failed to initialize curl");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Set a user-agent to avoid potential issues with the server
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.77.0");

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            throw std::runtime_error("Failed to perform curl request: " + std::string(curl_easy_strerror(res)));
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200)
        {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            throw std::runtime_error("HTTP error: " + std::to_string(http_code));
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return json::parse(response);
    }

    inline std::string unix_format_number(int num)
    {
        return (num < 10 ? "0" + std::to_string(num) : std::to_string(num));
    }

    inline std::string unix_get_remaining_time(time_t unix_timestamp)
    {
        time_t currentTime;
        time(&currentTime);

        time_t remainingTime = unix_timestamp - currentTime;
        const time_t remainingDays = remainingTime / (24 * 3600);
        remainingTime = remainingTime % (24 * 3600);
        const time_t remainingHours = remainingTime / 3600;
        remainingTime %= 3600;
        const time_t remainingMinutes = remainingTime / 60;
        remainingTime %= 60;
        const time_t remainingSeconds = remainingTime;

        return std::string(
            unix_format_number(remainingDays) + ":" +
            unix_format_number(remainingHours) + ":" +
            unix_format_number(remainingMinutes) + ":" +
            unix_format_number(remainingSeconds)
        );
    }
}
