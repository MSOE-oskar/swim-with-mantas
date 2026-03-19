//
// Created by sierzegao on 2026-03-17.
//
#ifndef WATERSCENE_H
#define WATERSCENE_H

#include "Scene.hpp"
#include "FreeCam.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class WaterScene : public Scene
{
public:
    WaterScene();
    ~WaterScene();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void renderDebug() override;
    void cleanup() override;

    void processInput(GLFWwindow *window, float deltaTime) override;
    void onMouseMove(GLFWwindow *window, double xpos, double ypos) override;
    void onMouseScroll(GLFWwindow *window, double xoffset, double yoffset) override;

private:
    static glm::vec3 BACKGROUND_COLOR;
    // water uniforms
    static float WATER_HEIGHT;
    static int OCTAVES;
    static float AMPLITUDE[8];
    static float WAVELENGTH[8];
    static float SPEED[8];
    static glm::vec2 DIRECTION[8];
    // light uniforms
    static glm::vec4 LIGHT_DIRECTION;
    static glm::vec3 LIGHT_COLOR;
    static glm::vec3 AMBIENT;
    static glm::vec3 DIFFUSE;
    static glm::vec3 SPECULAR;
    static float SHININESS;

    Shader *waterShader;
    unsigned int textures[1];
    FreeCam freeCam;

    Mesh *waterMesh;
};

#endif // WATERSCENE_H