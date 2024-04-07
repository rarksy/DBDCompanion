// miscLIB By Rarksy: a small header-only library containing multiple functions i use in most of my projects

#pragma once
#include "miniz/miniz.h"
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

    inline std::string to_lower(std::string content)
    {
        std::string temp = content;

        std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });

        return temp;
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

    inline std::string wide_str_to_str(const std::wstring& wstr)
    {
        int len;
        int slength = (int)wstr.length() + 1;
        len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, 0, 0); 
        char* buf = new char[len];
        WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, buf, len, 0, 0); 
        std::string r(buf);
        delete[] buf;
        return r;
    }
    
    inline std::string open_file_dialog() {
        OPENFILENAME ofn;
        wchar_t szFile[260];

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        
        if (GetOpenFileName(&ofn) == TRUE) {
            return wide_str_to_str(ofn.lpstrFile);
        } else {
            return "";
        }
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

        const bool created = file_or_directory_exists(file_name);

        return created;
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

    inline nlohmann::json json_get_from_url(const std::string& url)
    {
        curl_global_init(CURL_GLOBAL_ALL);

        CURL* curl = curl_easy_init();
        if (!curl)
            throw std::runtime_error("Failed to initialize curl");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        std::string response;

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

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
            return {};
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return nlohmann::json::parse(response);
    }

    inline int get_seconds_since_file_modified(std::string file_path)
    {
        if (!file_or_directory_exists(file_path))
            return -1;
        
        struct stat result;

        if (stat(file_path.c_str(), &result) == 0)
        {
            time_t mod_time = result.st_mtime;
            time_t current_time = time(nullptr);
            double seconds_since_modification = difftime(current_time, mod_time);

            return static_cast<int>(seconds_since_modification);
        }
        return -1;
    }

    inline bool json_write_data(const std::string& file_path, nlohmann::json json_data)
    {
        std::ofstream file_to_write(file_path);

        if (file_to_write.is_open())
        {
            file_to_write << json_data.dump(4);
            file_to_write.close();

            return true;
        }
        return false;
    }

    inline nlohmann::json json_get_data_from_file(const std::string& file_path)
    {
        std::ifstream file_to_read(file_path);

        if (file_to_read.is_open())
        {
            if (std::filesystem::is_empty(file_path))
                return nullptr;
            
            nlohmann::json data;

            file_to_read >> data;

            file_to_read.close();

            return data;
        }

        return nullptr;
    }

    inline std::string unix_format_number(time_t num)
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

    inline std::string html_formatter(std::string html_content)
    {
        size_t pos = 0;

        // Replace <br> with newline
        while ((pos = html_content.find("<br>", pos)) != std::string::npos)
        {
            html_content.replace(pos, 4, "\n");
            pos += 1;
        }

        pos = 0;
        while ((pos = html_content.find('<'), pos) != std::string::npos)
        {
            size_t end_pos = html_content.find('>', pos);

            if (end_pos != std::string::npos)
                html_content.erase(pos, end_pos - pos + 1);
        }

        // Replace % with %%
        pos = html_content.find('%');
        while (pos != std::string::npos)
        {
            html_content.replace(pos, 1, "%%");
            pos = html_content.find('%', pos + 2); // Move past the inserted "%%"
        }

        return html_content;
    }


    inline bool download_file(const std::string& url, const std::string& file_path)
    {
        CURL* curl = curl_easy_init();
        if (curl)
        {
            // Create directories if they don't exist
            std::filesystem::create_directories(std::filesystem::path(file_path).parent_path());

            FILE* fp;
            errno_t err = fopen_s(&fp, file_path.c_str(), "wb");
            if (err != 0 || !fp)
            {
                std::cerr << "Failed to open file for writing: " << file_path << std::endl;
                return false;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            fclose(fp);

            if (res == CURLE_OK)
            {
                return true;
            }
            else
            {
                remove(file_path.c_str()); // Delete partially downloaded file
                return false;
            }
        }
        return false;
    }

    inline bool extract_file_from_7z(const std::string& archive_path, const std::string& file_path, const std::string& destination)
    {
        std::ifstream file(archive_path, std::ios::binary);
        if (!file) {
            std::cerr << "Error: Could not open archive file." << std::endl;
            return false;
        }

        mz_zip_archive archive;
        mz_bool success = mz_zip_reader_init_file(&archive, archive_path.c_str(), 0);
        if (!success)
        {
            std::cerr << "Error: Failed to initialize archive reader." << std::endl;
            file.close();
            return false;
        }

        int index = mz_zip_reader_locate_file(&archive, file_path.c_str(), nullptr, 0);
        if (index < 0) {
            std::cerr << "Error: File not found in archive." << std::endl;
            mz_zip_reader_end(&archive);
            file.close();
            return false;
        }
        
        size_t uncomp_size;
        void* p = mz_zip_reader_extract_to_heap(&archive, index, &uncomp_size, 0);
        if (!p) {
            std::cerr << "Error: Failed to extract file from archive." << std::endl;
            mz_zip_reader_end(&archive);
            file.close();
            return false;
        }

        std::ofstream outFile(destination, std::ios::binary);
        if (!outFile) {
            std::cerr << "Error: Could not create destination file." << std::endl;
            mz_free(p);
            mz_zip_reader_end(&archive);
            file.close();
            return false;
        }

        outFile.write(reinterpret_cast<const char*>(p), uncomp_size);

        mz_free(p);
        mz_zip_reader_end(&archive);
        file.close();
        outFile.close();

        return true;
    }
}
