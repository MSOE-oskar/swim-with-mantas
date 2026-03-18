//
// Created by sierzegao on 3/18/2025.
//

#ifndef AXISALIGNEDBOUNDINGBOX_H
#define AXISALIGNEDBOUNDINGBOX_H

#include <glm/glm.hpp>

class AxisAlignedBoundingBox
{
public:
    glm::vec3 max;
    glm::vec3 min;

    AxisAlignedBoundingBox(glm::vec3 max = glm::vec3(0), glm::vec3 min = glm::vec3(0)) : max(max), min(min) {}

    /**
     *  @brief Checks if this AABB is colliding with another AABB and returns the collision vector.
     *  @param that The other AABB to check collision with.
     *  @return The collision vector indicating the depth of the collision along each axis.
     */
    glm::vec3 checkCollision(const AxisAlignedBoundingBox *that) const;
};

#endif // AXISALIGNEDBOUNDINGBOX_H
