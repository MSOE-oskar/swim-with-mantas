//
// Created by sierzegao on 3/16/2026.
//
#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "Scene.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "FastNoiseLite/FastNoiseLite.h"
#include "Shader.hpp"
#include "Cube.hpp"
#include "Chunk.hpp"

class MainScene : public Scene
{
public:
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
    Player *player;
    Camera *camera;
    FastNoiseLite noise;
    Shader *ourShader;
    unsigned int textures[1];
    Cube *cubes[1];
    Chunk *chunks[4];

    // for mouse movement
    float lastX, lastY;
    bool firstMouse = true;
};

#endif // MAINSCENE_H