#pragma once

#include "AntiAliasing\Off.hpp"
#include "AntiAliasing\On.hpp"
#include "ResolutionQuality\100.hpp"
#include "ResolutionQuality\80.hpp"
#include "ResolutionQuality\60.hpp"
#include "TextureQuality\Ultra.hpp"
#include "TextureQuality\High.hpp"
#include "TextureQuality\Medium.hpp"
#include "TextureQuality\Low.hpp"
#include "TextureQuality\VeryLow.hpp"

namespace Images
{
    bool LoadTextureFromMemory(const unsigned char* imageData, int imageSize, unsigned int* out_texture);
}