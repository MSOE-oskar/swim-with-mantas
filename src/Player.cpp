#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "AxisAlignedBoundingBox.hpp"
#include "Player.hpp"

float Player::GRAVITY = 1.0f;
float Player::MouseSensitivity = 0.1f;

Player::Player(glm::vec3 initialPos, float movementSpeed)
    : Position(initialPos),
      MovementSpeed(movementSpeed),
      CurrentVelocity(glm::vec3(0.0f)),
      camera(initialPos),
      // this just creates the bounding box to be like 1 bigger than the guy. we should change this.
      AABB{Position + glm::vec3(0.5f, 0.5f, 0.5f), Position - glm::vec3(0.5f, 0.5f, 0.5f)}
{
}

Player::~Player()
{
}

void Player::ProcessKeyboard(const Player_movement direction, const float deltaTime)
{
    glm::vec3 Front = camera.getFront();
    glm::vec3 Forward = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 Right = camera.getRight();
    glm::vec3 Up = camera.getWorldUp();

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

    CurrentVelocity += acceleration * 5.0f * deltaTime;
}

void Player::ProcessMouseMovement(float xpos, float ypos, const GLboolean constrainPitch)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y goes from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    float newPitch = camera.getPitch() + yoffset;
    float newYaw = camera.getYaw() + xoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (newPitch > 89.0f)
            newPitch = 89.0f;
        if (newPitch < -89.0f)
            newPitch = -89.0f;
    }

    camera.setEulerAngles(newYaw, newPitch);
}

void Player::ProcessMouseScroll(float yoffset)
{
    camera.Zoom -= yoffset;
    if (camera.Zoom < 1.0f)
        camera.Zoom = 1.0f;
    if (camera.Zoom > 90.0f)
        camera.Zoom = 90.0f;
}

void Player::UpdatePlayer(const float deltaTime, const std::vector<glm::vec3> &collisions)
{
    // slow down when not pressing buttons
    float dragX = 0.5f * CurrentVelocity.x * CurrentVelocity.x * deltaTime;
    float dragY = 0.5f * CurrentVelocity.y * CurrentVelocity.y * deltaTime;
    float dragZ = 0.5f * CurrentVelocity.z * CurrentVelocity.z * deltaTime;

    CurrentVelocity.x -= (CurrentVelocity.x > 0) ? dragX : -dragX;
    CurrentVelocity.y -= (CurrentVelocity.y > 0) ? dragY : -dragY;
    CurrentVelocity.z -= (CurrentVelocity.z > 0) ? dragZ : -dragZ;

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
    camera.Position = Position;
    // this seems bad to update the max and min individually.
    // we should have calcuations to update this automatically
    // based on the size and position of the thing.
    // just for now..
    AABB.max = Position + glm::vec3(0.5f, 0.5f, 0.5f);
    AABB.min = Position - glm::vec3(0.5f, 0.5f, 0.5f);

    CurrentVelocity.y -= GRAVITY * deltaTime;
}