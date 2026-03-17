
#include <vector>
#include "Scene.hpp"

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

/**
 * @brief The SceneManager class is responsible for managing different scenes in the application.
 *        It holds a list of scenes and keeps track of the current active scene.
 *        It is also in charge of changing scenes and calling the appropriate methods
 *        (init, update, render, cleanup) on the current scene.
 */
class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    /**
     * @brief Initializes the scene manager.
     *
     * Creates all the scenes and sets the current scene to the first one.
     */
    void init();

    /**
     * @brief Updates the current scene.
     *
     * Calls the update method of the current scene with the given delta time.
     *
     * @param deltaTime The time elapsed since the last frame, used for consistent movement and animations.
     */
    void update(float deltaTime);

    /**
     * @brief Renders the current scene.
     *
     * Calls the render method of the current scene to draw it on the screen.
     */
    void render();

    /**
     * @brief Cleans up the scene manager.
     *
     * Calls the cleanup method of the current scene and releases any allocated resources.
     */
    void cleanup();

    /**
     * @brief Processes input for the current scene.
     * Calls the processInput method of the current scene with the given window and delta time.
     * @param window The GLFW window to check for input events.
     * @param deltaTime The time elapsed since the last frame, used for consistent input handling.
     */
    void processInput(GLFWwindow *window, float deltaTime);
    /**
     * @brief Handles mouse movement input for the current scene.
     * Calls the onMouseMove method of the current scene with the given window and mouse position
     * @param window The GLFW window to check for mouse movement events.
     * @param xpos The new x-coordinate of the mouse cursor.
     * @param ypos The new y-coordinate of the mouse cursor.
     */
    void onMouseMove(GLFWwindow *window, double xpos, double ypos);
    /**
     * @brief Handles mouse scroll input for the current scene.
     * Calls the onMouseScroll method of the current scene with the given window and scroll offsets
     * @param window The GLFW window to check for mouse scroll events.
     * @param xoffset The scroll offset in the x-direction (horizontal scroll).
     * @param yoffset The scroll offset in the y-direction (vertical scroll).
     */
    void onMouseScroll(GLFWwindow *window, double xoffset, double yoffset);

private:
    Scene *currentScene;
    std::vector<Scene *> scenes;
};

#endif // SCREENMANAGER_H