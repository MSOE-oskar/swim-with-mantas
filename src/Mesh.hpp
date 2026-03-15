//
// Created by sierzegao on 4/5/2025.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include "Shader.hpp"

#ifndef MESH_H
#define MESH_H

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec4 Color;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;

    Mesh();
    Mesh(std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures);

    /**
     * @brief Adds a vertex to the mesh.
     *        Remember to call recreateVBO() after you are done adding vertices.
     * @param vertex The vertex to add to the mesh.
     */
    void addVertex(Vertex vertex);

    /**
     * @brief Clears the vertices from the mesh.
     *        Remember to call recreateVBO() after.
     */
    void clearVertices();

    void recreateVBO() const;
    void draw() const;

private:
    unsigned int VAO, VBO;
    void setupMesh();
};

#endif // MESH_H
