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

    Camera *camera;
    AxisAlignedBoundingBox *AABB;

    float CurrentSpeed;
    float MovementSpeed; // speed we move at

    Player(glm::vec3 initialPos, float movementSpeed, float sprintSpeed, float jumpHeight)
        : Position(initialPos),
          MovementSpeed(movementSpeed),
          CurrentVelocity(glm::vec3(0.0f))
    {
        this->camera = new Camera(initialPos);
        // this just creates the bounding box to be like 1 bigger than the guy. we should change this.
        this->AABB = new AxisAlignedBoundingBox(
            initialPos + glm::vec3(0.5f, 0.5f, 0.5f),
            initialPos - glm::vec3(0.5f, 0.5f, 0.5f));
    }

    ~Player()
    {
        delete camera;
        delete AABB;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(const Player_movement direction, const float deltaTime);

    void UpdatePlayer(const float deltaTime, const std::vector<glm::vec3> &collisions);

private:
    static float GRAVITY;
};
#endif // PLAYER_H
