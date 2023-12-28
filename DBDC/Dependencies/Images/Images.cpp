#include "Images.h"
#include <GLFW/GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION    
#include "../Dependencies/stb_image.h"

bool Images::LoadTextureFromMemory(const unsigned char* imageData, int imageSize, unsigned int* out_texture)
{
    int image_width = 0;
    int image_height = 0;
    
    // Use stbi_load_from_memory to load image from the byte array
    unsigned char* image_data = stbi_load_from_memory(imageData, imageSize, &image_width, &image_height, NULL, 4);

    if (image_data == NULL)
        return false;

    // Create an OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}