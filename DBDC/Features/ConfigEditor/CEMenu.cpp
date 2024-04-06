﻿#include "CEMenu.hpp"
#include "../Menu.h"
#include "../Misc/Misc.hpp"
#include "GUI/GUI.h"
#include <Windows.h>
#include <Images/Images.h>
#include "ConfigEditor.hpp"

bool CEMenu::Setup()
{
    do
    {
    }
    while (!config_editor::initialize_config());

    config_editor::load_config();

    // Load Images

    // Anti Aliasing
    images::load_texture_from_memory(antiAliasingOnRawData, sizeof antiAliasingOnRawData, &Image::AntiAliasing::textureOn);
    images::load_texture_from_memory(antiAliasingOffRawData, sizeof antiAliasingOffRawData,
                                     &Image::AntiAliasing::textureOff);

    // Resolution Quality
    images::load_texture_from_memory(resolutionQuality60RawData, sizeof resolutionQuality60RawData,
                                     &Image::ResolutionQuality::texture60);
    images::load_texture_from_memory(resolutionQuality80RawData, sizeof resolutionQuality80RawData,
                                     &Image::ResolutionQuality::texture80);
    images::load_texture_from_memory(resolutionQuality100RawData, sizeof resolutionQuality100RawData,
                                     &Image::ResolutionQuality::texture100);

    // Texture Quality
    images::load_texture_from_memory(textureQualityVeryLowRawData, sizeof textureQualityVeryLowRawData,
                                     &Image::TextureQuality::textureVeryLow);
    images::load_texture_from_memory(textureQualityLowRawData, sizeof textureQualityLowRawData,
                                     &Image::TextureQuality::textureLow);
    images::load_texture_from_memory(textureQualityMediumRawData, sizeof textureQualityMediumRawData,
                                     &Image::TextureQuality::textureMedium);
    images::load_texture_from_memory(textureQualityHighRawData, sizeof textureQualityHighRawData,
                                     &Image::TextureQuality::textureHigh);
    images::load_texture_from_memory(textureQualityUltraRawData, sizeof textureQualityUltraRawData,
                                     &Image::TextureQuality::textureUltra);

    return true;
}

