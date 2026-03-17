//
// Created by sierzegao on 3/16/2026.
//
#ifndef LOADTEXTUREIMAGE_H
#define LOADTEXTUREIMAGE_H

#include "stb_image/stb_image.hpp"
#include <glad/glad.h>
#include <iostream>

/**
 * Load a texture from the thing
 * @param path path of the texture
 * @param textureID id to store texture in
 * @param alpha true for pngs, false for jpgs.
 */
inline void loadTextureImage(const std::string &path, const unsigned int textureID, const bool alpha)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Load in image with STB_IMAGE what a goat
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels; // nrChannels = number of color channels.
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // Set the image to the currently bound texture! yay!
        // most of these parameters will stay the same for other textures i got a feeling.
        // esp if we use STB_IMAGE to load images.
        if (alpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // creates a mipmap.
        // Mipmap makes it easier for openGL to sample color from a texture
        // when it is far away.
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    // free the memory from reading the image. we made the texture already.
    stbi_image_free(data);
}

#endif // LOADTEXTUREIMAGE_H