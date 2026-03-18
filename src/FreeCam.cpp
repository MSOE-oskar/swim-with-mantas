//
// Created by sierzegao on 2026-03-17.
//
#include "FreeCam.hpp"
#include "Player.hpp"

float FreeCam::SPEED = 10.0f;
float FreeCam::MouseSensitivity = 0.1f;

FreeCam::FreeCam(glm::vec3 initialPos, glm::vec3 up, float yaw, float pitch)
    : camera(initialPos, up, yaw, pitch)
{
}

FreeCam::~FreeCam()
{
}

void FreeCam::ProcessKeyboard(const Player_movement direction, const float deltaTime)
{
    glm::vec3 forward = glm::cross(camera.getWorldUp(), camera.getRight());

    if (direction == FORWARD)
        camera.Position += forward * SPEED * deltaTime;
    if (direction == BACKWARD)
        camera.Position -= forward * SPEED * deltaTime;
    if (direction == LEFT)
        camera.Position -= camera.getRight() * SPEED * deltaTime;
    if (direction == RIGHT)
        camera.Position += camera.getRight() * SPEED * deltaTime;
    if (direction == UP)
        camera.Position += camera.getWorldUp() * SPEED * deltaTime;
    if (direction == DOWN)
        camera.Position -= camera.getWorldUp() * SPEED * deltaTime;
}

void FreeCam::ProcessMouseMovement(float xpos, float ypos, const GLboolean constrainPitch)
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

void FreeCam::ProcessMouseScroll(float yoffset)
{
    camera.Zoom -= yoffset;
    if (camera.Zoom < 1.0f)
        camera.Zoom = 1.0f;
    if (camera.Zoom > 90.0f)
        camera.Zoom = 90.0f;
}