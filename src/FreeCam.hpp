//
// Created by sierzegao on 2026-03-17.
//
#ifndef FREECAM_H
#define FREECAM_H

#include "Camera.hpp"
#include "Player.hpp"
#include <glm/glm.hpp>

/**
 * @brief A simple free camera that can be used for debugging and testing.
 */
class FreeCam
{
public:
    Camera camera;

    FreeCam(glm::vec3 initialPos, glm::vec3 up, float yaw, float pitch);
    ~FreeCam();

    void ProcessKeyboard(const Player_movement direction, const float deltaTime);
    void ProcessMouseMovement(float xpos, float ypos, const GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    static float SPEED;
    static float MouseSensitivity;

    // for mouse movement
    float lastX, lastY;
    bool firstMouse = true;
};

#endif // FREECAM_H