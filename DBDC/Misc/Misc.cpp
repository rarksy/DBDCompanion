#include "Misc.hpp"
#include "../Menu/ConfigEditor/ConfigEditor.hpp"

#include <regex>
#include <Windows.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include <fstream>
#include <filesystem>

#include "Base64/Base64.hpp"
#include "zlib/zlib.h"

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

void Misc::RestartGame(bool dx12)
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

    std::string renderApi = dx12 ? " -DX12" : "";
    std::string runString = "steam://rungameid/381210" + renderApi;

    ShellExecuteA(NULL, "open", runString.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

cv::Mat Misc::GetScreenshot(const cv::Rect& region, bool grayscale)
{
    const int regionWidth = region.width;
    const int regionHeight = region.height;

    const auto hdc = GetDC(HWND_DESKTOP);

    std::unique_ptr<HDC__, decltype(&DeleteDC)> memdc(CreateCompatibleDC(hdc), DeleteDC);
    std::unique_ptr<HBITMAP__, decltype(&DeleteObject)> bitmap(CreateCompatibleBitmap(hdc, regionWidth, regionHeight), DeleteObject);

    const auto oldbmp = SelectObject(memdc.get(), bitmap.get());
    BitBlt(memdc.get(), 0, 0, regionWidth, regionHeight, hdc, region.x, region.y, SRCCOPY);
    
    cv::Mat screenshot(regionHeight, regionWidth, CV_8UC4);

    BITMAPINFOHEADER bi = {sizeof(bi), regionWidth, -regionHeight, 1, 32, BI_RGB};
    GetDIBits(hdc, bitmap.get(), 0, regionHeight, screenshot.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    
    SelectObject(memdc.get(), oldbmp);
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
    ShellExecuteA(NULL, "open", ConfigEditor::SettingsFolderLocation.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}

bool Misc::SetClipboardText(std::string input)
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

                return true;
            }
        }

        CloseClipboard();
    }

    return false;
}

std::string Misc::GetClipboardText()
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

std::string Misc::Base64Encode(std::string input)
{
    return base64::to_base64(input);
}

std::string Misc::Base64Decode(std::string input)
{
    return base64::from_base64(input);
}

std::string Misc::CompressString(const std::string& input)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)input.data();
    zs.avail_in = input.size();

    int ret;
    char outbuffer[10240];
    std::string outstring;
    
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

std::string Misc::DecompressString(const std::string& input)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)input.data();
    zs.avail_in = input.size();

    int ret;
    const int CHUNK_SIZE = 1024;
    std::vector<char> outbuffer(CHUNK_SIZE);

    std::string outstring;

    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
        zs.avail_out = CHUNK_SIZE;

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer.data(), zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}
