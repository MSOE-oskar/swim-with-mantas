//
// Created by sierzegao on 3/18/2025.
//

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Cube.hpp"
#include "AxisAlignedBoundingBox.hpp"

// vertices for a cube
//  x, y, z        s, t
float Cube::vertices[180] = {
    // positions         // texture coords
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

unsigned int Cube::VAO = 0;
unsigned int Cube::VBO = 0;
bool Cube::readyToDraw = false;

/**
 * This binds the VBO, VAO, and EBO so we can draw cubes.
 * Of course we can call this a lot but should probably only call once before we wanna draw
 * any cubes, for performance purposes.
 */
void Cube::bindOptions()
{
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    readyToDraw = true;
}

Cube::Cube(const glm::vec3 position, const glm::vec3 scale, unsigned int textures[])
{
    this->position = position;
    this->scale = scale;
    this->textures = textures;

    this->AABB = new AxisAlignedBoundingBox(
        // max
        position + scale,
        // min
        position);
}

glm::vec3 Cube::checkCollision(AxisAlignedBoundingBox *other) const
{
    return this->AABB->checkCollision(other);
}

glm::vec3 Cube::getPosition() const
{
    return this->position;
}

glm::vec3 Cube::getScale() const
{
    return this->scale;
}

unsigned int *Cube::getTextures() const
{
    return this->textures;
}
