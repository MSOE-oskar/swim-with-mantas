//
// Created by sierzegao on 2026-03-17.
//
#include "WaterScene.hpp"
#include "Helpers/loadTextureImage.hpp"
#include "Player.hpp"

glm::vec3 WaterScene::BACKGROUND_COLOR = glm::vec3(0.0f, 0.0f, 0.50f);

WaterScene::WaterScene()
    : freeCam(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
      cube(nullptr)
{
}

WaterScene::~WaterScene()
{
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

    cube = new Cube(
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        std::vector<Texture>{Texture{textures[0]}});
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

    // cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cube->getPosition());
    model = glm::scale(model, cube->getScale());
    waterShader->setMat4("model", model);
    cube->draw();
}

void WaterScene::renderDebug()
{
    // no debug rendering needed for now
}

void WaterScene::cleanup()
{
    delete waterShader;
    delete cube;
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