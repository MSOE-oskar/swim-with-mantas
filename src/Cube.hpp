//
// Created by sierzegao on 3/18/2025.
//

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AxisAlignedBoundingBox.hpp"

#ifndef CUBE_H
#define CUBE_H

class Cube
{
public:
    // static methods to get in the mood to draw cubes.
    static bool readyToDraw;
    static void bindOptions();

    // create a new cube.
    Cube(glm::vec3 position, glm::vec3 scale, unsigned int textures[]);

    glm::vec3 checkCollision(AxisAlignedBoundingBox *other) const;
    glm::vec3 getPosition() const;
    glm::vec3 getScale() const;
    unsigned int *getTextures() const;

    AxisAlignedBoundingBox *AABB;

private:
    // for opengl drawing
    static unsigned int VAO, VBO;
    static float vertices[180];

    glm::vec3 position{};
    glm::vec3 scale{};
    unsigned int *textures;
};

#endif // CUBE_H