void CEMenu::RenderUI()
{
    ImGui::SetCursorPos({252, 15});

    if (ImGui::Button("Copy Settings"))
        config_editor::copy_config();
    gui::tool_tip("Will copy your settings to clipboard, you can send it to anyone else using DBDC.");

    ImGui::SameLine();

    if (ImGui::Button("Import Settings"))
        config_editor::import_config();
    gui::tool_tip("Will import settings copied to clipboard.");

    ImGui::SameLine();

    if (ImGui::Button("Open Folder"))
        misc::OpenSettingsFolder();

    ImGui::SameLine();

    if (ImGui::Button("Restart Game"))
        misc::restart_game();
    gui::tool_tip("Will close and reopen Dead By Daylight to apply any changed settings.");

    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 258);
    ImGui::SetColumnWidth(1, 227);

    ImGui::SeparatorText("Graphics Quality");
    gui::tool_tip("Features settings that affect the graphical fidelity of Dead By Daylight.");

    static GLuint texture = Image::ResolutionQuality::texture100;
    if (gui::slider("Resolution Quality", ce_vars.resolution_quality, 60, 100))
    {
        const auto value = ce_vars.resolution_quality.value;
        if (value < 79)
            texture = Image::ResolutionQuality::texture60;
        else if (value < 99)
            texture = Image::ResolutionQuality::texture80;
        else
            texture = Image::ResolutionQuality::texture100;
    }
    gui::tool_tip("Sets the quality at which the game is rendered.\n"
                  "Note: 100%% = native resolution", texture, ImVec2(400, 170));

    gui::drop_down_box("View Distance", ce_vars.view_distance_quality, qualities);
    gui::tool_tip("Changes the level of detail at which objects in the distance are rendered.\n"
        "The higher the setting, the further the distance before objects lose quality.");

    ImGui::BeginDisabled(!ce_vars.anti_alias_mode.value);
    gui::drop_down_box("Anti-AIiasing", ce_vars.anti_alias_quality, qualities);
    ImGui::EndDisabled();
    gui::tool_tip("Changes the strength of anti-aliasing's effect.");

    gui::drop_down_box("Shadows", ce_vars.shadow_quality, qualities);
    gui::tool_tip("Lowers the quality & amount of shadows used.");

    gui::drop_down_box("Post Processing", ce_vars.post_process_quality, qualities);
    gui::tool_tip("Changes the quality of glow related effects (such as fire glow).");

    gui::drop_down_box("Textures", ce_vars.texture_quality, qualities, true, menu::styling::item_width, "Changes the quality of textures & models.",
                       Image::TextureQuality::allTextures,
                       ImVec2(500, 200));

    gui::tool_tip("Changes the quality of textures & models.");

    gui::drop_down_box("Effects", ce_vars.effects_quality, qualities);
    gui::tool_tip("Changes the quality of effects (such as fire particles etc).");

    gui::drop_down_box("Foliage", ce_vars.foliage_quality, qualities);
    gui::tool_tip("Changes the quality & amount of foliage used (such as grass, bushes, corn).");

    gui::drop_down_box("Shading", ce_vars.shading_quality, qualities);
    gui::tool_tip("Changes the quality of the shading.\n" "(i'll be real idk what this does)");

    gui::drop_down_box("Animations", ce_vars.animation_quality, qualities);
    gui::tool_tip("Changes the quality of animations at long distances.");

    ImGui::NextColumn();

    ImGui::SeparatorText("Rendering");
    gui::tool_tip("Features settings that affect the way the game renders.");

    gui::drop_down_box("Window Mode", ce_vars.window_mode, windowModes);
    gui::tool_tip("Changes the rendering mode used to display the game.");

    ImGui::BeginDisabled(ce_vars.window_mode.value);
    {
        gui::input_int("##ResolutionW", ce_vars.resolution_width, 49);
        gui::tool_tip("Sets the desired width for the game window.");
        ImGui::SameLine();
        ImGui::Text("x");
        gui::tool_tip("Sets the desired resolution for the game window.");
        ImGui::SameLine();
        gui::input_int("##ResolutionH", ce_vars.resolution_height, 48);

        gui::tool_tip("Sets the desired height for the game window.");
        ImGui::SameLine(133);
        ImGui::Text("Resolution");
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(ce_vars.use_vsync.value);

    gui::drop_down_box("FPS Limit Mode", ce_vars.fps_limit_mode, fpsLimitModes, false, 40);

    ImGui::EndDisabled();
    gui::tool_tip("Sets the maximum achievable framerate.\n" "Values are clamped and cannot go above/below the available options.");

    gui::checkbox("VSync", ce_vars.use_vsync);
    gui::tool_tip("Syncs Dead By Daylight's framerate to your refresh rate.\n"
        "Note: Can cause input delay.");

    gui::checkbox("Anti-Aliasing", ce_vars.anti_alias_mode);
    gui::tool_tip("Blurs the edges of objects to appear less jagged.",
                  ce_vars.anti_alias_mode.value
                      ? Image::AntiAliasing::textureOn
                      : Image::AntiAliasing::textureOff, ImVec2(400, 250));


    gui::checkbox("Ambient Occlusion", ce_vars.ambient_occlusion);
    gui::checkbox("A/O Static Fraction", ce_vars.ambient_occlusion_static_fraction);
    gui::checkbox("Bloom", ce_vars.bloom);
    gui::checkbox("Lens Flare", ce_vars.lens_flare);
    gui::checkbox("Motion Blur", ce_vars.motion_blur);

    ImGui::NextColumn();

    ImGui::SeparatorText("Misc");
    gui::tool_tip("Features settings that affect the user experience.");

    gui::slider("Killer FOV", ce_vars.killer_fov, 87, 103);
    gui::tool_tip("Changes the FOV used for 1st person killers.");

    if (ImGui::Checkbox("Remove Intro Cutscene", &ce_vars.remove_intro_cutscene))
    {
        const std::string gameDir = misc::get_game_root_directory();
        const std::string moviesDir = gameDir + "DeadByDaylight/Content/Movies/";

        if (ce_vars.remove_intro_cutscene)
        {
            if (std::filesystem::exists(moviesDir + "disabled_AdditionalLoadingScreen"))
                std::filesystem::remove_all(moviesDir + "disabled_AdditionalLoadingScreen");

            if (std::rename(
                (moviesDir + "AdditionalLoadingScreen").c_str(),
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Couldn't Disable Intro Cutscene.", "Error...", MB_OK);
                ce_vars.remove_intro_cutscene = false;
            }
        }
        else
        {
            if (std::filesystem::exists(moviesDir + "AdditionalLoadingScreen"))
                std::filesystem::remove_all(moviesDir + "AdditionalLoadingScreen");

            if (std::rename(
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str(),
                (moviesDir + "AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Couldn't Enable Intro Cutscene.", "Error...", MB_OK);
                ce_vars.remove_intro_cutscene = true;
            }
        }
    }
    gui::tool_tip("Skips the cutscene that plays after launching the game.");

    gui::checkbox("Skip News Popup", ce_vars.skip_news_popup, 0, 99999);
    gui::tool_tip("Disables the news popup that appears after launching the game.");

    if (ImGui::Checkbox("Launch With DBD", &ce_vars.launch_with_dbd))
    {
        if (ce_vars.launch_with_dbd)
        {
            char file_name_buffer[MAX_PATH];
            GetModuleFileNameA(NULL, file_name_buffer, MAX_PATH);

            std::ofstream file_to_write(backend::exe_directory.string() + backend::settings_directory + "dual_load.bat");

            if (file_to_write.is_open())
            {
                file_to_write
                    << "@echo off\n"
                    << "start \"\" \"" << misc::get_game_root_directory() << "DeadByDaylight.exe\" -provider Steam\n"
                    << "start \"\" \"" << file_name_buffer << "\"\n"
                    << "exit";

                file_to_write.close();

                ml::set_clipboard_text("\"" + backend::exe_directory.string() + backend::settings_directory + "dual_load.bat\" %command%");

                MessageBox(
                    NULL,
                    L"Launch Command Copied\n\nGo To Steam -> Library -> Right Click \"Dead By Daylight\" -> Properties -> In The \"Launch Options\" Box -> Right Click -> Paste",
                    L"Notice", MB_OK);
            }
        }
        else
        {
            MessageBox(NULL, L"To Disable Dual Loading, Go To Steam -> Library -> Right Click \"Dead By Daylight\" -> Properties -> Clear The \"Launch Options\" Box", L"Notice", MB_OK);
        }
    }
    gui::tool_tip(
        "Will automatically open DBDC when you launch Dead By Daylight"
        "\n\n"
        "Note: Due to how steam works, after enabling, you will need to manually add the launch option that gets copied to your clipboard, instructions appear when enabling / disabling."
    );

    ImGui::SeparatorText("Sensitivity");

    gui::slider("Survivor Mouse", ce_vars.survivor_mouse_sensitivity, 0, 100, false);
    gui::slider("Survivor Controller", ce_vars.survivor_controller_sensitivity, 0, 100, false);
    gui::slider("Killer Mouse", ce_vars.killer_mouse_sensitivity, 0, 100, false);
    gui::slider("Killer Controller", ce_vars.killer_controller_sensitivity, 0, 100, false);

    ImGui::SeparatorText("Accessibility");

    gui::checkbox("Terror Radius Visual", ce_vars.terror_radius_visual);
    gui::tool_tip("Adds a visual heartbeat whenever inside the killers terror radius");

    gui::drop_down_box("Colorblind Mode", ce_vars.color_blind_mode, colorBlindModes);
    gui::tool_tip("Adjusts the games color pallet.");

    ImGui::BeginDisabled(!ce_vars.color_blind_mode.value);

    gui::slider("Strength", ce_vars.color_blind_mode_strength, 0, 100);
    gui::tool_tip("Adjusts the strength of the changed color pallet.");

    ImGui::EndDisabled();

    //if (ImGui::Button("Start In DX12")) // doesnt actually work, malding
    //Misc::RestartGame(true);
    //GUI::ToolTip("Will close and reopen Dead By Daylight Using DirectX 12.\nThis will also apply any changed settings.");

    ImGui::EndColumns();
}

inline ImVec4 RGBToImVec4(int r, int g, int b, int a = 255)
{
    return {r / 255.F, g / 255.F, b / 255.F, a / 255.F};
}

void CEMenu::CreateStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    // Button
    colors[ImGuiCol_Button] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_ButtonHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_ButtonActive] = RGBToImVec4(255, 203, 203);

    // Main Window
    colors[ImGuiCol_FrameBg] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_FrameBgHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_FrameBgActive] = RGBToImVec4(255, 203, 203);
    style.FrameRounding = 2.F;
    style.DisabledAlpha = 0.3F;
    style.FrameBorderSize = 1.7F;

    // Slider
    colors[ImGuiCol_Slider] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_SliderActive] = RGBToImVec4(255, 203, 203);
    colors[ImGuiCol_SliderHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_SliderGrab] = RGBToImVec4(255, 83, 83);
    style.GrabRounding = style.FrameRounding;

    // Checkbox
    colors[ImGuiCol_CheckMark] = RGBToImVec4(255, 83, 83);

    // Combo
    colors[ImGuiCol_Combo] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_ComboActive] = RGBToImVec4(255, 203, 203);
    colors[ImGuiCol_ComboHovered] = RGBToImVec4(255, 153, 153);

    // Header ( Selectables Etc )
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(255, 203, 203);

    // Separator
    colors[ImGuiCol_Separator] = RGBToImVec4(255, 83, 83);
}