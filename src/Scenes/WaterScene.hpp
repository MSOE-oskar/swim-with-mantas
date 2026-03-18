//
// Created by sierzegao on 2026-03-17.
//
#ifndef WATERSCENE_H
#define WATERSCENE_H

#include "Scene.hpp"
#include "FreeCam.hpp"
#include "Shader.hpp"
#include "Cube.hpp"

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
    Shader *waterShader;
    unsigned int textures[1];
    FreeCam freeCam;

    Cube *cube;
};

#endif // WATERSCENE_H