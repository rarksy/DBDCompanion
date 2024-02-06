#pragma once
#include "ImGui/imgui.h"
#include "../Backend/Backend.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace Crosshair
{
    void Setup();
    void DrawCrosshairs();
    void DrawLines(const ImVec2& centerPoint);
    void DrawOutline(const ImVec2& centerPoint);
    void DrawCenterDot(const ImVec2& centerPoint);
    void ModifyDynamicCenterPoint();
    inline std::vector<ImVec2> allCenterPoints;
    
    struct Variables
    {
        bool enabled = false;

        bool enableLines = false;
        bool enableTopLine = true;
        bool enableLeftLine = true;
        bool enableRightLine = true;
        bool enableBottomLine = true;
        int lineLength = 4;
        int lineThickness = 2;
        int lineGap = 3;
        ImColor lineColor = ImColor(255, 74, 74);

        bool enableOutline = true;
        ImColor outlineColor = ImColor(0, 0, 0);
        int outlineThickness = 1;

        bool enableCenterDot = false;
        bool filledCenterDot = false;
        int centerDotSize = 7;
        int centerDotThickness = 2;
        int centerDotSegments = 12;
        ImColor centerDotColor = ImColor(255, 74, 74);
        
        ImVec2 trueScreenCenterPoint;
        ImVec2 savedScreenCenterPoint;


        bool useDynamicCenterPoint = false;
        int dynamicCenterPointIndex = 0;
    };

    namespace ProfileHandling
    {
        inline int currentSelectedProfile = -1;
        inline int loadedProfile = -1;
        inline std::string profileFolder = "\\DBDC\\Crosshair Profiles\\";
        inline std::vector<std::string> allProfiles;
        inline std::string selectedProfileName;
        inline std::string loadedProfileName;
    }

    inline bool CheckProfileDirectory()
    {
        if (!std::filesystem::exists(Backend::exeDirectory.string() + ProfileHandling::profileFolder))
        {
            const bool directoryCreated = std::filesystem::create_directories(
                Backend::exeDirectory.string() + ProfileHandling::profileFolder);

            return directoryCreated;
        }
        return true;
    }

    inline void ReloadProfiles()
    {
        CheckProfileDirectory();

        for (int i = 0; i < ProfileHandling::allProfiles.size(); i++)
        {
            const auto profile = ProfileHandling::allProfiles[i];
            if (!std::filesystem::exists(Backend::exeDirectory.string() + ProfileHandling::profileFolder + profile))
            {
                const auto it = std::ranges::find(ProfileHandling::allProfiles, profile);

                if (it != ProfileHandling::allProfiles.end())
                    ProfileHandling::allProfiles.erase(it);
            }
        }

        for (const auto& entry : std::filesystem::directory_iterator(
                 Backend::exeDirectory.string() + ProfileHandling::profileFolder))
        {
            if (entry.path().has_extension())
                continue;

            std::string fileName = entry.path().filename().string();

            if (std::ranges::find(ProfileHandling::allProfiles, fileName) != ProfileHandling::allProfiles.end())
                continue;

            ProfileHandling::allProfiles.push_back(fileName);
        }
    }

    inline bool CreateProfile(std::string fileName)
    {
        if (std::ranges::find(ProfileHandling::allProfiles, fileName) != ProfileHandling::allProfiles.end())
            return false;

        std::filesystem::path filePath(
            Backend::exeDirectory.string() + ProfileHandling::profileFolder + fileName
        );

        if (!filePath.has_filename())
            return false;

        if (exists(filePath))
            return false;

        std::ofstream fileToCreate(filePath);
        fileToCreate.close();

        if (exists(filePath))
        {
            ProfileHandling::selectedProfileName = fileName;
            ReloadProfiles();
            return true;
        }

        return false;
    }

    inline bool DeleteProfile(std::string fileName)
    {
        std::filesystem::path filePath(
            Backend::exeDirectory.string() + ProfileHandling::profileFolder + fileName
        );

        if (!exists(filePath))
            return false;

        if (!filePath.has_filename())
            return false;

        const bool removed = std::filesystem::remove(filePath);

        if (!removed)
            return false;

        if (ProfileHandling::loadedProfileName == ProfileHandling::selectedProfileName)
            ProfileHandling::loadedProfileName = "";


        ProfileHandling::selectedProfileName = "";
        ProfileHandling::currentSelectedProfile = -1;

        ReloadProfiles();

        return true;
    }

    template <typename T>
    bool SaveProfile(std::string fileName, T& cfg)
    {
        std::ofstream filePath(Backend::exeDirectory.string() + ProfileHandling::profileFolder + fileName,
                               std::ios::binary);

        if (!filePath.is_open())
        {
            std::cerr << "Failed to open file for writing" << std::endl;
            return false;
        }

        filePath.write(reinterpret_cast<const char*>(&cfg), sizeof(T));
        filePath.close();

        return true;
    }

    template <typename T>
    bool LoadProfile(std::string fileName, T& cfg)
    {
        std::ifstream file(Backend::exeDirectory.string() + ProfileHandling::profileFolder + fileName,
                           std::ios::binary);

        if (!file)
            return false;


        file.read(reinterpret_cast<char*>(&cfg), sizeof(T));
        file.close();

        ProfileHandling::loadedProfile = ProfileHandling::currentSelectedProfile;
        ProfileHandling::loadedProfileName = ProfileHandling::selectedProfileName;

        return true;
    }

    template <typename T>
    bool ResetProfile(T& cfg)
    {
        Variables newCVars;
        newCVars.enabled = true;
        cfg = newCVars;

        return true;
    }
}

inline Crosshair::Variables CVars;
