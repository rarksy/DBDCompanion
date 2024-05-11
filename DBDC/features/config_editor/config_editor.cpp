#include "config_editor.hpp"
#include <regex>
#include "misc/misc.hpp"
#include "mINI/ini.h"
#include <ShlObj_core.h>
#include "ce_menu.hpp"
#include "menu/menu.hpp"
#include "backend/backend.hpp"
#include "miscLIB/miscLIB.hpp"

bool config_editor::initialize_config()
{
    const auto path = get_settings_folder_location();

    if (exists(path) && !path.empty())
    {
        settings_folder_location = path;
        return true;
    }

    const auto result = MessageBoxA(
        nullptr, "Unable To Locate Settings Folder\nPlease Locate Manually Or Press No To Exit.", "Notice...",
        MB_YESNO);

    if (result != IDYES)
    {
        menu::menu_to_show = 0;
        return false;
    }


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

        settings_folder_location = pszPath;
        // Add your code here to work with the selected folder path
        CoTaskMemFree(pszPath);

        psi->Release();
        pfd->Release();
    }

    CoUninitialize();


    return std::filesystem::exists(settings_folder_location.string() + files::game_user_settings);
}

void config_editor::load_config()
{
    ce_vars.resolution_quality.load_value();
    ce_vars.view_distance_quality.load_value();
    ce_vars.anti_alias_quality.load_value();
    ce_vars.shadow_quality.load_value();
    ce_vars.post_process_quality.load_value();
    ce_vars.texture_quality.load_value();
    ce_vars.effects_quality.load_value();
    ce_vars.foliage_quality.load_value();
    ce_vars.shading_quality.load_value();
    ce_vars.animation_quality.load_value();

    ce_vars.window_mode.load_value();
    ce_vars.resolution_width.load_value();
    ce_vars.resolution_height.load_value();
    ce_vars.fps_limit_mode.load_value();

    const auto find_fps_limit_mode = std::ranges::find(CEMenu::fpsLimitModes, std::to_string(ce_vars.fps_limit_mode.value));
    if (find_fps_limit_mode != CEMenu::fpsLimitModes.end())
    {
        const int index = std::ranges::distance(CEMenu::fpsLimitModes.begin(), find_fps_limit_mode);
        ce_vars.fps_limit_mode_index = index;
    }
    
    ce_vars.use_vsync.load_value();
    ce_vars.anti_alias_mode.load_value();
    ce_vars.ambient_occlusion.load_value();
    ce_vars.ambient_occlusion_static_fraction.load_value();
    ce_vars.bloom.load_value();
    ce_vars.lens_flare.load_value();
    ce_vars.motion_blur.load_value();
    
    ce_vars.killer_fov.load_value();
    ce_vars.remove_intro_cutscene = std::filesystem::exists(misc::get_game_root_directory() + "DeadByDaylight\\Content\\Movies\\" + "disabled_AdditionalLoadingScreen");
    ce_vars.skip_news_popup.load_value();
    ce_vars.launch_with_dbd = std::filesystem::exists(backend::exe_directory.string() + backend::settings_directory + "dual_load.bat");

    ce_vars.survivor_mouse_sensitivity.load_value();
    ce_vars.survivor_controller_sensitivity.load_value();
    ce_vars.killer_mouse_sensitivity.load_value();
    ce_vars.killer_controller_sensitivity.load_value();

    ce_vars.terror_radius_visual.load_value();
    ce_vars.color_blind_mode.load_value();
    ce_vars.color_blind_mode_strength.load_value();
    

    if (ml::is_exe_running(L"DeadByDaylight-Win64-Shipping.exe"))
        MessageBoxA(nullptr, "Game Is Running, Changes Won't Apply Until It Is Restarted.", "Notice...", MB_OK);
}

bool config_editor::copy_config()
{
    std::ifstream file(settings_folder_location.string() + files::game_user_settings, std::ios::binary);

    if (!file.is_open())
        return false;

    std::ostringstream oss_game_user_settings;
    oss_game_user_settings << file.rdbuf();
    file.close();

    const std::string game_user_settings_content = oss_game_user_settings.str();

    file.open(settings_folder_location.string() + files::engine, std::ios::binary);
    if (!file.is_open())
        return false;

    std::ostringstream oss_engine;
    oss_engine << file.rdbuf();
    file.close();

    const std::string engine_content = oss_engine.str();

    const std::string combined_content = game_user_settings_content + "ENDFILE" + engine_content;

    const std::string compressed = misc::CompressString(combined_content);

    const std::string to_base64 = misc::Base64Encode(compressed);

    misc::SetClipboardText(to_base64);

    return true;
}

