#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @brief An abstract class representing a scene in the application.
 *        Each scene should implement the init, update, render, and cleanup methods.
 *
 * My idea is that each scene will handle it's own input, textures, shaders,
 * rendering, etc. Then, the scene manager will just call the appropriate methods on the current scene.
 * This way, we can easily switch between different scenes (like a menu vs game screen)
 */
class Scene
{
public:
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    virtual void processInput(GLFWwindow *window, float deltaTime) = 0;
    virtual void onMouseMove(GLFWwindow *window, double xpos, double ypos) = 0;
    virtual void onMouseScroll(GLFWwindow *window, double xoffset, double yoffset) = 0;
};

#endif // SCENE_H