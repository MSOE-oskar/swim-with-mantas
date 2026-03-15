#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "AxisAlignedBoundingBox.hpp"
#include "Player.hpp"

float Player::GRAVITY = 17.8f;

void Player::ProcessKeyboard(const Player_movement direction, const float deltaTime)
{
    glm::vec3 Front = camera->Front;
    glm::vec3 Forward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 Right = camera->Right;
    glm::vec3 Up = camera->WorldUp;

    glm::vec3 acceleration = glm::vec3(0.0f);

    if (direction == FORWARD)
        acceleration += Forward;
    if (direction == BACKWARD)
        acceleration -= Forward;
    if (direction == LEFT)
        acceleration -= Right;
    if (direction == RIGHT)
        acceleration += Right;
    if (direction == UP)
        acceleration += Up;
    if (direction == DOWN)
        acceleration -= Up;

    CurrentVelocity += acceleration * 0.5f;
}

void Player::UpdatePlayer(const float deltaTime, const std::vector<glm::vec3> &collisions)
{
    // slow down when not pressing buttons
    CurrentVelocity.x *= 0.9f;
    CurrentVelocity.y *= 0.9f;
    CurrentVelocity.z *= 0.9f;

    // handle collisions
    for (const glm::vec3 collision : collisions)
    {
        // ensure we dont got no wacky length 0 collisions
        if (glm::length(collision) == 0)
            continue;

        // Find the axis with the smallest absolute penetration depth
        const float absX = std::abs(collision.x);
        const float absY = std::abs(collision.y);
        const float absZ = std::abs(collision.z);

        // X is the smallest penetration
        if (absX < absY && absX < absZ)
        {
            // Resolve in the X direction
            Position.x += collision.x;
            CurrentVelocity.x = 0;
        }
        // Y is the smallest penetration
        else if (absY < absX && absY < absZ)
        {
            // Resolve in the Y direction
            Position.y += collision.y;
            CurrentVelocity.y = 0;
        }
        // Z is the smallest penetration
        else
        {
            // Resolve in the Z direction
            Position.z += collision.z;
            CurrentVelocity.z = 0;
        }
    }

    // update position of player, camera, and aabb
    glm::vec3 newPosition = Position + CurrentVelocity * deltaTime;

    Position = newPosition;
    camera->Position = Position;
    // this seems bad to update the max and min individually.
    // we should have calcuations to update this automatically
    // based on the size and position of the thing.
    // just for now..
    AABB->max = Position + glm::vec3(0.5f, 0.5f, 0.5f);
    AABB->min = Position - glm::vec3(0.5f, 0.5f, 0.5f);

    CurrentVelocity.y -= GRAVITY * deltaTime;
}