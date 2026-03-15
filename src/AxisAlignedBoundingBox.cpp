#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AxisAlignedBoundingBox.hpp"

glm::vec3 AxisAlignedBoundingBox::checkCollision(const AxisAlignedBoundingBox *that) const
{
    glm::vec3 toReturn = glm::vec3(0.0f);
    // Start with the assumption that the two are colliding.
    // Then, if we are not colliding on at least one axis, then we aren't colliding.
    if (this->min.x >= that->max.x || //  max------|
        this->max.x <= that->min.x || //   |  this |
        this->min.y >= that->max.y || //   |------min
        this->max.y <= that->min.y || //               max------|
        this->min.z >= that->max.z || //                | that  |
        this->max.z <= that->min.z)   //                |------min
        return toReturn;

    // now we know that we are colliding.
    // return the vector of the collision; how deep we are into the box.
    glm::vec3 temp1 = that->max - this->min;
    glm::vec3 temp2 = that->min - this->max;

    // we want the collision on the right axis for x, y, and z.
    toReturn.x = (abs(temp1.x) < abs(temp2.x)) ? temp1.x : temp2.x;
    toReturn.y = (abs(temp1.y) < abs(temp2.y)) ? temp1.y : temp2.y;
    toReturn.z = (abs(temp1.z) < abs(temp2.z)) ? temp1.z : temp2.z;
    return toReturn;
}