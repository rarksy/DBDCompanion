#pragma once

#include <string>

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

namespace images
{
    bool load_texture_from_memory(const unsigned char* image_array, int imageSize, unsigned int* out_texture);
    bool load_texture_from_file(std::string file_name, unsigned int* out_texture);
}