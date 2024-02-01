#include "ConfigEditor.hpp"

#include <regex>

#include "../Misc/Misc.hpp"
#include "mINI/ini.h"

#include <ShlObj_core.h>

bool ConfigEditor::InitializeConfig()
{
    const auto path = GetSettingsFolderLocation();

    if (exists(path))
    {
        SettingsFolderLocation = path;
        return true;
    }

    const auto result = MessageBoxA(
        nullptr, "Unable To Locate Settings Folder\nPlease Locate Manually Or Press No To Exit.", "Notice...",
        MB_YESNO);

    if (result == IDNO)
        exit(1);


    CoInitialize(NULL);
    IFileDialog* pfd;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {
        DWORD dwOptions;
        if (FAILED(pfd->GetOptions(&dwOptions)))
            return false;

        pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

        if (FAILED(pfd->Show(NULL)))
        {
            pfd->Release();
            return false;
        }

        IShellItem* psi;
        if (FAILED(pfd->GetResult(&psi)))
        {
            psi->Release();
            pfd->Release();
            return false;
        }

        PWSTR pszPath;
        if (FAILED(psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath)))
        {
            psi->Release();
            pfd->Release();
        }

        SettingsFolderLocation = pszPath;
        // Add your code here to work with the selected folder path
        CoTaskMemFree(pszPath);

        psi->Release();
        pfd->Release();
    }

    CoUninitialize();


    return std::filesystem::exists(SettingsFolderLocation.string() + Files::gameUserSettings);
}

void ConfigEditor::LoadConfig()
{
    CEVars.resolutionQuality.LoadValue();
    CEVars.viewDistanceQuality.LoadValue();
    CEVars.antiAliasQuality.LoadValue();
    CEVars.shadowQuality.LoadValue();
    CEVars.postProcessQuality.LoadValue();
    CEVars.textureQuality.LoadValue();
    CEVars.effectsQuality.LoadValue();
    CEVars.foliageQuality.LoadValue();
    CEVars.shadingQuality.LoadValue();
    CEVars.animationQuality.LoadValue();

    CEVars.useVSync.LoadValue();
    CEVars.antiAliasMode.LoadValue();
    CEVars.ambientOcclusion.LoadValue();
    CEVars.ambientOcclusionStaticFraction.LoadValue();
    CEVars.bloom.LoadValue();
    CEVars.lensFlare.LoadValue();
    CEVars.motionBlur.LoadValue();

    CEVars.windowMode.LoadValue();
    CEVars.skipNewsPopup.LoadValue();

    CEVars.terrorRadiusVisual.LoadValue();
    CEVars.colorBlindMode.LoadValue();
    CEVars.colorBlindModeStrength.LoadValue();
    

    if (Misc::IsGameRunning())
        MessageBoxA(nullptr, "Game Is Running, Changes Won't Apply Until It Is Restarted.", "Notice...", MB_OK);
}

bool ConfigEditor::CopyConfig()
{
    std::ifstream file(SettingsFolderLocation.string() + Files::gameUserSettings, std::ios::binary);

    if (!file.is_open())
        return false;

    std::ostringstream ossGameUserSettings;
    ossGameUserSettings << file.rdbuf();
    file.close();

    const std::string gameUserSettingsContent = ossGameUserSettings.str();

    file.open(SettingsFolderLocation.string() + Files::engine, std::ios::binary);
    if (!file.is_open())
        return false;

    std::ostringstream ossEngine;
    ossEngine << file.rdbuf();
    file.close();

    const std::string engineContent = ossEngine.str();

    const std::string combinedContent = gameUserSettingsContent + "ENDFILE" + engineContent;

    const std::string compressed = Misc::CompressString(combinedContent);

    const std::string tobase64 = Misc::Base64Encode(compressed);

    Misc::SetClipboardText(tobase64);

    return true;
}

