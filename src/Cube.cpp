//
// Created by sierzegao on 3/18/2025.
//

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Cube.hpp"
#include "Mesh.hpp"
#include "AxisAlignedBoundingBox.hpp"

// cube vertices as Vertex structs with positions, normals, colors, and texture coords
std::vector<Vertex> Cube::generateVertices()
{
    const glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    return {
        // +Z
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, white, {0.0f, 0.0f}},

        // -Z
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {0.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {0.0f, 1.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, white, {0.0f, 0.0f}},

        // +X
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, white, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, white, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, white, {0.0f, 1.0f}},
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, white, {0.0f, 0.0f}},

        // -X
        {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, white, {0.0f, 0.0f}},
        {{0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, white, {1.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, white, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, white, {0.0f, 0.0f}},

        // +Y
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, white, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, white, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, white, {0.0f, 0.0f}},
        {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, white, {0.0f, 1.0f}},

        // -Y
        {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, white, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, white, {1.0f, 0.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, white, {1.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, white, {0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, white, {0.0f, 0.0f}},
    };
}

Cube::Cube(const glm::vec3 position, const glm::vec3 scale, std::vector<Texture> textures)
{
    this->position = position;
    this->scale = scale;

    this->mesh = new Mesh(generateVertices(), textures);
    this->AABB = new AxisAlignedBoundingBox(
        // max
        position + scale,
        // min
        position);
}

Cube::~Cube()
{
    delete mesh;
    delete AABB;
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

void Cube::draw() const
{
    this->mesh->draw();
}