//
// Created by sierzegao on 2026-03-17.
//
#include "WaterScene.hpp"
#include "Helpers/loadTextureImage.hpp"
#include "Player.hpp"

glm::vec3 WaterScene::BACKGROUND_COLOR = glm::vec3(35.0f / 255.0f, 183.0f / 255.0f, 255.0f / 255.0f);
float WaterScene::WATER_HEIGHT = 0.0f;
int WaterScene::OCTAVES = 8;
float WaterScene::AMPLITUDE[8] = {0.16f, 0.16f, 0.07f, 0.05f, 0.03f, 0.022f, 0.010f, 0.009f};
float WaterScene::WAVELENGTH[8] = {19 / 3.0f, 17 / 3.0f, 13 / 3.0f, 11 / 3.0f, 7 / 3.0f, 11 / 3.0f, 13 / 3.0f, 17 / 3.0f};
float WaterScene::SPEED[8] = {0.25f * 3.0f, 0.198f * 3.0f, 0.368f * 3.0f, 0.225f * 3.0f, 0.097f * 3.0f, 0.074f * 3.0f, 0.056f * 3.0f, 0.040f * 3.0f};
glm::vec2 WaterScene::DIRECTION[8] = {
    glm::vec2(1.0f, 0.0f),    // dominant swell
    glm::vec2(0.25f, 0.50f),  // slight spread
    glm::vec2(0.76f, -0.38f), // opposite spread
    glm::vec2(0.95f, 0.60f),  // medium spread
    glm::vec2(-0.31f, 0.95f), // cross-wave
    glm::vec2(0.60f, -0.80f), // cross-wave
    glm::vec2(-0.47f, 0.74f), // scattered ripple
    glm::vec2(0.82f, 0.42f)}; // scattered ripple

glm::vec4 WaterScene::LIGHT_DIRECTION = glm::vec4(-0.5f, -0.0f, -0.312f, 0.0f);
glm::vec3 WaterScene::LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 WaterScene::AMBIENT = glm::vec3(0.04f, 0.10f, 0.18f);
glm::vec3 WaterScene::DIFFUSE = glm::vec3(0.08f, 0.28f, 0.38f);
glm::vec3 WaterScene::SPECULAR = glm::vec3(0.85f, 0.88f, 0.92f);
float WaterScene::SHININESS = 16.0f;

WaterScene::WaterScene()
    : freeCam(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
      waterMesh(nullptr)
{
}

WaterScene::~WaterScene()
{
    delete waterShader;
    delete waterMesh;
    glDeleteTextures(1, textures);
}

void WaterScene::init()
{
    freeCam = FreeCam(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    waterShader = new Shader("../shaders/water.vert", "../shaders/water.frag");

    glGenTextures(1, textures);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadTextureImage("../textures/water.png", textures[0], true);

    waterShader->use();
    waterShader->setInt("waterTexture", 0);

    // enable depth testing so that triangles don't draw over each other in 3D.
    glEnable(GL_DEPTH_TEST);

    // enable blending for transparency in water
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    waterMesh = new Mesh();

    const float CHUNK_SIZE = 16.0f;
    const float STEP = 0.1f;
    const glm::vec4 color = glm::vec4(0.0f, 0.5f, 1.0f, 0.5f);

    // generate water mesh... just a blanket
    for (float x = 0; x < CHUNK_SIZE; x += STEP)
    {
        for (float z = 0; z < CHUNK_SIZE; z += STEP)
        {
            Vertex blVert = {
                glm::vec3(x, 0.0f, z),       // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                color,                       // Color
                glm::vec2(0.0f, 0.0f)};      // TexCoords
            Vertex brVert = {
                glm::vec3(x + STEP, 0.0f, z),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(1.0f, 0.0f)};
            Vertex tlVert = {
                glm::vec3(x, 0.0f, z + STEP),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(0.0f, 1.0f)};
            Vertex trVert = {
                glm::vec3(x + STEP, 0.0f, z + STEP),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(1.0f, 1.0f)};

            // first triangle
            waterMesh->addVertex(blVert);
            waterMesh->addVertex(brVert);
            waterMesh->addVertex(tlVert);
            // second triangle
            waterMesh->addVertex(tlVert);
            waterMesh->addVertex(brVert);
            waterMesh->addVertex(trVert);
        }
    }

    waterMesh->recreateVBO();
}

void WaterScene::update(float deltaTime)
{
    // no physics updates needed for now
}

void WaterScene::render()
{
    // bg
    glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // transforms
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
    const glm::mat4 view = freeCam.camera.GetViewMatrix();
    const glm::mat4 projection = glm::perspective(glm::radians(freeCam.camera.Zoom), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.01f, 100.0f);

    waterShader->use();
    waterShader->setMat4("view", view);
    waterShader->setMat4("projection", projection);
    waterShader->setVec3("viewPos", freeCam.camera.Position);
    waterShader->setVec3("fogColor", BACKGROUND_COLOR);

    waterShader->setFloat("waterHeight", WATER_HEIGHT);
    waterShader->setFloat("time", static_cast<float>(glfwGetTime()));
    waterShader->setInt("octaves", OCTAVES);
    for (int i = 0; i < OCTAVES; ++i)
    {
        waterShader->setFloat("amplitude[" + std::to_string(i) + "]", AMPLITUDE[i]);
        waterShader->setFloat("wavelength[" + std::to_string(i) + "]", WAVELENGTH[i]);
        waterShader->setFloat("speed[" + std::to_string(i) + "]", SPEED[i]);
        waterShader->setVec2("direction[" + std::to_string(i) + "]", DIRECTION[i]);
    }

    waterShader->setVec4("light.direction", LIGHT_DIRECTION);
    waterShader->setVec3("light.color", LIGHT_COLOR);
    waterShader->setVec3("material.ambient", AMBIENT);
    waterShader->setVec3("material.diffuse", DIFFUSE);
    waterShader->setVec3("material.specular", SPECULAR);
    waterShader->setFloat("material.shininess", SHININESS);

    // water mesh
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    waterShader->setMat4("model", model);
    waterMesh->draw();
}

void WaterScene::renderDebug()
{
    // no debug rendering needed for now
}

void WaterScene::cleanup()
{
    delete waterShader;
    delete waterMesh;
    // TODO: loading and deleting textures on every scene load is really wasteful.
    // We should probably have a texture manager and maybe shader manager too?
    // For now since we only have one tiny texture it's not a big deal but this is not scalable at all.
    // teehee :P
    glDeleteTextures(1, textures);
}

void WaterScene::processInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        freeCam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        freeCam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        freeCam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        freeCam.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        freeCam.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        freeCam.ProcessKeyboard(DOWN, deltaTime);
}

void WaterScene::onMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    freeCam.ProcessMouseMovement(xpos, ypos);
}

void WaterScene::onMouseScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    freeCam.ProcessMouseScroll(yoffset);
}