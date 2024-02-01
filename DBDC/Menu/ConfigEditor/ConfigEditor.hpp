#pragma once
#include <filesystem>
#include "mINI/ini.h"

namespace ConfigEditor
{
    bool InitializeConfig();
    void LoadConfig();

    bool SetReadOnly(const std::string& file, const bool value);
    bool GetReadOnly(const std::string& file);

    std::filesystem::path GetSettingsFolderLocation();
    inline std::filesystem::path SettingsFolderLocation;

    namespace Files
    {
        inline std::string AdditionalLoadingScreen = "DeadByDaylight\\Content\\Movies\\AdditionalLoadingScreen";
        inline std::string gameUserSettings = "\\GameUserSettings.ini";
        inline std::string engine = "\\Engine.ini";
    }

    namespace Sections
    {
        inline std::string scalabilityGroups = "ScalabilityGroups";
        inline std::string DBDGameUserSettings = "/Script/DeadByDaylight.DBDGameUserSettings";
        inline std::string rendererOverrideSettings = "/Script/Engine.RendererOverrideSettings";
    }

    struct Setting
    {
        std::string file;
        std::string section;
        std::string variable;
        int value;

        Setting(std::string _file, std::string _section, std::string _variable, int _value)
            : file(_file), section(_section), variable(_variable), value(_value)
        {
        }
        ~Setting(){}

        bool ImportValue(const mINI::INIFile& _file)
        {
            mINI::INIStructure importedINI;
            _file.read(importedINI);
            
            mINI::INIFile localINIFile(SettingsFolderLocation.string() + this->file);
            mINI::INIStructure localINI;
            localINIFile.read(localINI);

            localINI[this->section][this->variable] = importedINI[this->section][this->variable];

            if (GetReadOnly(this->file))
                SetReadOnly(this->file, false);

            const bool writeSuccess = localINIFile.write(localINI, true);

            SetReadOnly(this->file, true);

            return writeSuccess;
        }

        bool LoadValue()
        {
            mINI::INIFile iniFile(SettingsFolderLocation.string() + this->file);
            mINI::INIStructure ini;

            const bool readSuccess = iniFile.read(ini);
            
            if (!readSuccess)
                return false;

            std::string loadedValueString = ini[this->section][this->variable];
            std::transform(loadedValueString.begin(), loadedValueString.end(), loadedValueString.begin(), tolower);

            if (loadedValueString.empty())
                return false;
            
            if (loadedValueString == "true" || loadedValueString == "false")
            {
                this->value = loadedValueString == "true";
                return true;
            }

            this->value = std::stoi(loadedValueString);
            return true;
        }
        
        bool SetValue()
        {
            mINI::INIFile iniFile(SettingsFolderLocation.string() + this->file);
            mINI::INIStructure ini;

            iniFile.read(ini);

            ini[this->section][this->variable] = std::to_string(this->value);

            if (GetReadOnly(this->file))
                SetReadOnly(this->file, false);

            const bool writeSuccess = iniFile.write(ini);

            SetReadOnly(this->file, true);

            return writeSuccess;
        }

        bool RemoveValue()
        {
            mINI::INIFile iniFile(SettingsFolderLocation.string() + this->file);
            mINI::INIStructure ini;

            iniFile.read(ini);

            ini[this->section].remove(this->variable);

            if (GetReadOnly(this->file))
                SetReadOnly(this->file, false);

            const bool writeSuccess = iniFile.write(ini);

            SetReadOnly(this->file, true);

            return writeSuccess;
        }
    };

    class Variables
    {
    public:
        // Graphics Quality
        Setting resolutionQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.ResolutionQuality", 100);
        Setting viewDistanceQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.ViewDistanceQuality", 0);
        Setting antiAliasQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.AntiAliasingQuality", 0);
        Setting shadowQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.ShadowQuality", 0);
        Setting postProcessQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.PostProcessQuality", 0);
        Setting textureQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.TextureQuality", 0);
        Setting effectsQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.EffectsQuality", 0);
        Setting foliageQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.FoliageQuality", 0);
        Setting shadingQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.ShadingQuality", 0);
        Setting animationQuality = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.AnimationQuality", 0);
        
        // Rendering
        Setting useVSync = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "bUseVSync", 1);
        Setting antiAliasMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "AntiAliasingMode", 1);
        
        Setting ambientOcclusion = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.AmbientOcclusion", 1);
        Setting ambientOcclusionStaticFraction = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.AmbientOcclusionStaticFraction", 1);
        Setting bloom = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.Bloom", 1);
        Setting lensFlare = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.LensFlare", 1);
        Setting motionBlur = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.MotionBlur", 1);
        
        // Misc
        Setting windowMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "FullscreenMode", 1);
        Setting desiredScreenWidth = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "DesiredScreenWidth", 1920);
        Setting desiredScreenHeight = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "DesiredScreenHeight", 1080);
        Setting resolutionWidth = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ResolutionSizeX", 1920);
        Setting resolutionHeight = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ResolutionSizeY", 1080);
        Setting fpsLimitMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "FPSLimitMode", 60);
        Setting killerFOV = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "FieldOfView", 87);
        bool removeIntroCutscene = false;
        Setting skipNewsPopup = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "HighestWeightSeenNews", 0);
        
        // Accessibility
        Setting terrorRadiusVisual = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "TerrorRadiusVisualFeedback", 0);
        Setting colorBlindMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ColorBlindMode", 0);
        Setting colorBlindModeStrength = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ColorBlindModeIntensity", 0);
    };


    bool CopyConfig();
    bool ImportConfig();
};

inline ConfigEditor::Variables CEVars;
