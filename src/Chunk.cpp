
#include "Chunk.hpp"

float Chunk::CHUNK_SIZE = 16.0f;
float Chunk::VERTICALITY = 20.0f;

Chunk::Chunk(glm::vec3 position, std::vector<Texture> textures, FastNoiseLite *noise)
    : position(position), noise(noise)
{
    terrainMesh = new Mesh();
    terrainMesh->textures = textures;
}

Chunk::~Chunk()
{
    delete terrainMesh;
}

glm::vec3 Chunk::getPosition() const
{
    return position;
}

void Chunk::generate()
{
    const float chunkX = position.x * CHUNK_SIZE;
    const float chunkZ = position.z * CHUNK_SIZE;
    for (float x = 0; x < CHUNK_SIZE; x++)
    {
        for (float z = 0; z < CHUNK_SIZE; z++)
        {
            // bot left
            Vertex blVert = {
                glm::vec3(
                    x,
                    noise->GetNoise(chunkX + x, chunkZ + z) * VERTICALITY,
                    z),                      // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                glm::vec4(1.0f),             // Color
                glm::vec2(0.0f, 0.0f)        // TexCoords
            };
            // bot right
            Vertex brVert = {
                glm::vec3(
                    x + 1.0f,
                    noise->GetNoise(chunkX + x + 1.0f, chunkZ + z) * VERTICALITY,
                    z),                      // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                glm::vec4(1.0f),             // Color
                glm::vec2(1.0f, 0.0f)        // TexCoords
            };
            // top left
            Vertex tlVert = {
                glm::vec3(
                    x,
                    noise->GetNoise(chunkX + x, chunkZ + z + 1.0f) * VERTICALITY,
                    z + 1.0f),               // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                glm::vec4(1.0f),             // Color
                glm::vec2(0.0f, 1.0f)        // TexCoords
            };
            // top right
            Vertex trVert = {
                glm::vec3(
                    x + 1.0f,
                    noise->GetNoise(chunkX + x + 1.0f, chunkZ + z + 1.0f) * VERTICALITY,
                    z + 1.0f),               // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                glm::vec4(1.0f),             // Color
                glm::vec2(1.0f, 1.0f)        // TexCoords
            };

            // first triangle
            terrainMesh->addVertex(blVert);
            terrainMesh->addVertex(brVert);
            terrainMesh->addVertex(tlVert);
            // second triangle
            terrainMesh->addVertex(brVert);
            terrainMesh->addVertex(tlVert);
            terrainMesh->addVertex(trVert);
            // Basically we're generating a square for each unit in the chunk.
            // Square = 2 triangles = 6 vertices
        }
    }

    terrainMesh->recreateVBO();
}

void Chunk::draw() const
{
    terrainMesh->draw();
}