bool ConfigEditor::ImportConfig()
{
    const auto result = MessageBoxA(NULL, "This Will Replace Your Current Companion Settings, Continue?", "Notice...", MB_YESNO);

    if (result == IDNO)
        return false;
    
    const std::string config = Misc::GetClipboardText();

    std::regex base64Regex("^[A-Za-z0-9+/]*={0,2}$"); // still dont know how regex works, thanks stackoverflow
    if (!std::regex_match(config, base64Regex))
    {
        MessageBoxA(NULL, "Invalid Config", "Error...", MB_OK);
        return false;
    }

    const std::string unbase64 = Misc::Base64Decode(config);
    const std::string decompressed = Misc::DecompressString(unbase64);

    const std::string gameUserSettingsContent = decompressed.substr(0, decompressed.find("ENDFILE"));
    const std::string engineContent = decompressed.substr(decompressed.find("ENDFILE") + 7);

    // Setup Temp Files
    
    const std::string tempGameUserSettings = "ImportedGameUserSettings.ini";
    std::ofstream tempGameUserSettingsFile(tempGameUserSettings);
    tempGameUserSettingsFile << gameUserSettingsContent;
    tempGameUserSettingsFile.close();

    const std::string tempEngine = "ImportedEngine.ini";
    std::ofstream tempEngineFile(tempEngine);
    tempEngineFile << engineContent;
    tempEngineFile.close();

    // Setup Imported INI Structures
    
    mINI::INIStructure importedGameUserSettingsIni;
    mINI::INIFile importedGameUserSettings(tempGameUserSettings);
    importedGameUserSettings.read(importedGameUserSettingsIni);
    
    mINI::INIStructure importedEngineIni;
    mINI::INIFile importedEngine(tempEngine);
    importedEngine.read(importedEngineIni);

    // Setup Main INI Structures

    CEVars.resolutionQuality.ImportValue(importedGameUserSettings);
    CEVars.viewDistanceQuality.ImportValue(importedGameUserSettings);
    CEVars.antiAliasQuality.ImportValue(importedGameUserSettings);
    CEVars.shadowQuality.ImportValue(importedGameUserSettings);
    CEVars.postProcessQuality.ImportValue(importedGameUserSettings);
    CEVars.textureQuality.ImportValue(importedGameUserSettings);
    CEVars.effectsQuality.ImportValue(importedGameUserSettings);
    CEVars.foliageQuality.ImportValue(importedGameUserSettings);
    CEVars.shadingQuality.ImportValue(importedGameUserSettings);
    CEVars.animationQuality.ImportValue(importedGameUserSettings);

    CEVars.useVSync.ImportValue(importedGameUserSettings);
    CEVars.antiAliasMode.ImportValue(importedGameUserSettings);
    CEVars.ambientOcclusion.ImportValue(importedEngine);
    CEVars.ambientOcclusionStaticFraction.ImportValue(importedEngine);
    CEVars.bloom.ImportValue(importedEngine);
    CEVars.lensFlare.ImportValue(importedEngine);
    CEVars.motionBlur.ImportValue(importedEngine);

    
    CEVars.windowMode.ImportValue(importedGameUserSettings);
    CEVars.resolutionWidth.ImportValue(importedGameUserSettings);
    CEVars.resolutionHeight.ImportValue(importedGameUserSettings);
    CEVars.fpsLimitMode.ImportValue(importedGameUserSettings);
    CEVars.killerFOV.ImportValue(importedGameUserSettings);

    CEVars.removeIntroCutscene = std::filesystem::exists(Misc::GetGameRootDirectory() + "DeadByDaylight\\Content\\Movies\\" + "disabled_AdditionalLoadingScreen");
    CEVars.skipNewsPopup.ImportValue(importedGameUserSettings);

    CEVars.terrorRadiusVisual.ImportValue(importedGameUserSettings);
    CEVars.colorBlindMode.ImportValue(importedGameUserSettings);
    CEVars.colorBlindModeStrength.ImportValue(importedGameUserSettings);
    
    LoadConfig();
    
    std::remove(tempGameUserSettings.c_str());
    std::remove(tempEngine.c_str());

    return true;
}

bool ConfigEditor::SetReadOnly(const std::string& file, const bool value)
{
    DWORD attributes = GetFileAttributesA((SettingsFolderLocation.string() + file).c_str());

    if (value)
        attributes |= FILE_ATTRIBUTE_READONLY;
    else
        attributes &= ~FILE_ATTRIBUTE_READONLY;

    return SetFileAttributesA((SettingsFolderLocation.string() + file).c_str(), attributes);
}

bool ConfigEditor::GetReadOnly(const std::string& file)
{
    return GetFileAttributesA((SettingsFolderLocation.string() + file).c_str()) &
        FILE_ATTRIBUTE_READONLY;
}

std::filesystem::path ConfigEditor::GetSettingsFolderLocation()
{
    auto localAppDataPath = std::filesystem::temp_directory_path().parent_path().parent_path();

    localAppDataPath /= "DeadByDaylight\\Saved\\Config\\WindowsClient\\";

    return localAppDataPath;
}