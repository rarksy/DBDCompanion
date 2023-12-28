#include "Misc.hpp"
#include <Windows.h>
#include <TlHelp32.h>
#include <shellapi.h>
#include "../Config/Config.h"

LPCWSTR exe = L"DeadByDaylight-Win64-Shipping.exe";

DWORD GetProcessId(LPCWSTR ProcessName)
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!wcscmp(pt.szExeFile, ProcessName)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
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

void Misc::OpenSettingsFolder()
{
    ShellExecuteA(NULL,  "open", Config::SettingsFolderLocation.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