bool config_editor::import_config()
{
    const auto result = MessageBoxA(NULL, "This Will Replace Your Current Companion Settings, Continue?", "Notice...", MB_YESNO);

    if (result == IDNO)
        return false;
    
    const std::string config = misc::GetClipboardText();

    std::regex base64Regex("^[A-Za-z0-9+/]*={0,2}$"); // still dont know how regex works, thanks stackoverflow
    if (!std::regex_match(config, base64Regex))
    {
        MessageBoxA(NULL, "Invalid Config", "Error...", MB_OK);
        return false;
    }

    const std::string base64_decoded = misc::Base64Decode(config);
    const std::string decompressed = misc::DecompressString(base64_decoded);

    const std::string game_user_settings_content = decompressed.substr(0, decompressed.find("ENDFILE"));
    const std::string engine_content = decompressed.substr(decompressed.find("ENDFILE") + 7);
    // Setup Temp Files
    
    const std::string temp_game_user_settings = "ImportedGameUserSettings.ini";
    std::ofstream temp_game_user_settings_file(temp_game_user_settings);
    temp_game_user_settings_file << game_user_settings_content;
    temp_game_user_settings_file.close();

    const std::string basic_string = "ImportedEngine.ini";
    std::ofstream temp_engine_file(basic_string);
    temp_engine_file << engine_content;
    temp_engine_file.close();

    // Setup Imported INI Structures
    
    mINI::INIStructure imported_game_user_settings_ini;
    mINI::INIFile imported_game_user_settings(temp_game_user_settings);
    imported_game_user_settings.read(imported_game_user_settings_ini);
    
    mINI::INIStructure imported_engine_ini;
    mINI::INIFile imported_engine(basic_string);
    imported_engine.read(imported_engine_ini);

    // Setup Main INI Structures

    ce_vars.resolution_quality.import_value(imported_game_user_settings);
    ce_vars.view_distance_quality.import_value(imported_game_user_settings);
    ce_vars.anti_alias_quality.import_value(imported_game_user_settings);
    ce_vars.shadow_quality.import_value(imported_game_user_settings);
    ce_vars.post_process_quality.import_value(imported_game_user_settings);
    ce_vars.texture_quality.import_value(imported_game_user_settings);
    ce_vars.effects_quality.import_value(imported_game_user_settings);
    ce_vars.foliage_quality.import_value(imported_game_user_settings);
    ce_vars.shading_quality.import_value(imported_game_user_settings);
    ce_vars.animation_quality.import_value(imported_game_user_settings);

    ce_vars.use_vsync.import_value(imported_game_user_settings);
    ce_vars.anti_alias_mode.import_value(imported_game_user_settings);
    ce_vars.ambient_occlusion.import_value(imported_engine);
    ce_vars.ambient_occlusion_static_fraction.import_value(imported_engine);
    ce_vars.bloom.import_value(imported_engine);
    ce_vars.lens_flare.import_value(imported_engine);
    ce_vars.motion_blur.import_value(imported_engine);

    
    ce_vars.window_mode.import_value(imported_game_user_settings);
    ce_vars.resolution_width.import_value(imported_game_user_settings);
    ce_vars.resolution_height.import_value(imported_game_user_settings);
    ce_vars.fps_limit_mode.import_value(imported_game_user_settings);

    const auto find_fps_limit_mode = std::ranges::find(CEMenu::fpsLimitModes, std::to_string(ce_vars.fps_limit_mode.value));
    if (find_fps_limit_mode != CEMenu::fpsLimitModes.end())
    {
        const int index = std::ranges::distance(CEMenu::fpsLimitModes.begin(), find_fps_limit_mode);
        ce_vars.fps_limit_mode_index = index;
    }
    
    ce_vars.killer_fov.import_value(imported_game_user_settings);

    ce_vars.remove_intro_cutscene = !std::filesystem::exists(misc::get_game_root_directory() + "DeadByDaylight\\Content\\Movies\\" + "AdditionalLoadingScreen");
    ce_vars.skip_news_popup.import_value(imported_game_user_settings);

    ce_vars.survivor_mouse_sensitivity.import_value(imported_game_user_settings);
    ce_vars.survivor_controller_sensitivity.import_value(imported_game_user_settings);
    ce_vars.killer_mouse_sensitivity.import_value(imported_game_user_settings);
    ce_vars.killer_controller_sensitivity.import_value(imported_game_user_settings);

    ce_vars.terror_radius_visual.import_value(imported_game_user_settings);
    ce_vars.color_blind_mode.import_value(imported_game_user_settings);
    ce_vars.color_blind_mode_strength.import_value(imported_game_user_settings);
    
    load_config();
    
    std::remove(temp_game_user_settings.c_str());
    std::remove(basic_string.c_str());

    return true;
}

bool config_editor::set_read_only(const std::string& file, const bool& value)
{
    DWORD attributes = GetFileAttributesA((settings_folder_location.string() + file).c_str());

    if (value)
        attributes |= FILE_ATTRIBUTE_READONLY;
    else
        attributes &= ~FILE_ATTRIBUTE_READONLY;

    return SetFileAttributesA((settings_folder_location.string() + file).c_str(), attributes);
}

bool config_editor::get_read_only(const std::string& file)
{
    return GetFileAttributesA((settings_folder_location.string() + file).c_str()) &
        FILE_ATTRIBUTE_READONLY;
}

std::filesystem::path config_editor::get_settings_folder_location()
{
    auto local_app_data_path = std::filesystem::temp_directory_path().parent_path().parent_path();
    const std::string steam_path = local_app_data_path.string() + "\\DeadByDaylight\\Saved\\Config\\WindowsClient\\";

    if (!std::filesystem::exists(steam_path))
    {
        const std::string egs_path = local_app_data_path.string() + "\\DeadByDaylight\\Saved\\Config\\EGS\\";
        if (std::filesystem::exists(egs_path))
            return egs_path;
    }
    else return steam_path;

    MessageBoxA(nullptr, (std::filesystem::exists(steam_path) ? "Settings Path Found" : "Settings Path Not Found"), "Debug", MB_OK);

    return "";
}