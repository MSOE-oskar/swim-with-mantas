//
// Created by sierzegao on 3/18/2025.
//
#ifndef CUBE_H
#define CUBE_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AxisAlignedBoundingBox.hpp"
#include "Mesh.hpp"

class Cube
{
public:
    // create a new cube.
    Cube(glm::vec3 position, glm::vec3 scale, std::vector<Texture> textures);
    ~Cube();

    glm::vec3 checkCollision(AxisAlignedBoundingBox *other) const;
    glm::vec3 getPosition() const;
    glm::vec3 getScale() const;

    void draw() const;

    AxisAlignedBoundingBox *AABB;

private:
    // for opengl drawing
    static std::vector<Vertex> generateVertices();

    Mesh *mesh;
    glm::vec3 position{};
    glm::vec3 scale{};
};

#endif // CUBE_H
