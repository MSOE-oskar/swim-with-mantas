
#include "Chunk.hpp"

float Chunk::CHUNK_SIZE = 16.0f;
float Chunk::STEP = 0.5f;
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
    for (float x = 0; x < CHUNK_SIZE; x += STEP)
    {
        for (float z = 0; z < CHUNK_SIZE; z += STEP)
        {
            // bl = bottom left, etc.
            float blY = noise->GetNoise(chunkX + x, chunkZ + z) * VERTICALITY;
            float brY = noise->GetNoise(chunkX + x + STEP, chunkZ + z) * VERTICALITY;
            float tlY = noise->GetNoise(chunkX + x, chunkZ + z + STEP) * VERTICALITY;
            float trY = noise->GetNoise(chunkX + x + STEP, chunkZ + z + STEP) * VERTICALITY;

            // calculate normals, use the right hand rule!!
            // take the two sides of a triangle to be two vectors,
            // then take the cross product.
            // Calc is finally useful...
            glm::vec3 tri1Normal = glm::normalize(glm::cross(
                glm::vec3(STEP, brY - blY, 0.0f),
                glm::vec3(0.0f, tlY - blY, STEP)));
            glm::vec3 tri2Normal = glm::normalize(glm::cross(
                glm::vec3(-STEP, tlY - trY, 0.0f),
                glm::vec3(0.0f, brY - trY, -STEP)));

            Vertex tri1vert1 = {
                glm::vec3(x, blY, z), // Position
                tri1Normal,           // Normal
                glm::vec4(1.0f),      // Color
                glm::vec2(0.0f, 0.0f) // TexCoords
            };
            Vertex tri1vert2 = {
                glm::vec3(x + STEP, brY, z),
                tri1Normal,
                glm::vec4(1.0f),
                glm::vec2(1.0f, 0.0f)};
            Vertex tri1vert3 = {
                glm::vec3(x, tlY, z + STEP),
                tri1Normal,
                glm::vec4(1.0f),
                glm::vec2(0.0f, 1.0f)};

            Vertex tri2vert1 = tri1vert2;
            tri2vert1.Normal = tri2Normal;
            Vertex tri2vert2 = tri1vert3;
            tri2vert2.Normal = tri2Normal;
            Vertex tri2Vert3 = {
                glm::vec3(x + STEP, trY, z + STEP),
                tri2Normal,
                glm::vec4(1.0f),
                glm::vec2(1.0f, 1.0f)};

            // first triangle
            terrainMesh->addVertex(tri1vert1);
            terrainMesh->addVertex(tri1vert2);
            terrainMesh->addVertex(tri1vert3);
            // second triangle
            terrainMesh->addVertex(tri2vert1);
            terrainMesh->addVertex(tri2vert2);
            terrainMesh->addVertex(tri2Vert3);
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