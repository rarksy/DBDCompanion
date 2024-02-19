#pragma once
#include <filesystem>
#include "mINI/ini.h"

namespace config_editor
{
    bool initialize_config();
    void load_config();

    bool set_read_only(const std::string& file, const bool& value);
    bool get_read_only(const std::string& file);

    std::filesystem::path get_settings_folder_location();
    inline std::filesystem::path settings_folder_location;

    namespace files
    {
        inline std::string additional_loading_screen = "DeadByDaylight\\Content\\Movies\\AdditionalLoadingScreen";
        inline std::string game_user_settings = "\\GameUserSettings.ini";
        inline std::string engine = "\\Engine.ini";
    }

    namespace sections
    {
        inline std::string scalability_groups = "ScalabilityGroups";
        inline std::string dbd_game_user_settings = "/Script/DeadByDaylight.DBDGameUserSettings";
        inline std::string renderer_override_settings = "/Script/Engine.RendererOverrideSettings";
    }

    struct setting
    {
        std::string file;
        std::string section;
        std::string variable;
        int value;

        setting(std::string _file, std::string _section, std::string _variable, int _value)
            : file(_file), section(_section), variable(_variable), value(_value)
        {
        }
        ~setting(){}

        bool import_value(const mINI::INIFile& _file)
        {
            mINI::INIStructure imported_ini;
            _file.read(imported_ini);
            
            mINI::INIFile local_ini_file(settings_folder_location.string() + this->file);
            mINI::INIStructure local_ini;
            local_ini_file.read(local_ini);

            local_ini[this->section][this->variable] = imported_ini[this->section][this->variable];

            if (get_read_only(this->file))
                set_read_only(this->file, false);

            const bool write_success = local_ini_file.write(local_ini, true);

            set_read_only(this->file, true);

            return write_success;
        }

        bool load_value()
        {
            mINI::INIFile ini_file(settings_folder_location.string() + this->file);
            mINI::INIStructure ini;

            const bool read_success = ini_file.read(ini);
            
            if (!read_success)
                return false;

            std::string loaded_value_string = ini[this->section][this->variable];
            std::transform(loaded_value_string.begin(), loaded_value_string.end(), loaded_value_string.begin(), tolower);

            if (loaded_value_string.empty())
                return false;
            
            if (loaded_value_string == "true" || loaded_value_string == "false")
            {
                this->value = loaded_value_string == "true";
                return true;
            }

            this->value = std::stoi(loaded_value_string);
            return true;
        }
        
        bool set_value()
        {
            mINI::INIFile ini_file(settings_folder_location.string() + this->file);
            mINI::INIStructure ini;

            ini_file.read(ini);

            ini[this->section][this->variable] = std::to_string(this->value);

            if (get_read_only(this->file))
                set_read_only(this->file, false);

            const bool write_success = ini_file.write(ini);

            set_read_only(this->file, true);

            return write_success;
        }

        bool remove_value()
        {
            mINI::INIFile ini_file(settings_folder_location.string() + this->file);
            mINI::INIStructure ini;

            ini_file.read(ini);

            ini[this->section].remove(this->variable);

            if (get_read_only(this->file))
                set_read_only(this->file, false);

            const bool write_success = ini_file.write(ini);

            set_read_only(this->file, true);

            return write_success;
        }
    };

    class variables
    {
    public:
        // Graphics Quality
        setting resolution_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.ResolutionQuality", 100);
        setting view_distance_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.ViewDistanceQuality", 0);
        setting anti_alias_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.AntiAliasingQuality", 0);
        setting shadow_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.ShadowQuality", 0);
        setting post_process_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.PostProcessQuality", 0);
        setting texture_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.TextureQuality", 0);
        setting effects_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.EffectsQuality", 0);
        setting foliage_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.FoliageQuality", 0);
        setting shading_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.ShadingQuality", 0);
        setting animation_quality = setting(files::game_user_settings, sections::scalability_groups, "sg.AnimationQuality", 0);
        
        // Rendering
        setting window_mode = setting(files::game_user_settings, sections::dbd_game_user_settings, "FullscreenMode", 1);
        setting desired_screen_width = setting(files::game_user_settings, sections::dbd_game_user_settings, "DesiredScreenWidth", 1920);
        setting desired_screen_height = setting(files::game_user_settings, sections::dbd_game_user_settings, "DesiredScreenHeight", 1080);
        setting resolution_width = setting(files::game_user_settings, sections::dbd_game_user_settings, "ResolutionSizeX", 1920);
        setting resolution_height = setting(files::game_user_settings, sections::dbd_game_user_settings, "ResolutionSizeY", 1080);
        setting fps_limit_mode = setting(files::game_user_settings, sections::dbd_game_user_settings, "FPSLimitMode", 60);
        
        setting use_vsync = setting(files::game_user_settings, sections::dbd_game_user_settings, "bUseVSync", 1);
        setting anti_alias_mode = setting(files::game_user_settings, sections::dbd_game_user_settings, "AntiAliasingMode", 1);
        
        setting ambient_occlusion = setting(files::engine, sections::renderer_override_settings, "r.DefaultFeature.AmbientOcclusion", 1);
        setting ambient_occlusion_static_fraction = setting(files::engine, sections::renderer_override_settings, "r.DefaultFeature.AmbientOcclusionStaticFraction", 1);
        setting bloom = setting(files::engine, sections::renderer_override_settings, "r.DefaultFeature.Bloom", 1);
        setting lens_flare = setting(files::engine, sections::renderer_override_settings, "r.DefaultFeature.LensFlare", 1);
        setting motion_blur = setting(files::engine, sections::renderer_override_settings, "r.DefaultFeature.MotionBlur", 1);
        
        // Misc
        setting killer_fov = setting(files::game_user_settings, sections::dbd_game_user_settings, "FieldOfView", 87);
        setting killer_mouse_sensitivity = setting(files::game_user_settings, sections::dbd_game_user_settings, "KillerMouseSensitivity", 50);
        setting killer_controller_sensitivity = setting(files::game_user_settings, sections::dbd_game_user_settings, "KillerControllerSensitivity", 50);
        setting survivor_mouse_sensitivity = setting(files::game_user_settings, sections::dbd_game_user_settings, "SurvivorMouseSensitivity", 50);
        setting survivor_controller_sensitivity = setting(files::game_user_settings, sections::dbd_game_user_settings, "SurvivorControllerSensitivity", 50);
        bool remove_intro_cutscene = false;
        setting skip_news_popup = setting(files::game_user_settings, sections::dbd_game_user_settings, "HighestWeightSeenNews", 0);
        
        // Accessibility
        setting terror_radius_visual = setting(files::game_user_settings, sections::dbd_game_user_settings, "TerrorRadiusVisualFeedback", 0);
        setting color_blind_mode = setting(files::game_user_settings, sections::dbd_game_user_settings, "ColorBlindMode", 0);
        setting color_blind_mode_strength = setting(files::game_user_settings, sections::dbd_game_user_settings, "ColorBlindModeIntensity", 0);
    };


    bool copy_config();
    bool import_config();
};

inline config_editor::variables ce_vars;
