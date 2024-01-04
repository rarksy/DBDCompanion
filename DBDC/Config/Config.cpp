#include "Config.h"
#include <ShlObj_core.h>
#include "mINI/ini.h"

bool Config::InitializeConfig()
{
    const auto path = GetSettingsFolderLocation();

    if (exists(path))
    {
        SettingsFolderLocation = path;
        return true;
    }

    const auto result = MessageBoxA(
        nullptr, "Un able To Locate Settings Folder\nPlease Locate Manually Or Press No To Exit.", "Notice...", MB_YESNO);

    if (result == IDNO)
        exit(1);
    
    {
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
    }

    return std::filesystem::exists(SettingsFolderLocation.string() + "\\GameUserSettings.ini");
}

bool Config::LoadConfig()
{
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::resolutionQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::viewDistanceQuality);

    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::antiAliasQuality);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::antiAliasMode);

    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::shadowQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::postProcessQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::textureQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::effectsQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::foliageQuality);
    LoadSetting(Files::gameUserSettings, Groups::scalabilityGroups, Variables::shadingQuality);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::windowMode);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::resolutionWidth);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::resolutionHeight);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::desiredScreenWidth);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::desiredScreenHeight);

    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::fpsLimitMode);
    LoadSetting(Files::gameUserSettings, Groups::DBDGameUserSettings, Variables::useVSync);

    if (GetReadOnly(Files::engine))
        Variables::engineReadOnly = true;

    LoadSettingFind(Files::engine, Variables::ambientOcclusion);
    LoadSettingFind(Files::engine, Variables::ambientOcclusionStaticFraction);
    LoadSettingFind(Files::engine, Variables::bloom);
    LoadSettingFind(Files::engine, Variables::lensFlare);
    LoadSettingFind(Files::engine, Variables::motionBlur);

    return true;
}

bool Config::LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, int>& setting)
{
    mINI::INIFile file(SettingsFolderLocation.string() + _file);
    mINI::INIStructure ini;

    const bool readSuccess = file.read(ini);

    if (readSuccess)
        setting.second = std::atoi(ini[group][setting.first].c_str());

    return readSuccess;
}

bool Config::LoadSetting(const std::string& _file, const std::string& group,
                         std::pair<std::string, std::string>& setting)
{
    mINI::INIFile file(SettingsFolderLocation.string() + _file);
    mINI::INIStructure ini;

    const bool readSuccess = file.read(ini);

    if (readSuccess)
        setting.second = ini[group][setting.first];

    return readSuccess;
}

bool Config::LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting)
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

bool Config::LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting)
{
    std::ifstream file(SettingsFolderLocation.string() + _file);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.find(setting.first + "=") != std::string::npos)
        {
            setting.second = sFalse;
            return true;
        }
    }

    setting.second = sTrue;
    return true;
}

bool Config::SetReadOnly(const std::string& file, const bool value)
{
    DWORD attributes = GetFileAttributesA((SettingsFolderLocation.string() + file).c_str());

    if (value)
        attributes |= FILE_ATTRIBUTE_READONLY;
    else
        attributes &= ~FILE_ATTRIBUTE_READONLY;

    return SetFileAttributesA((SettingsFolderLocation.string() + file).c_str(), attributes);
}

bool Config::GetReadOnly(const std::string& file)
{
    const bool readOnly = GetFileAttributesA((SettingsFolderLocation.string() + file).c_str()) &
        FILE_ATTRIBUTE_READONLY;

    return readOnly;
}

std::filesystem::path Config::GetSettingsFolderLocation()
{
    auto localAppDataPath = std::filesystem::temp_directory_path().parent_path().parent_path();

    localAppDataPath /= "DeadByDaylight\\Saved\\Config\\WindowsClient\\";

    return localAppDataPath;
}

bool Config::Edit::ChangeValue(std::string _file, std::string group, std::pair<std::string, int> intSetting)
{
    return ChangeValue(_file, group, std::pair(intSetting.first, std::to_string(intSetting.second)));
}

bool Config::Edit::ChangeValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    ini[group][stringSetting.first] = stringSetting.second;

    bool writeSuccess = file.write(ini);

    if (Variables::engineReadOnly)
        SetReadOnly(_file, true);

    return writeSuccess;
}

bool Config::Edit::RemoveValue(std::string _file, std::string group, std::pair<std::string, int> intSetting)
{
    return RemoveValue(_file, group, std::pair(intSetting.first, std::to_string(intSetting.second)));
}

bool Config::Edit::RemoveValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    bool removed = ini[group].remove(stringSetting.first);

    file.write(ini);

    if (Variables::engineReadOnly)
        SetReadOnly(_file, true);

    return removed;
}

bool Config::Edit::RemoveGroup(std::string _file, std::string group)
{
    if (GetReadOnly(_file))
        SetReadOnly(_file, false);

    mINI::INIFile file(SettingsFolderLocation.string() + _file);

    mINI::INIStructure ini;

    file.read(ini);

    const bool removed = ini.remove(group);

    file.write(ini);

    if (Variables::engineReadOnly)
        SetReadOnly(_file, true);

    return removed;
}
