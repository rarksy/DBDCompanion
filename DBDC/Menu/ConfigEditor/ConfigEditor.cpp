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
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.resolutionQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.viewDistanceQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.antiAliasQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.shadowQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.postProcessQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.textureQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.effectsQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.foliageQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, CEVars.shadingQuality);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.useVSync);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.antiAliasMode);

    if (GetReadOnly(Files::engine))
        CEVars.engineReadOnly = true;

    LoadSettingFind(Files::engine, CEVars.ambientOcclusion);
    LoadSettingFind(Files::engine, CEVars.ambientOcclusionStaticFraction);
    LoadSettingFind(Files::engine, CEVars.bloom);
    LoadSettingFind(Files::engine, CEVars.lensFlare);
    LoadSettingFind(Files::engine, CEVars.motionBlur);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.windowMode);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.resolutionWidth);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.resolutionHeight);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.desiredScreenWidth);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.desiredScreenHeight);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.fpsLimitMode);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.killerFOV);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.terrorRadiusVisual);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.colorBlindMode);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, CEVars.colorBlindModeStrength);

    CEVars.removeIntroCutscene =
        !std::filesystem::exists(
            Misc::GetGameRootDirectory() + Files::AdditionalLoadingScreen
        );

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
    const std::string config = Misc::GetClipboardText();

    for (const auto& c : config) // make sure its actually base64
        if (!(isalnum(c) || c == '+' || c == '/'))
            return false;

    const std::string unbase64 = Misc::Base64Decode(config);
    const std::string decompressed = Misc::DecompressString(unbase64);

    const std::string gameUserSettingsContent = decompressed.substr(0, decompressed.find("ENDFILE"));
    const std::string engineContent = decompressed.substr(decompressed.find("ENDFILE") + 7);

    // explore potential alternative to entire file copy as it will overwrite EVERYthing in the gameusersettings
    
    return true;
}

bool ConfigEditor::LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, int>& setting)
{
    mINI::INIFile file(SettingsFolderLocation.string() + _file);
    mINI::INIStructure ini;

    const bool readSuccess = file.read(ini);

    if (readSuccess)
        setting.second = std::atoi(ini[group][setting.first].c_str());

    return readSuccess;
}

bool ConfigEditor::LoadSetting(const std::string& _file, const std::string& group,
                               std::pair<std::string, std::string>& setting)
{
    mINI::INIFile file(SettingsFolderLocation.string() + _file);
    mINI::INIStructure ini;

    const bool readSuccess = file.read(ini);

    if (readSuccess)
        setting.second = ini[group][setting.first];

    return readSuccess;
}

bool ConfigEditor::LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting)
{
    std::ifstream file(SettingsFolderLocation.string() + _file);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(setting.first + "=") != std::string::npos)
        {
            setting.second = 0;
            return true;
        }
    }

    setting.second = 1;
    return true;
}

bool ConfigEditor::LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting)
{
    std::ifstream file(SettingsFolderLocation.string() + _file);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(setting.first + "=") != std::string::npos)
        {
            setting.second = vFalse;
            return true;
        }
    }

    setting.second = vTrue;
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

bool ConfigEditor::RemoveGroup(std::string _file, std::string group)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    const bool removed = ini.remove(group);

    file.write(ini);

    if (CEVars.engineReadOnly)
        SetReadOnly(_file, true);

    return removed;
}
