//
// Created by sierzegao on 4/5/2025.
//

#include <string>
#include <vector>

#include "Mesh.hpp"

void Mesh::clearVertices()
{
    vertices.clear();
}

void Mesh::addVertex(Vertex vertex)
{
    vertices.push_back(vertex);
}

// this method puts the vertice data into the VBO back onto GPU.
// we should call this after we are done updating a mesh with new vertices.
// better to do this seperately rather than after EACH vertex add.
void Mesh::recreateVBO() const
{
    // recreate VBO data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void Mesh::setupMesh()
{
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0); // we are jumping over sizeof(Vertex) each time
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    // normals attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));

    // color attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));

    // texture attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::draw() const
{
    // bind textures
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        // activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->textures = textures;

    setupMesh();
}

Mesh::Mesh(std::vector<Texture> textures)
{
    this->textures = textures;

    setupMesh();
}

Mesh::Mesh()
{
    setupMesh();
}
