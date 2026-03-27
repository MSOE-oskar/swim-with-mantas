#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <vector>
#include <string>
#include <map>

class TextureManager
{
public:
    static TextureManager *getInstance();
    void init();
    /**
     * Load a texture from an image file path and return the OpenGL texture ID.
     * @param path path of the texture
     * @param textureID id to store texture in
     * @param alpha true for pngs, false for jpgs.
     * @return was successful in loading the texture.
     */
    bool loadTexture(const std::string &name, const std::string &path, bool alpha);
    /**
     * Loads a cubemap texture from 6 image file paths and returns the OpenGL texture ID.
     * @name name to identify the texture in the manager
     * @param faces vector of 6 file paths for the cubemap textures in the order:
     * +X, -X, +Y, -Y, +Z, -Z
     * @return was successful in loading the cubemap texture.
     */
    bool loadCubemap(const std::string &name, const std::vector<std::string> &faces);
    /**
     * Get the OpenGL texture ID of a loaded texture by name.
     * @param name name of the texture to retrieve
     * @return the OpenGL texture ID of the texture, or 0 if not found.
     */
    unsigned int getTexture(const std::string &name) const;

private:
    static TextureManager *instance;
    std::map<std::string, unsigned int> textureMap;

    TextureManager();
    ~TextureManager();
};

#endif // TEXTURE_MANAGER_HPP