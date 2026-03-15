//
// Created by sierzegao on 3/15/2026.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FastNoiseLite/FastNoiseLite.h"

#include "Mesh.hpp"

class Chunk
{
public:
    /**
     * @brief The size of the chunk in world units.
     *        chunks are CHUNK_SIZE by CHUNK_SIZE in the xz plane
     */
    static float CHUNK_SIZE;
    /**
     * @brief The maximum height of the terrain in the chunk.
     *        This is multiplied by the noise value to get the final height of each vertex.
     */
    static float VERTICALITY;

    /**
     * @brief Creates a new chunk at the given position with the given noise generator.
     * @param position The position of the chunk in world space.
     * @param noise The noise generator to use for generating the chunk's terrain.
     */
    Chunk(glm::vec3 position, std::vector<Texture> textures, FastNoiseLite *noise);
    ~Chunk();
    glm::vec3 getPosition() const;

    /**
     * @brief Generates the chunk's terrain using the noise generator,
     * filling the chunk's mesh with vertices and textures.
     */
    void generate();
    void draw() const;

private:
    glm::vec3 position{};
    Mesh *terrainMesh;
    FastNoiseLite *noise;
};

#endif // CHUNK_H
