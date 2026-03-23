//
// Created by sierzegao on 3/16/2026.
//
#ifndef LOADTEXTUREHELPERS_H
#define LOADTEXTUREHELPERS_H

#include "stb_image/stb_image.hpp"
#include <glad/glad.h>
#include <iostream>
#include <vector>

/**
 * Load a texture from an image file path and return the OpenGL texture ID.
 * @param path path of the texture
 * @param textureID id to store texture in
 * @param alpha true for pngs, false for jpgs.
 * @return the OpenGL texture ID of the loaded texture.
 */
inline unsigned int loadTextureImage(const std::string &path, const bool alpha)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

    // The current texture is still bound to GL_TEXTURE_2D, so we can set parameters on it.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // free the memory from reading the image. we made the texture already.
    stbi_image_free(data);
    return textureID;
}

/**
 * Loads a cubemap texture from 6 image file paths and returns the OpenGL texture ID.
 * @param faces vector of 6 file paths for the cubemap textures in the order:
 * +X, -X, +Y, -Y, +Z, -Z
 * @return the OpenGL texture ID of the loaded cubemap texture.
 */
inline unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

#endif // LOADTEXTUREHELPERS_H