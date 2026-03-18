#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

glm::vec3 Camera::getFront() const
{
    return Front;
};
glm::vec3 Camera::getUp() const
{
    return Up;
};
glm::vec3 Camera::getRight() const
{
    return Right;
};
glm::vec3 Camera::getWorldUp() const
{
    return WorldUp;
};

float Camera::getYaw() const
{
    return Yaw;
};
float Camera::getPitch() const
{
    return Pitch;
};
void Camera::setEulerAngles(float newYaw, float newPitch)
{
    Yaw = newYaw;
    Pitch = newPitch;
    updateCameraVectors();
};

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}