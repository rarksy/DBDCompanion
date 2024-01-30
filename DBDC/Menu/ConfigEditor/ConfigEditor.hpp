#pragma once
#include <filesystem>
#include <variant>

#include "mINI/ini.h"

#define vFalse std::string("False")
#define vTrue std::string("True")

namespace ConfigEditor
{
    bool InitializeConfig();
    void LoadConfig();

    bool LoadSettingBool(const std::string& _file, const std::string& group, std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>>& setting);
    bool LoadSettingInt(const std::string& _file, const std::string& group, std::pair<std::string, int>& setting);
    bool LoadSettingString(const std::string& _file, const std::string& group, std::pair<std::string, std::string>& setting);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, int>& setting, bool invertValue = false);
    bool LoadSettingFind(const std::string& _file, std::pair<std::string, std::string>& setting, bool invertValue = false);
    
    bool SetReadOnly(const std::string& file, const bool value);
    bool GetReadOnly(const std::string& file);
    
    std::filesystem::path GetSettingsFolderLocation();
    inline std::filesystem::path SettingsFolderLocation;
    
    bool ChangeValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
    bool ChangeValue(std::string _file, std::string section, std::pair<std::string, std::string> stringSetting);
    bool ChangeValue(std::string _file, std::string group, std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>> boolSetting);
    
    bool RemoveValue(std::string _file, std::string group, std::pair<std::string, int> intSetting);
    bool RemoveValue(std::string _file, std::string group, std::pair<std::string, std::string> stringSetting);

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
        std::variant<int, std::string> value;
        
        Setting(std::string _file, std::string _section, std::string _variable, std::variant<int, std::string> _value)
            : file(_file), section(_section), variable(_variable), value(_value)
        {
        }
        
        void SetValue(std::variant<int, std::string> _value)
        {
            value = _value;
        }

        std::string GetValue() const {
            return std::visit([](const auto& v) -> std::string {
                if constexpr (std::is_same_v<decltype(v), int>) {
                    return std::to_string(v);
                } else if constexpr (std::is_same_v<decltype(v), std::string>) {
                    return v;
                } else {
                    return "Unsupported Type";
                }
            }, value);
        }
    };
    
    class Var2
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
        Setting useVSync = Setting(Files::gameUserSettings, Sections::scalabilityGroups, "sg.bUseVSync", "True");
        Setting antiAliasMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "AntiAliasingMode", 1);

        bool engineReadOnly = false;

        Setting ambientOcclusion = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.AmbientOcclusion", "True");
        Setting ambientOcclusionStaticFraction = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.AmbientOcclusionStaticFraction", "True");
        Setting bloom = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.Bloom", "True");
        Setting lensFlare = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.LensFlare", "True");
        Setting motionBlur = Setting(Files::engine, Sections::rendererOverrideSettings, "r.DefaultFeature.MotionBlur", "True");

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
        Setting terrorRadiusVisual = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "TerrorRadiusVisualFeedback", "False");
        Setting colorBlindMode = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ColorBlindMode", 0);
        Setting colorBlindModeStrength = Setting(Files::gameUserSettings, Sections::DBDGameUserSettings, "ColorBlindModeIntensity", 0);
    };

    struct Variables
    {
        bool engineReadOnly = false;
        
        // Variable Name | Bool Value | Enabled Value | Disabled Value
        using boolSetting = std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>>;

        // Variable Name | Value
        using intSetting = std::pair<std::string, int>;
        
        // Variable Name | Value
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
        
        boolSetting skipNewsPopup = {"HighestWeightSeenNews", {false, {"99999", "0"}}};
        
        intSetting antiAliasMode = {"AntiAliasingMode", 1};

        intSetting windowMode = {"FullscreenMode", 1};
        intSetting desiredScreenWidth = {"DesiredScreenWidth", 1920};
        intSetting desiredScreenHeight = {"DesiredScreenHeight", 1080};
        intSetting resolutionWidth = {"ResolutionSizeX", 1080};
        intSetting resolutionHeight = {"ResolutionSizeY", 1080};
        intSetting fpsLimitMode = {"FPSLimitMode", 120};
        intSetting killerFOV = {"FieldOfView", 67};
        

        stringSetting useVSync = {"bUseVSync", vTrue};
        stringSetting ambientOcclusion = {"r.DefaultFeature.AmbientOcclusion", vFalse};
        stringSetting ambientOcclusionStaticFraction = {
            "r.DefaultFeature.AmbientOcclusionStaticFraction", vFalse
        };
        stringSetting bloom = {"r.DefaultFeature.Bloom", vFalse};
        stringSetting lensFlare = {"r.DefaultFeature.LensFlare", vFalse};
        stringSetting motionBlur = {"r.DefaultFeature.MotionBlur", vFalse};
        
        stringSetting terrorRadiusVisual = {"TerrorRadiusVisualFeedback", vFalse};
        intSetting colorBlindMode = {"ColorBlindMode", 0};
        intSetting colorBlindModeStrength = {"ColorBlindModeIntensity", 0};
        
        bool removeIntroCutscene = false;
    };

    bool CopyConfig();
    bool ImportConfig();
    void ImportSettingInt(mINI::INIStructure& ini, mINI::INIStructure& importedIni, std::string group, std::pair<std::string, int> setting);
    void ImportSettingString(mINI::INIStructure& ini, mINI::INIStructure& importedIni, std::string group, std::pair<std::string, std::string> setting);
    void ImportSettingBool(mINI::INIStructure& ini, mINI::INIStructure& importedIni, std::string group, std::pair<std::string, std::pair<bool, std::pair<std::string, std::string>>> setting);
    void ImportSettingFindString(mINI::INIStructure& ini, mINI::INIStructure& importedIni, std::string group, std::pair<std::string, std::string> setting, std
                                 ::string file = Files::gameUserSettings);
};

inline ConfigEditor::Variables CEVars;
