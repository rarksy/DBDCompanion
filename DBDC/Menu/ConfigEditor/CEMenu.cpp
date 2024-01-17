#include "CEMenu.hpp"

#include "../Menu.h"
#include "../Misc/Misc.hpp"
#include "../GUI/GUI.h"
#include <Windows.h>
#include <Images/Images.h>
#include "ConfigEditor.hpp"

bool CEMenu::Setup()
{
    do
    {
    }
    while (!ConfigEditor::InitializeConfig());

    ConfigEditor::LoadConfig();

    // Load Images

    // Anti Aliasing
    Images::LoadTextureFromMemory(antiAliasingOnRawData, sizeof antiAliasingOnRawData, &Image::AntiAliasing::textureOn);
    Images::LoadTextureFromMemory(antiAliasingOffRawData, sizeof antiAliasingOffRawData,
                                  &Image::AntiAliasing::textureOff);

    // Resolution Quality
    Images::LoadTextureFromMemory(resolutionQuality60RawData, sizeof resolutionQuality60RawData,
                                  &Image::ResolutionQuality::texture60);
    Images::LoadTextureFromMemory(resolutionQuality80RawData, sizeof resolutionQuality80RawData,
                                  &Image::ResolutionQuality::texture80);
    Images::LoadTextureFromMemory(resolutionQuality100RawData, sizeof resolutionQuality100RawData,
                                  &Image::ResolutionQuality::texture100);

    // Texture Quality
    Images::LoadTextureFromMemory(textureQualityVeryLowRawData, sizeof textureQualityVeryLowRawData,
                                  &Image::TextureQuality::textureVeryLow);
    Images::LoadTextureFromMemory(textureQualityLowRawData, sizeof textureQualityLowRawData,
                                  &Image::TextureQuality::textureLow);
    Images::LoadTextureFromMemory(textureQualityMediumRawData, sizeof textureQualityMediumRawData,
                                  &Image::TextureQuality::textureMedium);
    Images::LoadTextureFromMemory(textureQualityHighRawData, sizeof textureQualityHighRawData,
                                  &Image::TextureQuality::textureHigh);
    Images::LoadTextureFromMemory(textureQualityUltraRawData, sizeof textureQualityUltraRawData,
                                  &Image::TextureQuality::textureUltra);

    CEMenu::isSetup = true;
    
    return true;
}

