#include "CEMenu.hpp"

#include "../Menu.h"
#include "../Config/Config.h"
#include "../Misc/Misc.hpp"
#include "../GUI/GUI.h"
#include <Windows.h>
#include <Images/Images.h>
#include <fstream>

bool CEMenu::Setup()
{
    do
    {
    }
    while (!Config::InitializeConfig());

    if (!Config::LoadConfig())
    {
        MessageBoxA(nullptr, "Couldn't Load Config, Try Again Or Contact ski In The Discord", "Error...", MB_OK);
        return false;
    }

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

    return true;
}

void CEMenu::RenderUI()
{

    ImGui::Columns(3, nullptr, false);

    ImGui::SeparatorText("Graphics Quality");
    GUI::ToolTip("Features settings that affect the graphical fidelity of Dead By Daylight.");

    if (GUI::DropDownBox("View Distance", qualities, Config::Variables::viewDistanceQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::viewDistanceQuality);
    GUI::ToolTip("Changes the level of detail at which objects in the distance are rendered.\n"
        "The higher the setting, the further the distance before objects lose quality.");

    ImGui::BeginDisabled(Config::Variables::antiAliasMode.second == 0);
    if (GUI::DropDownBox("Anti-AIiasing", qualities, Config::Variables::antiAliasQuality))
    {
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::antiAliasQuality);
    }
    ImGui::EndDisabled();
    GUI::ToolTip("Changes the strength of anti-aliasing's effect.");

    if (GUI::DropDownBox("Shadows", qualities, Config::Variables::shadowQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::shadowQuality);
    GUI::ToolTip("Lowers the quality & amount of shadows used.");

    if (GUI::DropDownBox("Post Processing", qualities, Config::Variables::postProcessQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::postProcessQuality);
    GUI::ToolTip("Changes the quality of glow related effects (such as fire glow).");

    if (GUI::DropDownBox("Textures", qualities, Config::Variables::textureQuality, false,
                         "Changes the quality of textures & models.", Image::TextureQuality::allTextures,
                         ImVec2(500, 200)))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::textureQuality);
    GUI::ToolTip("Changes the quality of textures & models.");

    if (GUI::DropDownBox("Effects", qualities, Config::Variables::effectsQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::effectsQuality);
    GUI::ToolTip("Changes the quality of effects (such as fire particles etc).");

    if (GUI::DropDownBox("Foliage", qualities, Config::Variables::foliageQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::foliageQuality);
    GUI::ToolTip("Changes the quality & amount of foliage used (such as grass, bushes, corn).");

    if (GUI::DropDownBox("Shading", qualities, Config::Variables::shadingQuality))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::shadingQuality);
    GUI::ToolTip("Changes the quality of the shading.\n"
        "(i'll be real idk what this does)");

    ImGui::NextColumn();

    ImGui::SeparatorText("Misc");

    if (GUI::DropDownBox("Window Mode", windowModes, Config::Variables::windowMode))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                  Config::Variables::windowMode);
    GUI::ToolTip("Changes the rendering mode used to display the game.");

    ImGui::BeginDisabled(Config::Variables::windowMode.second == 1);
    {
        ImGui::SetNextItemWidth(49);
        if (ImGui::InputInt("##ResolutionW", &Config::Variables::resolutionWidth.second, 0))
        {
            Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                      Config::Variables::resolutionWidth);
            Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                      Config::Variables::desiredScreenWidth);
        }
        GUI::ToolTip("Sets the desired width for the game window.");
        ImGui::SameLine();
        ImGui::Text("x");
        GUI::ToolTip("Sets the desired resolution for the game window.");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(48);
        if (ImGui::InputInt("##ResolutionH", &Config::Variables::resolutionHeight.second, 0))
        {
            Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                      Config::Variables::resolutionHeight);
            Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                      Config::Variables::desiredScreenHeight);
        }
        GUI::ToolTip("Sets the desired height for the game window.");
        ImGui::SameLine(133);
        ImGui::Text("Resolution");
    }
    ImGui::EndDisabled();

    static GLuint texture = Image::ResolutionQuality::texture100;
    if (GUI::Slider("Resolution Quality", Config::Variables::resolutionQuality, 60, 100))
    {
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::scalabilityGroups,
                                  Config::Variables::resolutionQuality);

        const auto value = Config::Variables::resolutionQuality.second;
        if (value < 79)
            texture = Image::ResolutionQuality::texture60;
        else if (value < 99)
            texture = Image::ResolutionQuality::texture80;
        else
            texture = Image::ResolutionQuality::texture100;
    }
    GUI::ToolTip("Sets the quality at which the game is rendered.\n"
                 "Note: 100%% = native resolution", texture, ImVec2(400, 170));

    ImGui::BeginDisabled(Config::Variables::useVSync.second == "True");
    {
        if (GUI::DropDownBox("FPS Limit Mode", fpsLimitModes, Config::Variables::fpsLimitMode, true))
            Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                      Config::Variables::fpsLimitMode);
    }
    ImGui::EndDisabled();
    GUI::ToolTip("Sets the maximum achievable framerate.\n"
        "Values are clamped and cannot go above/below the available options.");
    if (GUI::StringCheckbox("VSync", Config::Variables::useVSync))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                  Config::Variables::useVSync);
    GUI::ToolTip("Syncs Dead By Daylight's framerate to your refresh rate.\n"
        "Note: Can cause input delay.");

    if (GUI::IntCheckbox("Anti-Aliasing", Config::Variables::antiAliasMode))
        Config::Edit::ChangeValue(Config::Files::gameUserSettings, Config::Groups::DBDGameUserSettings,
                                  Config::Variables::antiAliasMode);
    GUI::ToolTip("Blurs the edges of objects to appear less jagged.",
                 Config::Variables::antiAliasMode.second == 1
                     ? Image::AntiAliasing::textureOn
                     : Image::AntiAliasing::textureOff, ImVec2(400, 250));

    ImGui::NextColumn();

    ImGui::SeparatorText("Experimental");
    GUI::ToolTip("Features settings that modify the way the game renders.\nRequires \"Read-Only\" to function.");

    if (ImGui::Checkbox("Read-Only", &Config::Variables::engineReadOnly))
        Config::SetReadOnly(Config::Files::engine,
                            Config::Variables::engineReadOnly);
    GUI::ToolTip("Stops Dead By Daylight from resetting any chosen settings."
        "\nSome Options Require This To Work.");
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::BeginDisabled(!Config::Variables::engineReadOnly);
    {
        if (GUI::StringCheckbox("Ambient Occlusion", Config::Variables::ambientOcclusion))
        {
            if (Config::Variables::ambientOcclusion.second == "False")
                Config::Edit::ChangeValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::ambientOcclusion
                );
            else
            {
                Config::Edit::RemoveValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::ambientOcclusion
                );

                Config::Variables::ambientOcclusion.second = sTrue;
            }
        }

        if (GUI::StringCheckbox("A/O Static Fraction", Config::Variables::ambientOcclusionStaticFraction))
        {
            if (Config::Variables::ambientOcclusionStaticFraction.second == "False")
                Config::Edit::ChangeValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::ambientOcclusionStaticFraction
                );
            else
            {
                Config::Edit::RemoveValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::ambientOcclusionStaticFraction
                );

                Config::Variables::ambientOcclusionStaticFraction.second = sTrue;
            }
        }

        if (GUI::StringCheckbox("Bloom", Config::Variables::bloom))
        {
            if (Config::Variables::bloom.second == "False")
                Config::Edit::ChangeValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::bloom
                );
            else
            {
                Config::Edit::RemoveValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::bloom
                );

                Config::Variables::bloom.second = sTrue;
            }
        }

        if (GUI::StringCheckbox("Lens Flare", Config::Variables::lensFlare))
        {
            if (Config::Variables::lensFlare.second == "False")
                Config::Edit::ChangeValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::lensFlare
                );
            else
            {
                Config::Edit::RemoveValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::lensFlare
                );

                Config::Variables::lensFlare.second = sTrue;
            }
        }

        if (GUI::StringCheckbox("Motion Blur", Config::Variables::motionBlur))
        {
            if (Config::Variables::motionBlur.second == "False")
                Config::Edit::ChangeValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::motionBlur
                );
            else
            {
                Config::Edit::RemoveValue(
                    Config::Files::engine,
                    Config::Groups::rendererOverrideSettings,
                    Config::Variables::motionBlur
                );

                Config::Variables::motionBlur.second = sTrue;
            }
        }
    }
    ImGui::EndDisabled();
    ImGui::Spacing();

    ImGui::SeparatorText("Other");
    if (ImGui::Checkbox("Remove Intro Cutscene", &Config::Variables::removeIntroCutscene))
    {
        const std::string gameDir = Misc::GetGameRootDirectory();
        const std::string moviesDir = gameDir + "DeadByDaylight\\Content\\Movies\\";

        if (Config::Variables::removeIntroCutscene)
        {
            if (std::rename(
                (moviesDir + "AdditionalLoadingScreen").c_str(),  
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Error...", "Couldn't Disable Intro Cutscene.", MB_OK);
                Config::Variables::removeIntroCutscene = false;
            }
        }
        else
        {
            if (std::rename(
                (moviesDir + "disabled_AdditionalLoadingScreen").c_str(),
                (moviesDir + "AdditionalLoadingScreen").c_str()) != 0)
            {
                MessageBoxA(nullptr, "Error...", "Couldn't Enable Intro Cutscene.", MB_OK);
                Config::Variables::removeIntroCutscene = true;
            }
        }
    }
    if (ImGui::Button("Restart Game"))
        Misc::RestartGame();
    GUI::ToolTip("Will Close and reopen Dead By Daylight to apply any changed settings.");

    ImGui::SameLine();
    if (ImGui::Button("Open Folder"))
        Misc::OpenSettingsFolder();

    ImGui::SetCursorPos({720, 470});
    ImGui::TextColored(ImVec4(0.8F, 0.8F, 0.8F, 0.5F), "(?)");
    GUI::ToolTip("Hold right click when hovering an option to view information about it.\n"
                 "Tip: Some options have images associated to assist in selection."
                 "\n\nDead By Daylight Companion By rarksy/ski\n"
                 "Press Enter To Join The Discord Server.\n\n"
                 "Build Version: Early Access "
#ifdef _DEBUG
                 "Debug"
#else
                 "Release"
#endif
                 "\nBuild Date: " + std::string(__DATE__) +
                 "\nBuild Time: " + std::string(__TIME__), false);
    
    if (ImGui::IsItemHovered() && ImGui::IsKeyPressed(ImGuiKey_Enter, false))
        ShellExecuteA(NULL, "open", "https://discord.gg/vKjjS8yazu", NULL, NULL, SW_SHOWNORMAL);
}

ImVec4 RGBToImVec4(int r, int g, int b, int a = 255)
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
    colors[ImGuiCol_Header] = RGBToImVec4(255, 83, 83);
    colors[ImGuiCol_HeaderHovered] = RGBToImVec4(255, 153, 153);
    colors[ImGuiCol_HeaderActive] = RGBToImVec4(255, 203, 203);

    // Separator
    colors[ImGuiCol_Separator] = RGBToImVec4(255, 83, 83);
}
