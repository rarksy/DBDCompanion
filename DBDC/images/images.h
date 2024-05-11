#pragma once

#include <string>

namespace images
{
    void load_all();

    bool load_texture_from_memory(const unsigned char* image_array, int imageSize, unsigned int* out_texture);
    bool load_texture_from_file(std::string file_name, unsigned int* out_texture);
}
