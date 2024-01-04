#pragma once
#include <filesystem>
#include <string>

#define sFalse std::string("False")
#define sTrue std::string("True")

namespace Config
{
    bool InitializeConfig();
    bool LoadConfig();
    bool LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, int>& setting);
    bool LoadSetting(const std::string& _file, const std::string& group, std::pair<std::string, std::string>& setting);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting);

    bool SetReadOnly(const std::string& file, const bool value);
    bool GetReadOnly(const std::string& file);

    std::filesystem::path GetSettingsFolderLocation();

    namespace Edit
    {
        bool ChangeValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
        bool ChangeValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting);

        bool RemoveValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
        bool RemoveValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting);

        bool RemoveGroup(std::string _file, std::string group);
    }

    inline std::filesystem::path SettingsFolderLocation;

    namespace Groups
    {
        inline std::string scalabilityGroups = "ScalabilityGroups";
        inline std::string DBDGameUserSettings = "/Script/DeadByDaylight.DBDGameUserSettings";
        inline std::string rendererOverrideSettings = "/script/engine.rendereroverridesettings";
        inline std::string Systemsettings = "Systemsettings";
        inline std::string TextureQualityAt = "TextureQuality@";
        inline std::string TextureStreaming = "TextureStreaming";
    }

    namespace Variables
    {
        // Variable Name | Value
        using intSetting = std::pair<std::string, int>;
        using stringSetting = std::pair<std::string, std::string>;

        inline intSetting resolutionQuality = {"sg.ResolutionQuality", 100};
        inline intSetting viewDistanceQuality = {"sg.ViewDistanceQuality", 0};
        inline intSetting antiAliasQuality = {"sg.AntiAliasingQuality", 0};
        inline intSetting shadowQuality = {"sg.ShadowQuality", 0};
        inline intSetting postProcessQuality = {"sg.PostProcessQuality", 0};
        inline intSetting textureQuality = {"sg.TextureQuality", 0};
        inline intSetting effectsQuality = {"sg.EffectsQuality", 0};
        inline intSetting foliageQuality = {"sg.FoliageQuality", 0};
        inline intSetting shadingQuality = {"sg.ShadingQuality", 0};

        inline intSetting windowMode = {"FullscreenMode", 1};
        inline intSetting desiredScreenWidth = {"DesiredScreenWidth", 1920};
        inline intSetting desiredScreenHeight = {"DesiredScreenHeight", 1080};
        inline intSetting resolutionWidth = {"ResolutionSizeX", 1080};
        inline intSetting resolutionHeight = {"ResolutionSizeY", 1080};

        // 
        inline intSetting fpsLimitMode = {"FPSLimitMode", 120};
        inline stringSetting useVSync = {"bUseVSync", sTrue};
        inline intSetting antiAliasMode = {"AntiAliasingMode", 1};

        namespace TextureStreaming
        {
            inline stringSetting useTextureStreaming = {"bUseTextureStreaming", sFalse};
            inline intSetting gpuLUIDLow = {"r.GPULUIDLow", 68947};
            inline intSetting gpuLUIDHigh = {"r.GPULUIDHigh", 0};
            inline intSetting textureStreaming = {"r.TextureStreaming", 0};
            inline intSetting poolSize = {"r.Streaming.PoolSize", 0};
            inline intSetting hlodStrategy = {"r.Streaming.HLODStrategy", 2};

            inline stringSetting allowStreaming = {"AllowStreaming", sFalse};

            inline intSetting maxAnisotropy = {"r.MaxAnisotropy", 32};
            inline intSetting mipBias = {"r.Streaming.MipBias", 0};
            inline intSetting boost = {"r.Streaming.Boost", -1};
            inline intSetting poolSizeAt = {"r.Streaming.PoolSize", 32768};
            inline intSetting limitPoolSizeToVRAM = {"r.Streaming.LimitPoolSizeToVRAM", 0};
        }

        inline stringSetting bloom = {"r.DefaultFeature.Bloom", sFalse};
        inline stringSetting ambientOcclusion = {"r.DefaultFeature.AmbientOcclusion", sFalse};
        inline stringSetting ambientOcclusionStaticFraction = {
            "r.DefaultFeature.AmbientOcclusionStaticFraction", sFalse
        };
        inline stringSetting motionBlur = {"r.DefaultFeature.MotionBlur", sFalse};
        inline stringSetting lensFlare = {"r.DefaultFeature.LensFlare", sFalse};

        inline bool engineReadOnly = false;
    }

    namespace Files
    {
        inline std::string gameUserSettings = "\\GameUserSettings.ini";
        inline std::string engine = "\\Engine.ini";
    }
}
