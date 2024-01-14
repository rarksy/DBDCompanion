#pragma once
#include <vector>
#include <string>
#include "GLFW/glfw3.h"

namespace CEMenu
{
    inline bool isSetup = false;
    
    bool Setup();
    void RenderUI();
    void CreateStyle();

    namespace Image
    {
        namespace AntiAliasing
        {
            inline GLuint textureOn;
            inline GLuint textureOff;
        }

        namespace ResolutionQuality
        {
            inline GLuint texture60;
            inline GLuint texture80;
            inline GLuint texture100;
            inline GLuint texture120;
            inline GLuint texture135;
        }

        namespace TextureQuality
        {
            inline GLuint textureVeryLow;
            inline GLuint textureLow;
            inline GLuint textureMedium;
            inline GLuint textureHigh;
            inline GLuint textureUltra;

            inline std::vector<GLuint*> allTextures = {
            
                &textureVeryLow,
                &textureLow,
                &textureMedium,
                &textureHigh,
                &textureUltra,
            };
        }
    }

    inline std::vector<std::string> windowModes = {"Fullscreen", "Borderless", "Windowed"};
    inline std::vector<std::string> qualities = {"Very Low", "Low", "Medium", "High", "Ultra"};
    inline std::vector<std::string> qualitiesWithOff = {"Off", "Very Low", "Low", "Medium", "High", "Ultra"};
    inline std::vector<std::string> fpsLimitModes = {"30", "59", "60", "100", "120"};
}
