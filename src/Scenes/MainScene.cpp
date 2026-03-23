//
// Created by sierzegao on 3/16/2026.
//
#include "MainScene.hpp"
#include "Helpers/loadTextureHelpers.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"

glm::vec3 MainScene::BACKGROUND_COLOR = glm::vec3(35.0f / 255.0f, 183.0f / 255.0f, 255.0f / 255.0f);
glm::vec4 MainScene::LIGHT_DIRECTION = glm::vec4(0.5f, -0.0f, -0.312f, 0.0f);
glm::vec3 MainScene::LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 MainScene::AMBIENT = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 MainScene::DIFFUSE = glm::vec3(0.5f, 0.5f, 0.3f);
glm::vec3 MainScene::SPECULAR = glm::vec3(0.01f, 0.01f, 0.01f);
float MainScene::SHININESS = 16.0f;
float MainScene::ABSORPTION_COEF = 0.03f;

MainScene::MainScene()
    : player(glm::vec3(0.0f, 1.0f, 3.0f), 2.0f),
      noise(),
      ourShader(nullptr),
      cubes{nullptr},
      chunks{nullptr}
{
}

MainScene::~MainScene()
{
    for (const auto cube : cubes)
    {
        delete cube;
    }
    for (const auto chunk : chunks)
    {
        delete chunk;
    }
    delete ourShader;
}

void MainScene::init()
{
    // Player and camera setup
    player = Player(glm::vec3(0.0f, 1.0f, 3.0f), 2.0f);

    // Shader
    ourShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");

    // set texture wrapping and filtering.
    // The WRAP parameters are for wrapping along the S and T axes. recall that for textures, STPQ = XYZW
    // the MIN and MAG filters are for when OpenGL wants to increase/decrease the size of the
    // texture to fit the actual thingy. for min we wanna use mipmaps!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // this process of binding and manipulating is not something I'm used to...
    // Very different from OO.
    textures[0] = loadTextureImage("../textures/sand.png", true);

    ourShader->use();
    ourShader->setInt("sandTexture", 0);

    // enable depth testing so that triangles don't draw over each other in 3D.
    glEnable(GL_DEPTH_TEST);

    // array to store all our cubes
    cubes[0] = new Cube(
        glm::vec3(-50.0f, -20.0f, -50.0f),
        glm::vec3(100.0f, 1.0f, 100.0f),
        std::vector<Texture>{Texture{textures[0]}});

    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.01f);
    noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
    noise.SetFractalOctaves(4);

    chunks[0] = new Chunk(
        glm::vec3(0.0f, 0.0f, 0.0f),
        std::vector<Texture>{Texture{textures[0]}},
        &noise);
    chunks[1] = new Chunk(
        glm::vec3(1.0f, 0.0f, 0.0f),
        std::vector<Texture>{Texture{textures[0]}},
        &noise);
    chunks[2] = new Chunk(
        glm::vec3(0.0f, 0.0f, 1.0f),
        std::vector<Texture>{Texture{textures[0]}},
        &noise);
    chunks[3] = new Chunk(
        glm::vec3(1.0f, 0.0f, 1.0f),
        std::vector<Texture>{Texture{textures[0]}},
        &noise);

    for (auto chunk : chunks)
    {
        chunk->generate();
    }
}

void MainScene::update(float deltaTime)
{
    std::vector<glm::vec3> collisions;
    for (const auto cube : cubes)
    {
        // check collisions
        glm::vec3 collision = player.AABB.checkCollision(cube->AABB);
        if (collision.x != 0 || collision.y != 0 || collision.z != 0)
        {
            collisions.push_back(collision);
        }
    }

    player.UpdatePlayer(deltaTime, collisions);
}

void MainScene::render()
{
    // Clear background
    glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view matrix - transform the world so that it appears in front of the camera.
    // basically instead of moving the camera we move the world
    const glm::mat4 view = player.camera.GetViewMatrix();

    // projection matrix with perspective
    // FOV, aspect ratio, near plane, far plane.
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &windowWidth, &windowHeight);
    const glm::mat4 projection = glm::perspective(glm::radians(player.camera.Zoom), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.01f, 100.0f);

    // send transformation matrices to uniforms.
    // done each frame since they change a lot.
    ourShader->use();
    ourShader->setMat4("view", view);
    ourShader->setMat4("projection", projection);
    ourShader->setVec3("viewPos", player.camera.Position);

    ourShader->setVec4("light.direction", LIGHT_DIRECTION);
    ourShader->setVec3("light.color", LIGHT_COLOR);
    ourShader->setVec3("material.ambient", AMBIENT);
    ourShader->setVec3("material.diffuse", DIFFUSE);
    ourShader->setVec3("material.specular", SPECULAR);
    ourShader->setFloat("material.shininess", SHININESS);
    ourShader->setFloat("absorptionCoef", ABSORPTION_COEF);
    ourShader->setVec3("fogColor", BACKGROUND_COLOR);

    // draw each cube
    // actually i don't wanna... just want an invisible floor for now.

    // draw chunks
    for (const auto chunk : chunks)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chunk->getPosition() * Chunk::CHUNK_SIZE);
        ourShader->setMat4("model", model);
        chunk->draw();
    }
}

void MainScene::renderDebug()
{
    ImGui::Begin("MainScene");
    ImGui::Text("Player Position: (%.2f, %.2f, %.2f)", player.Position.x, player.Position.y, player.Position.z);
    ImGui::Text("Player Velocity: (%.2f, %.2f, %.2f)", player.CurrentVelocity.x, player.CurrentVelocity.y, player.CurrentVelocity.z);

    ImGui::SliderFloat3("Light Direction", &LIGHT_DIRECTION.x, -1.0f, 1.0f);
    ImGui::ColorEdit3("Light Color", &LIGHT_COLOR.x);
    ImGui::ColorEdit3("Ambient", &AMBIENT.x);
    ImGui::ColorEdit3("Diffuse", &DIFFUSE.x);
    ImGui::ColorEdit3("Specular", &SPECULAR.x);
    ImGui::SliderFloat("Shininess", &SHININESS, 1.0f, 128.0f);
    ImGui::SliderFloat("Absorption Coef", &ABSORPTION_COEF, 0.0f, 0.05f);

    ImGui::End();
}

void MainScene::cleanup()
{
    for (const auto cube : cubes)
    {
        delete cube;
    }
    for (const auto chunk : chunks)
    {
        delete chunk;
    }
    delete ourShader;
    // TODO: loading and deleting textures on every scene load is really wasteful.
    // We should probably have a texture manager and maybe shader manager too?
    // For now since we only have one tiny texture it's not a big deal but this is not scalable at all.
    // teehee :P
    glDeleteTextures(1, textures);
}

void MainScene::processInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player.ProcessKeyboard(DOWN, deltaTime);
}

void MainScene::onMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    player.ProcessMouseMovement(xpos, ypos);
}

void MainScene::onMouseScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    player.ProcessMouseScroll(yoffset);
}
