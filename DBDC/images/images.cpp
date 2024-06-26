﻿#include "images.h"
#include <GLFW/GL/glew.h>
#include "menu/menu.hpp"

#include "icons\Tabs\back_icon.hpp"
#include "icons\Tabs\config_editor.hpp"
#include "icons\discord_icon.hpp"
#include "icons\Tabs\settings_icon.hpp"
#include "icons\Tabs\crosshair_overlay.hpp"
#include "icons\Tabs\icon_packager.hpp"
#include "icons\Tabs\on_screen_timers.hpp"
#include "updater\update_icon.hpp"

#define STB_IMAGE_IMPLEMENTATION    
#include "../dependencies/stb_image.h"
#include "icons\x_icon.hpp"
#include "icons\Tabs\hook_tracker.hpp"

void images::load_all()
{
    images::load_texture_from_memory(update_icon_raw_data, sizeof update_icon_raw_data, &menu::icons::update_icon);
    images::load_texture_from_memory(settings_icon_raw_data, sizeof settings_icon_raw_data, &menu::icons::settings_icon);
    images::load_texture_from_memory(back_icon_raw_data, sizeof back_icon_raw_data, &menu::icons::back_icon);
    images::load_texture_from_memory(discord_icon_raw_data, sizeof discord_icon_raw_data, &menu::icons::discord_icon);
    
    images::load_texture_from_memory(config_editor_icon_raw_data, sizeof config_editor_icon_raw_data, &menu::icons::config_editor_icon);
    images::load_texture_from_memory(icon_packager_raw_data, sizeof icon_packager_raw_data, &menu::icons::icon_packager_icon);
    images::load_texture_from_memory(hook_tracker_icon_raw_data, sizeof hook_tracker_icon_raw_data, &menu::icons::hook_tracker_icon);
    images::load_texture_from_memory(crosshair_raw_data, sizeof crosshair_raw_data, &menu::icons::crosshair_overlay_icon);
    images::load_texture_from_memory(on_screen_timers_raw_data, sizeof on_screen_timers_raw_data, &menu::icons::on_screen_timers_icon);
    images::load_texture_from_memory(x_icon_raw_data, sizeof x_icon_raw_data, &menu::icons::x_icon);
}

bool images::load_texture_from_memory(const unsigned char* image_array, int imageSize, unsigned int* out_texture)
{
    int image_width = 0;
    int image_height = 0;

    unsigned char* image_data = stbi_load_from_memory(image_array, imageSize, &image_width, &image_height, NULL, 4);

    if (image_data == NULL)
        return false;

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}

bool images::load_texture_from_file(std::string file_name, unsigned* out_texture)
{
    int image_width = 0;
    int image_height = 0;

    // Use stbi_load_from_memory to load image from the byte array
    unsigned char* image_data = stbi_load(file_name.c_str(), &image_width, &image_height, NULL, 0);

    if (image_data == NULL)
        return false;

    // Create an OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);

    *out_texture = image_texture;

    return true;
}
