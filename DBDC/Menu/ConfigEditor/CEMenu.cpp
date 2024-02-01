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

    return true;
}

void CEMenu::RenderUI()
{
    ImGui::Columns(3, nullptr, false);
    ImGui::SetColumnWidth(0, 270);
    ImGui::SetColumnWidth(1, 200);

    ImGui::SeparatorText("Graphics Quality");
    GUI::ToolTip("Features settings that affect the graphical fidelity of Dead By Daylight.");

    static GLuint texture = Image::ResolutionQuality::texture100;
    if (GUI::Slider("Resolution Quality", CEVars.resolutionQuality, 60, 100))
    {
        const auto value = CEVars.resolutionQuality.value;
        if (value < 79)
            texture = Image::ResolutionQuality::texture60;
        else if (value < 99)
            texture = Image::ResolutionQuality::texture80;
        else
            texture = Image::ResolutionQuality::texture100;
    }
    GUI::ToolTip("Sets the quality at which the game is rendered.\n"
                 "Note: 100%% = native resolution", texture, ImVec2(400, 170));

    GUI::DropDownBox("View Distance", CEVars.viewDistanceQuality, qualities);
    GUI::ToolTip("Changes the level of detail at which objects in the distance are rendered.\n"
        "The higher the setting, the further the distance before objects lose quality.");

    ImGui::BeginDisabled(!CEVars.antiAliasMode.value);
    GUI::DropDownBox("Anti-AIiasing", CEVars.antiAliasQuality, qualities);
    ImGui::EndDisabled();
    GUI::ToolTip("Changes the strength of anti-aliasing's effect.");

    GUI::DropDownBox("Shadows", CEVars.shadowQuality, qualities);
    GUI::ToolTip("Lowers the quality & amount of shadows used.");

    GUI::DropDownBox("Post Processing", CEVars.postProcessQuality, qualities);
    GUI::ToolTip("Changes the quality of glow related effects (such as fire glow).");

    GUI::DropDownBox("Textures", CEVars.textureQuality, qualities, true, Menu::Styling::itemWidth, "Changes the quality of textures & models.", Image::TextureQuality::allTextures,
                     ImVec2(500, 200));

    GUI::ToolTip("Changes the quality of textures & models.");

    GUI::DropDownBox("Effects", CEVars.effectsQuality, qualities);
    GUI::ToolTip("Changes the quality of effects (such as fire particles etc).");

    GUI::DropDownBox("Foliage", CEVars.foliageQuality, qualities);
    GUI::ToolTip("Changes the quality & amount of foliage used (such as grass, bushes, corn).");

    GUI::DropDownBox("Shading", CEVars.shadingQuality, qualities);

    GUI::DropDownBox("Animations", CEVars.animationQuality, qualities);

    GUI::ToolTip("Changes the quality of the shading.\n" "(i'll be real idk what this does)");

    ImGui::NextColumn();

    ImGui::SeparatorText("Rendering");
    GUI::ToolTip("Features settings that affect the way the game renders.");

    GUI::Checkbox("VSync", CEVars.useVSync);
    GUI::ToolTip("Syncs Dead By Daylight's framerate to your refresh rate.\n"
        "Note: Can cause input delay.");

    GUI::Checkbox("Anti-Aliasing", CEVars.antiAliasMode);
    GUI::ToolTip("Blurs the edges of objects to appear less jagged.",
                 CEVars.antiAliasMode.value
                     ? Image::AntiAliasing::textureOn
                     : Image::AntiAliasing::textureOff, ImVec2(400, 250));


    GUI::Checkbox("Ambient Occlusion", CEVars.ambientOcclusion);
    GUI::Checkbox("A/O Static Fraction", CEVars.ambientOcclusionStaticFraction);
    GUI::Checkbox("Bloom", CEVars.bloom);
    GUI::Checkbox("Lens Flare", CEVars.lensFlare);
    GUI::Checkbox("Motion Blur", CEVars.motionBlur);

    ImGui::NextColumn();

    ImGui::SeparatorText("Misc");
    GUI::ToolTip("Features settings that affect the user experience.");

    GUI::DropDownBox("Window Mode", CEVars.windowMode, windowModes);
    GUI::ToolTip("Changes the rendering mode used to display the game.");

    ImGui::BeginDisabled(CEVars.windowMode.value);
    {
        GUI::InputInt("##ResolutionW", CEVars.resolutionWidth, 49);
        GUI::ToolTip("Sets the desired width for the game window.");
        ImGui::SameLine();
        ImGui::Text("x");
        GUI::ToolTip("Sets the desired resolution for the game window.");
        ImGui::SameLine();
        GUI::InputInt("##ResolutionH", CEVars.resolutionHeight, 48);

        GUI::ToolTip("Sets the desired height for the game window.");
        ImGui::SameLine(133);
        ImGui::Text("Resolution");
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(CEVars.useVSync.value);

    GUI::DropDownBox("FPS Limit Mode", CEVars.fpsLimitMode, fpsLimitModes, false, 40);

    ImGui::EndDisabled();
    GUI::ToolTip("Sets the maximum achievable framerate.\n" "Values are clamped and cannot go above/below the available options.");
    
    GUI::Slider("Killer FOV", CEVars.killerFOV, 87, 103);
    GUI::ToolTip("Changes the FOV used for 1st person killers.");
    
    if (ImGui::Checkbox("Remove Intro Cutscene", &CEVars.removeIntroCutscene))
    {
        const std::string gameDir = Misc::GetGameRootDirectory();
        const std::string moviesDir = gameDir + "DeadByDaylight\\Content\\Movies\\";
    
        if (CEVars.removeIntroCutscene)
        {
            if (std::filesystem::exists(moviesDir + "disabled_AdditionalLoadingScreen"))
                std::filesystem::remove_all(moviesDir + "disabled_AdditionalLoadingScreen");
            
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
    GUI::ToolTip("Skips the cutscene that plays after launching the game.");
    
    GUI::Checkbox("Skip News Popup", CEVars.skipNewsPopup, 0, 99999);
    GUI::ToolTip("Disables the news popup that appears after launching the game.");
    
    ImGui::SeparatorText("Accessibility");
    
    GUI::Checkbox("Terror Radius Visual", CEVars.terrorRadiusVisual);
    GUI::ToolTip("Adds a visual heartbeat whenever inside the killers terror radius");
    
    GUI::DropDownBox("Colorblind Mode", CEVars.colorBlindMode, colorBlindModes);
    GUI::ToolTip("Adjusts the games color pallet.");
    
    GUI::Slider("Colorblind Strength", CEVars.colorBlindModeStrength, 0, 100);
    GUI::ToolTip("Adjusts the strength of the changed color pallet.");
    
    ImGui::SeparatorText("Other");
    
    //if (ImGui::Button("Start In DX12")) // doesnt actually work, malding
        //Misc::RestartGame(true);
    //GUI::ToolTip("Will close and reopen Dead By Daylight Using DirectX 12.\nThis will also apply any changed settings.");
    
    if (ImGui::Button("Copy Settings"))
        ConfigEditor::CopyConfig();
    GUI::ToolTip("Will copy your settings to clipboard, you can send it to anyone else using DBDC.");
    
    ImGui::SameLine();
    
    if (ImGui::Button("Import Settings"))
        ConfigEditor::ImportConfig();
    GUI::ToolTip("Will import settings copied to clipboard.");
    
    if (ImGui::Button("Open Folder"))
        Misc::OpenSettingsFolder();
    
    ImGui::SameLine();
    
    if (ImGui::Button("Restart Game"))
        Misc::RestartGame();
    GUI::ToolTip("Will close and reopen Dead By Daylight to apply any changed settings.");
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
