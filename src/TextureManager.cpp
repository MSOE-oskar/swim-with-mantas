#include "TextureManager.hpp"
#include "stb_image/stb_image.hpp"
#include <glad/glad.h>
#include <iostream>

TextureManager *TextureManager::instance = nullptr;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for (const auto &pair : textureMap)
    {
        glDeleteTextures(1, &pair.second);
    }
    delete instance;
    instance = nullptr;
}

TextureManager *TextureManager::getInstance()
{
    if (!instance)
    {
        instance = new TextureManager();
    }
    return instance;
}

void TextureManager::init()
{
    // Load in textures here so we only do it once and can reuse them across scenes.
    loadTexture("sand", "../textures/sand.png", true);
    loadTexture("water", "../textures/water.png", true);
    loadTexture("waterNormal1", "../textures/waternormal1.jpg", false);
    loadTexture("waterNormal2", "../textures/waternormal2.png", true);
    loadCubemap("skybox", {"../textures/skybox/jettelly_sunshine_BACK.png",
                           "../textures/skybox/jettelly_sunshine_LEFT.png",
                           "../textures/skybox/jettelly_sunshine_UP.png",
                           "../textures/skybox/jettelly_sunshine_DOWN.png",
                           "../textures/skybox/jettelly_sunshine_FRONT.png",
                           "../textures/skybox/jettelly_sunshine_BACK.png"});
}

bool TextureManager::loadTexture(const std::string &name, const std::string &path, bool alpha)
{
    if (textureMap.find(name) != textureMap.end())
    {
        return false; // Texture already exists
    }
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
        return false; // Failed to load texture
    }

    // The current texture is still bound to GL_TEXTURE_2D, so we can set parameters on it.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // free the memory from reading the image. we made the texture already.
    stbi_image_free(data);

    textureMap[name] = textureID;
    return true;
}

bool TextureManager::loadCubemap(const std::string &name, const std::vector<std::string> &faces)
{
    if (textureMap.find(name) != textureMap.end())
    {
        std::cout << "Texture " << name << " already exists." << std::endl;
        return false; // Texture already exists
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

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
            return false; // Failed to load cubemap
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textureMap[name] = textureID;
    return true;
}

unsigned int TextureManager::getTexture(const std::string &name) const
{
    auto it = textureMap.find(name);
    if (it != textureMap.end())
    {
        return it->second;
    }

    std::cout << "Texture " << name << " not found." << std::endl;
    return 0;
}