void CEMenu::RenderUI()
{
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 250);
    ImGui::SeparatorText("Graphics Quality");
    GUI::ToolTip("Features settings that affect the graphical fidelity of Dead By Daylight.");

    static GLuint texture = Image::ResolutionQuality::texture100;
    if (GUI::Slider("Resolution Quality", CEVars.resolutionQuality, 60, 100))
    {
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.resolutionQuality);

        const auto value = CEVars.resolutionQuality.second;
        if (value < 79)
            texture = Image::ResolutionQuality::texture60;
        else if (value < 99)
            texture = Image::ResolutionQuality::texture80;
        else
            texture = Image::ResolutionQuality::texture100;
    }
    GUI::ToolTip("Sets the quality at which the game is rendered.\n"
                 "Note: 100%% = native resolution", texture, ImVec2(400, 170));

    if (GUI::DropDownBox("View Distance", qualities, CEVars.viewDistanceQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.viewDistanceQuality);
    GUI::ToolTip("Changes the level of detail at which objects in the distance are rendered.\n"
        "The higher the setting, the further the distance before objects lose quality.");

    ImGui::BeginDisabled(CEVars.antiAliasMode.second == 0);
    if (GUI::DropDownBox("Anti-AIiasing", qualities, CEVars.antiAliasQuality))
    {
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.antiAliasQuality);
    }
    ImGui::EndDisabled();
    GUI::ToolTip("Changes the strength of anti-aliasing's effect.");

    if (GUI::DropDownBox("Shadows", qualities, CEVars.shadowQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.shadowQuality);
    GUI::ToolTip("Lowers the quality & amount of shadows used.");

    if (GUI::DropDownBox("Post Processing", qualities, CEVars.postProcessQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.postProcessQuality);
    GUI::ToolTip("Changes the quality of glow related effects (such as fire glow).");

    if (GUI::DropDownBox("Textures", qualities, CEVars.textureQuality, false,
                         "Changes the quality of textures & models.", Image::TextureQuality::allTextures,
                         ImVec2(500, 200)))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.textureQuality);
    GUI::ToolTip("Changes the quality of textures & models.");

    if (GUI::DropDownBox("Effects", qualities, CEVars.effectsQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.effectsQuality);
    GUI::ToolTip("Changes the quality of effects (such as fire particles etc).");

    if (GUI::DropDownBox("Foliage", qualities, CEVars.foliageQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.foliageQuality);
    GUI::ToolTip("Changes the quality & amount of foliage used (such as grass, bushes, corn).");

    if (GUI::DropDownBox("Shading", qualities, CEVars.shadingQuality))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::scalabilityGroups,
                                  CEVars.shadingQuality);
    GUI::ToolTip("Changes the quality of the shading.\n"
        "(i'll be real idk what this does)");

    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, 230);

    ImGui::SeparatorText("Rendering");
    GUI::ToolTip("Features settings that modify the way the game renders.");

    if (GUI::StringCheckbox("VSync", CEVars.useVSync))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                  CEVars.useVSync);
    GUI::ToolTip("Syncs Dead By Daylight's framerate to your refresh rate.\n"
        "Note: Can cause input delay.");

    if (GUI::IntCheckbox("Anti-Aliasing", CEVars.antiAliasMode))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                  CEVars.antiAliasMode);
    GUI::ToolTip("Blurs the edges of objects to appear less jagged.",
                 CEVars.antiAliasMode.second == 1
                     ? Image::AntiAliasing::textureOn
                     : Image::AntiAliasing::textureOff, ImVec2(400, 250));
    
    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::Checkbox("Read-Only", &CEVars.engineReadOnly))
        ConfigEditor::SetReadOnly(ConfigEditor::Files::engine,
                            CEVars.engineReadOnly);
    GUI::ToolTip("Stops Dead By Daylight from resetting any chosen settings."
        "\nSome Options Require This To Work.");

    ImGui::BeginDisabled(!CEVars.engineReadOnly);
    {
        if (GUI::StringCheckbox("Ambient Occlusion", CEVars.ambientOcclusion))
        {
            if (CEVars.ambientOcclusion.second == "False")
                ConfigEditor::ChangeValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.ambientOcclusion
                );
            else
            {
                ConfigEditor::RemoveValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.ambientOcclusion
                );

                CEVars.ambientOcclusion.second = vTrue;
            }
        }

        if (GUI::StringCheckbox("A/O Static Fraction", CEVars.ambientOcclusionStaticFraction))
        {
            if (CEVars.ambientOcclusionStaticFraction.second == "False")
                ConfigEditor::ChangeValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.ambientOcclusionStaticFraction
                );
            else
            {
                ConfigEditor::RemoveValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.ambientOcclusionStaticFraction
                );

                CEVars.ambientOcclusionStaticFraction.second = vTrue;
            }
        }

        if (GUI::StringCheckbox("Bloom", CEVars.bloom))
        {
            if (CEVars.bloom.second == "False")
                ConfigEditor::ChangeValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.bloom
                );
            else
            {
                ConfigEditor::RemoveValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.bloom
                );

                CEVars.bloom.second = vTrue;
            }
        }

        if (GUI::StringCheckbox("Lens Flare", CEVars.lensFlare))
        {
            if (CEVars.lensFlare.second == "False")
                ConfigEditor::ChangeValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.lensFlare
                );
            else
            {
                ConfigEditor::RemoveValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.lensFlare
                );

                CEVars.lensFlare.second = vTrue;
            }
        }

        if (GUI::StringCheckbox("Motion Blur", CEVars.motionBlur))
        {
            if (CEVars.motionBlur.second == "False")
                ConfigEditor::ChangeValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.motionBlur
                );
            else
            {
                ConfigEditor::RemoveValue(
                    ConfigEditor::Files::engine,
                    ConfigEditor::Groups::rendererOverrideSettings,
                    CEVars.motionBlur
                );

                CEVars.motionBlur.second = vTrue;
            }
        }
    }
    ImGui::EndDisabled();

    ImGui::NextColumn();

    ImGui::SeparatorText("Misc");

    if (GUI::DropDownBox("Window Mode", windowModes, CEVars.windowMode))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                  CEVars.windowMode);
    GUI::ToolTip("Changes the rendering mode used to display the game.");

    ImGui::BeginDisabled(CEVars.windowMode.second == 1);
    {
        ImGui::SetNextItemWidth(49);
        if (ImGui::InputInt("##ResolutionW", &CEVars.resolutionWidth.second, 0))
        {
            ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                      CEVars.resolutionWidth);
            ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                      CEVars.desiredScreenWidth);
        }
        GUI::ToolTip("Sets the desired width for the game window.");
        ImGui::SameLine();
        ImGui::Text("x");
        GUI::ToolTip("Sets the desired resolution for the game window.");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(48);
        if (ImGui::InputInt("##ResolutionH", &CEVars.resolutionHeight.second, 0))
        {
            ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                      CEVars.resolutionHeight);
            ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                      CEVars.desiredScreenHeight);
        }
        
        GUI::ToolTip("Sets the desired height for the game window.");
        ImGui::SameLine(133);
        ImGui::Text("Resolution");
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(CEVars.useVSync.second == "True");
    {
        if (GUI::DropDownBox("FPS Limit Mode", fpsLimitModes, CEVars.fpsLimitMode, true))
            ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                      CEVars.fpsLimitMode);
    }
    ImGui::EndDisabled();
    GUI::ToolTip("Sets the maximum achievable framerate.\n"
        "Values are clamped and cannot go above/below the available options.");

    if (GUI::Slider("Killer FOV", CEVars.killerFOV, 87, 103, false))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings,
                                  CEVars.killerFOV);

    if (ImGui::Checkbox("Remove Intro Cutscene", &CEVars.removeIntroCutscene))
    {
        const std::string gameDir = Misc::GetGameRootDirectory();
        const std::string moviesDir = gameDir + "DeadByDaylight\\Content\\Movies\\";

        if (CEVars.removeIntroCutscene)
        {
            if (std::filesystem::exists(moviesDir + "disabled_AdditionalLoadingScreen"))
                std::filesystem::remove(moviesDir + "disabled_AdditionalLoadingScreen");
            
            if (std::rename(
                (moviesDir + "AdditionalLoadingScreen").c_str(),
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Couldn't Disable Intro Cutscene.", "Error...", MB_OK);
                CEVars.removeIntroCutscene = false;
            }
        }
        else
        {
            if (std::rename(
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str(),
                (moviesDir + "AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Couldn't Enable Intro Cutscene.", "Error...", MB_OK);
                CEVars.removeIntroCutscene = true;
            }
        }
    }

    ImGui::SeparatorText("Accessibility");

    if (GUI::StringCheckbox("Terror Radius Visual", CEVars.terrorRadiusVisual))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings, CEVars.terrorRadiusVisual);
    GUI::ToolTip("Adds a visual heartbeat whenever inside the killers terror radius");

    if (GUI::DropDownBox("Colorblind Mode", colorBlindModes, CEVars.colorBlindMode))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings, CEVars.colorBlindMode);

    if (GUI::Slider("Colorblind Strength", CEVars.colorBlindModeStrength, 0, 100))
        ConfigEditor::ChangeValue(ConfigEditor::Files::gameUserSettings, ConfigEditor::Groups::DBDGameUserSettings, CEVars.colorBlindModeStrength);

    ImGui::SeparatorText("Other");
   
    if (ImGui::Button("Restart Game"))
        Misc::RestartGame();
    GUI::ToolTip("Will close and reopen Dead By Daylight to apply any changed settings.");

    ImGui::SameLine();
    //if (ImGui::Button("Start In DX12")) // doesnt actually work, malding
        //Misc::RestartGame(true);
    //GUI::ToolTip("Will close and reopen Dead By Daylight Using DirectX 12.\nThis will also apply any changed settings.");
    
    if (ImGui::Button("Open Folder"))
        Misc::OpenSettingsFolder();

    if (ImGui::Button("Copy Settings"))
        ConfigEditor::CopyConfig();

    ImGui::SameLine();
    
    if (ImGui::Button("Import Settings"))
        ConfigEditor::ImportConfig();
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
