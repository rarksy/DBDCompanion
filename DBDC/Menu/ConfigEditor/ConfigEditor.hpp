#pragma once
#include <filesystem>

#define vFalse std::string("False")
#define vTrue std::string("True")

namespace ConfigEditor
{
    bool InitializeConfig();
    void LoadConfig();
    bool CopyConfig(); // copy encoded file to clipboard
    bool ImportConfig(); // decode clipboard text, write to file

    
    bool LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, int>& setting);
    bool LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, std::string>& setting);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting);

    bool SetReadOnly(const std::string& file, const bool value);
    bool GetReadOnly(const std::string& file);

    std::filesystem::path GetSettingsFolderLocation();
    inline std::filesystem::path SettingsFolderLocation;

    bool ChangeValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
    bool ChangeValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting);

    bool RemoveValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
    bool RemoveValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting);

    bool RemoveGroup(std::string _file, std::string group);

    namespace Files
    {
        inline std::string AdditionalLoadingScreen = "DeadByDaylight\\Content\\Movies\\AdditionalLoadingScreen";
        inline std::string gameUserSettings = "\\GameUserSettings.ini";
        inline std::string engine = "\\Engine.ini";
    }

    namespace Groups
    {
        inline std::string scalabilityGroups = "ScalabilityGroups";
        inline std::string DBDGameUserSettings = "/Script/DeadByDaylight.DBDGameUserSettings";
        inline std::string rendererOverrideSettings = "/script/engine.rendereroverridesettings";
        inline std::string Systemsettings = "Systemsettings";
        inline std::string TextureQualityAt = "TextureQuality@";
        inline std::string TextureStreaming = "TextureStreaming";
    }

    struct Variables
    {
        // Variable Name | Value
        using intSetting = std::pair<std::string, int>;
        using stringSetting = std::pair<std::string, std::string>;

        

        intSetting resolutionQuality = {"sg.ResolutionQuality", 100};
        intSetting viewDistanceQuality = {"sg.ViewDistanceQuality", 0};
        intSetting antiAliasQuality = {"sg.AntiAliasingQuality", 0};
        intSetting shadowQuality = {"sg.ShadowQuality", 0};
        intSetting postProcessQuality = {"sg.PostProcessQuality", 0};
        intSetting textureQuality = {"sg.TextureQuality", 0};
        intSetting effectsQuality = {"sg.EffectsQuality", 0};
        intSetting foliageQuality = {"sg.FoliageQuality", 0};
        intSetting shadingQuality = {"sg.ShadingQuality", 0};

        stringSetting useVSync = {"bUseVSync", vTrue};
        intSetting antiAliasMode = {"AntiAliasingMode", 1};

        bool engineReadOnly = false;
        stringSetting ambientOcclusion = {"r.DefaultFeature.AmbientOcclusion", vFalse};
        stringSetting ambientOcclusionStaticFraction = {
            "r.DefaultFeature.AmbientOcclusionStaticFraction", vFalse
        };
        stringSetting bloom = {"r.DefaultFeature.Bloom", vFalse};
        stringSetting lensFlare = {"r.DefaultFeature.LensFlare", vFalse};
        stringSetting motionBlur = {"r.DefaultFeature.MotionBlur", vFalse};

        intSetting windowMode = {"FullscreenMode", 1};
        intSetting desiredScreenWidth = {"DesiredScreenWidth", 1920};
        intSetting desiredScreenHeight = {"DesiredScreenHeight", 1080};
        intSetting resolutionWidth = {"ResolutionSizeX", 1080};
        intSetting resolutionHeight = {"ResolutionSizeY", 1080};
        intSetting fpsLimitMode = {"FPSLimitMode", 120};
        intSetting killerFOV = {"FieldOfView", 67};

        stringSetting terrorRadiusVisual = {"TerrorRadiusVisualFeedback", vFalse};
        intSetting colorBlindMode = {"ColorBlindMode", 0};
        intSetting colorBlindModeStrength = {"ColorBlindModeIntensity", 0};
        
        bool removeIntroCutscene = false;
    };
};

inline ConfigEditor::Variables CEVars;
