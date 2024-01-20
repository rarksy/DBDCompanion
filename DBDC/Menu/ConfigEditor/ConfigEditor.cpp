#include "ConfigEditor.hpp"
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
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.resolutionQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.viewDistanceQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.antiAliasQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.shadowQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.postProcessQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.textureQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.effectsQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.foliageQuality);
    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.shadingQuality);
    
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.antiAliasMode);
    
    LoadSettingString(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.useVSync);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.antiAliasMode);
    
    if (GetReadOnly(Files::engine))
        CEVars.engineReadOnly = true;
    
    LoadSettingFind(Files::engine, CEVars.ambientOcclusion);
    LoadSettingFind(Files::engine, CEVars.ambientOcclusionStaticFraction);
    LoadSettingFind(Files::engine, CEVars.bloom);
    LoadSettingFind(Files::engine, CEVars.lensFlare);
    LoadSettingFind(Files::engine, CEVars.motionBlur);
    
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.windowMode);
    
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.resolutionWidth);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.resolutionHeight);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.desiredScreenWidth);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.desiredScreenHeight);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.fpsLimitMode);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.killerFOV);
    
    LoadSettingString(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.terrorRadiusVisual);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.colorBlindMode);
    LoadSettingInt(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.colorBlindModeStrength);
    
    CEVars.removeIntroCutscene =
        !std::filesystem::exists(
            Misc::GetGameRootDirectory() + Files::AdditionalLoadingScreen
        );

    LoadSettingBool(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.skipNewsPopup);

    LoadSettingInt(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.resolutionQuality);

    if (Misc::IsGameRunning())
        MessageBoxA(nullptr, "Game Is Running, Changes Won't Apply Until It Is Restarted.", "Notice...", MB_OK);
}

bool ConfigEditor::LoadSettingBool(const std::string& _file, const std::string& group,
    std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>>& setting)
{
    mINI::INIStructure ini;
    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    const bool readSuccess = file.read(ini);

    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    setting.second.first = ini[group][setting.first] == setting.second.second.first;

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return readSuccess;
}

bool ConfigEditor::LoadSettingInt(const std::string& _file, const std::string& group,
                                  std::pair<std::string, int>& setting)
{
    mINI::INIStructure ini;
    mINI::INIFile file(SettingsFolderLocation.string() + _file);
    
    const bool readSuccess = file.read(ini);
    
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);
    
    setting.second = std::atoi(ini[group][setting.first].c_str());
    
    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);
    
    return readSuccess;
}

bool ConfigEditor::LoadSettingString(const std::string& _file, const std::string& group,
    std::pair<std::string, std::string>& setting)
{
    mINI::INIStructure ini;
    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    const bool readSuccess = file.read(ini);

    if (GetReadOnly(_file))
        SetReadOnly(_file, false);
    
    setting.second = ini[group][setting.first];

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return readSuccess;
}

bool ConfigEditor::LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting, bool invertValue)
{
    std::ifstream file(SettingsFolderLocation.string() + _file);
    
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(setting.first + "=") != std::string::npos)
        {
            setting.second = invertValue ? 0 : 1;
            return true;
        }
    }
    
    setting.second = invertValue ? 1 : 0;
    return true;
}

bool ConfigEditor::LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting, bool invertValue)
{
    std::ifstream file(SettingsFolderLocation.string() + _file);
    
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(setting.first + "=") != std::string::npos)
        {
            setting.second = invertValue ? vFalse : vTrue;
            return true;
        }
    }
    
    setting.second = invertValue ? vTrue : vFalse;
    return true;
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
    const std::string config = Misc::GetClipboardText();

    for (const auto& c : config) // make sure the config is actually base64
        if (!(isalnum(c) || c == '+' || c == '/'))
            return false;

    const std::string unbase64 = Misc::Base64Decode(config);
    const std::string decompressed = Misc::DecompressString(unbase64);

    const std::string gameUserSettingsContent = decompressed.substr(0, decompressed.find("ENDFILE"));
    const std::string engineContent = decompressed.substr(decompressed.find("ENDFILE") + 7);

    const std::string tempGameUserSettingsFile = "ImportedGameUserSettings.ini";
    std::ofstream tempFile(tempGameUserSettingsFile);
    tempFile << gameUserSettingsContent;
    tempFile.close();

    mINI::INIStructure importedIni;
    mINI::INIFile importedGameUserSettings(tempGameUserSettingsFile);
    importedGameUserSettings.read(importedIni);

    mINI::INIStructure ini;
    mINI::INIFile gameUserSettings(SettingsFolderLocation.string() + Files::gameUserSettings);
    gameUserSettings.read(ini);

    ini[Groups::scalabilityGroups][CEVars.resolutionQuality.first] = importedIni[Groups::scalabilityGroups][CEVars.resolutionQuality.first];

    if (GetReadOnly(Files::gameUserSettings))
        SetReadOnly(Files::gameUserSettings, false);
    
    gameUserSettings.write(ini);
    
    LoadConfig();

    if (CEVars.engineReadOnly)
        SetReadOnly(Files::gameUserSettings, true);

    std::remove(tempGameUserSettingsFile.c_str());
    
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

bool ConfigEditor::ChangeValue(std::string _file, std::string group, std::pair<std::string, int> intSetting)
{
    return ChangeValue(_file, group, std::pair(intSetting.first, std::to_string(intSetting.second)));
}

bool ConfigEditor::ChangeValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    ini[group][stringSetting.first] = stringSetting.second;

    bool writeSuccess = file.write(ini);

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return writeSuccess;
}

bool ConfigEditor::ChangeValue(std::string _file, std::string group,
    std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>> boolSetting)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    if (boolSetting.second.first)
        ini[group][boolSetting.first] = boolSetting.second.second.first;
    else ini[group][boolSetting.first] = boolSetting.second.second.second;

    bool writeSuccess = file.write(ini);

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return writeSuccess;
}

bool ConfigEditor::RemoveValue(std::string _file, std::string group, std::pair<std::string, int> intSetting)
{
    return RemoveValue(_file, group, std::pair(intSetting.first, std::to_string(intSetting.second)));
}

bool ConfigEditor::RemoveValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    bool removed = ini[group].remove(stringSetting.first);

    file.write(ini);

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return removed;
}
