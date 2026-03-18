//
// Created by sierzegao on 3/18/2025.
//
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "AxisAlignedBoundingBox.hpp"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Player_movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Player
{
public:
    glm::vec3 Position;
    glm::vec3 CurrentVelocity;

    Camera camera;
    AxisAlignedBoundingBox AABB;

    float CurrentSpeed;
    float MovementSpeed; // speed we move at

    Player(glm::vec3 initialPos = glm::vec3(0.0f), float movementSpeed = 2.5f);
    ~Player();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(const Player_movement direction, const float deltaTime);

    // processes input received from a mouse input system.
    // Expects the xpos and ypos of the mouse.
    void ProcessMouseMovement(float xpos, float ypos, const GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event.
    // Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    void UpdatePlayer(const float deltaTime, const std::vector<glm::vec3> &collisions);

private:
    static float GRAVITY;
    static float MouseSensitivity;

    // for mouse movement
    float lastX, lastY;
    bool firstMouse = true;
};
#endif // PLAYER_H
