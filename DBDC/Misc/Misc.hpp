#pragma once
#include <string>
#include <vector>

namespace Misc
{
    bool IsGameRunning();
    void RestartGame();

    std::vector<std::string> GetAllLibraryDirectories();
    std::string GetGameRootDirectory();

    void OpenSettingsFolder();
}
