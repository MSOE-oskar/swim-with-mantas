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
    MainScene();
    ~MainScene();

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
    Player player;
    FastNoiseLite noise;

    // rule of thumb: anything related to rendering on screen in a scene
    // should be a pointer. This is because the construction of these objects
    // involves opengl calls that require a valid OpenGL context, which we don't
    // have until we call the init method.
    Shader *ourShader;
    unsigned int textures[1];
    Cube *cubes[1];
    Chunk *chunks[4];

    // for mouse movement
    float lastX, lastY;
    bool firstMouse = true;
};

#endif // MAINSCENE_H