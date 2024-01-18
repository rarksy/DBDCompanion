#pragma once
#include "ImGui/imgui.h"
#include "../Backend/Backend.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace Crosshair
{
    void Setup();
    void DrawCrosshair();
    void DrawLines();
    void DrawOutline();
    void DrawCenterDot();

    void ModifyDynamicCenterPoint();
    

    struct Variables
    {
        bool setCustom = false;

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

        ImVec2 screenCenterPoint;
        ImVec2 trueScreenCenterPoint;
        ImVec2 savedScreenCenterPoint;
        
        bool useDynamicCenterPoint = false;
        int dynamicCenterPointIndex = 0;
        
        template<typename T>
        bool Save(T& cfg)
        {
            std::ofstream filePath(std::filesystem::current_path().string() + "CrosshairSettings", std::ios::binary);

            if (!filePath.is_open())
            {
                std::cerr << "Failed to open file for writing" << std::endl;
                return false;
            }
            
            filePath.write(reinterpret_cast<const char*>(&cfg), sizeof(T));
            filePath.close();
            
            std::cout << "Saved Config" << std::endl;
            return true;
        }

        template<typename T>
        bool Load(T& cfg)
        {
            std::ifstream file(std::filesystem::current_path().string() + "CrosshairSettings", std::ios::binary);
    
            if (!file) 
            {
                std::cerr << "Failed to open file for reading" << std::endl;
                return false;
            }
            
            file.read(reinterpret_cast<char*>(&cfg), sizeof(T));
            file.close();
            
            std::cout << "Loaded Config" << std::endl;
            return true;
        }

        template<typename T>
        bool Reset(T& cfg)
        {
            Variables newCVars;
            newCVars.enabled = true;
            newCVars.screenCenterPoint = ImVec2(Backend::screenWidth / 2, Backend::screenHeight / 2);
            cfg = newCVars;

            return true;
        }

    };
}

inline Crosshair::Variables CVars;