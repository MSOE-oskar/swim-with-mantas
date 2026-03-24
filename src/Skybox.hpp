#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include "Shader.hpp"

class Skybox
{
public:
    Skybox(std::vector<std::string> faces);
    ~Skybox();

    void init(std::vector<std::string> faces);
    void render(glm::mat4 view, glm::mat4 projection);
    void cleanup();

private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTexture;
    static float vertices[];
    Shader *skyboxShader;
};

#endif // SKYBOX_H