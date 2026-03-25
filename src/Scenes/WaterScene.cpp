//
// Created by sierzegao on 2026-03-17.
//
#include "WaterScene.hpp"
#include "Helpers/loadTextureHelpers.hpp"
#include "Player.hpp"
#include "imgui/imgui.h"

glm::vec3 WaterScene::BACKGROUND_COLOR = glm::vec3(35.0f / 255.0f, 183.0f / 255.0f, 255.0f / 255.0f);
float WaterScene::WATER_HEIGHT = 0.0f;
int WaterScene::OCTAVES = 4;
float WaterScene::STEEPNESS[4] = {0.970f, 0.941f, 0.274f, 0.405f};
float WaterScene::AMPLITUDE[4] = {0.675f, 0.350f, 0.418f, 0.329f};
float WaterScene::WAVELENGTH[4] = {8.561f, 11.393f, 8.975f, 7.321f};
float WaterScene::SPEED[4] = {0.506f, 1.055f, 1.857f, 2.215f};
glm::vec2 WaterScene::DIRECTION[4] = {
    glm::vec2(0.094f, 0.046f),
    glm::vec2(-0.074f, 0.106f),
    glm::vec2(-0.185f, -0.380f),
    glm::vec2(0.444f, -0.266f)};

glm::vec4 WaterScene::LIGHT_DIRECTION = glm::vec4(-0.5f, -0.0f, -0.312f, 0.0f);
glm::vec3 WaterScene::LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 WaterScene::AMBIENT = glm::vec3(0.04f, 0.10f, 0.18f);
glm::vec3 WaterScene::DIFFUSE = glm::vec3(0.08f, 0.28f, 0.38f);
glm::vec3 WaterScene::SPECULAR = glm::vec3(0.85f, 0.88f, 0.92f);
float WaterScene::SHININESS = 16.0f;

float WaterScene::FRESNEL_BIAS = 0.3f;
float WaterScene::FRESNEL_SCALE = 0.8f;
float WaterScene::FRESNEL_POWER = 1.0f;

WaterScene::WaterScene()
    : freeCam(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
      waterMesh(nullptr)
{
}

WaterScene::~WaterScene()
{
    delete waterShader;
    delete waterMesh;
    delete cube;
    glDeleteTextures(3, textures);
}

void WaterScene::init()
{
    freeCam = FreeCam(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

    textures[0] = loadTextureImage("../textures/sand.png", true);
    // Water normal from: https://www.filterforge.com/filters/12066-normal.html
    textures[1] = loadTextureImage("../textures/waternormal1.jpg", false);
    // https://www.filterforge.com/filters/9110-normal.html
    textures[2] = loadTextureImage("../textures/waternormal2.png", true);

    waterShader = new Shader("../shaders/water.vert", "../shaders/water.frag");
    waterShader->use();
    waterShader->setInt("waterNormalMap1", 0);
    waterShader->setInt("waterNormalMap2", 1);

    cubeShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");
    cubeShader->use();
    cubeShader->setInt("sandTexture", 0);

    // enable depth testing so that triangles don't draw over each other in 3D.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick
    // enable blending for transparency in water
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    waterMesh = new Mesh(std::vector<Texture>{Texture{textures[1]}, Texture{textures[2]}});

    const float CHUNK_SIZE = 50.0f;
    const float STEP = 0.5f;
    const float TEX_COORD_STEP = STEP / 10.0f;
    const glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    float u = 0.0f, v = 0.0f;

    // generate water mesh... just a blanket
    for (float x = 0; x < CHUNK_SIZE; x += STEP)
    {
        for (float z = 0; z < CHUNK_SIZE; z += STEP)
        {
            Vertex blVert = {
                glm::vec3(x, 0.0f, z),       // Position
                glm::vec3(0.0f, 1.0f, 0.0f), // Normal
                color,                       // Color
                glm::vec2(u, v)};            // TexCoords
            Vertex brVert = {
                glm::vec3(x + STEP, 0.0f, z),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(u + TEX_COORD_STEP, v)};
            Vertex tlVert = {
                glm::vec3(x, 0.0f, z + STEP),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(u, v + TEX_COORD_STEP)};
            Vertex trVert = {
                glm::vec3(x + STEP, 0.0f, z + STEP),
                glm::vec3(0.0f, 1.0f, 0.0f),
                color,
                glm::vec2(u + TEX_COORD_STEP, v + TEX_COORD_STEP)};

            // first triangle
            waterMesh->addVertex(blVert);
            waterMesh->addVertex(brVert);
            waterMesh->addVertex(tlVert);
            // second triangle
            waterMesh->addVertex(tlVert);
            waterMesh->addVertex(brVert);
            waterMesh->addVertex(trVert);

            v += TEX_COORD_STEP;
        }
        u += TEX_COORD_STEP;
    }

    waterMesh->recreateVBO();

    cube = new Cube(
        glm::vec3(0.0f, -10.5f, 0.0f),
        glm::vec3(50.0f, 1.0f, 50.0f),
        std::vector<Texture>{Texture{textures[0]}});

    std::vector<std::string> skyboxFaces = {
        "../textures/skybox/jettelly_sunshine_RIGHT.png",
        "../textures/skybox/jettelly_sunshine_LEFT.png",
        "../textures/skybox/jettelly_sunshine_UP.png",
        "../textures/skybox/jettelly_sunshine_DOWN.png",
        "../textures/skybox/jettelly_sunshine_FRONT.png",
        "../textures/skybox/jettelly_sunshine_BACK.png"};
    skybox = new Skybox(skyboxFaces);

    // this is terrible
    textures[3] = loadCubemap(skyboxFaces);
    waterShader->setInt("skyboxTexture", 2);
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

    // render skybox
    skybox->render(glm::mat4(glm::mat3(view)), projection);

    cubeShader->use();
    cubeShader->setMat4("view", view);
    cubeShader->setMat4("projection", projection);
    cubeShader->setVec3("viewPos", freeCam.camera.Position);
    cubeShader->setVec4("light.direction", LIGHT_DIRECTION);
    cubeShader->setVec3("fogColor", BACKGROUND_COLOR);
    cubeShader->setVec3("light.color", LIGHT_COLOR);
    cubeShader->setVec3("material.ambient", AMBIENT);
    cubeShader->setVec3("material.diffuse", DIFFUSE);
    cubeShader->setVec3("material.specular", SPECULAR);
    cubeShader->setFloat("material.shininess", SHININESS);

    // cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cube->getPosition());
    model = glm::scale(model, cube->getScale());
    cubeShader->setMat4("model", model);
    cube->draw();

    waterShader->use();
    waterShader->setMat4("view", view);
    waterShader->setMat4("projection", projection);
    waterShader->setVec3("viewPos", freeCam.camera.Position);
    waterShader->setVec3("fogColor", BACKGROUND_COLOR);

    waterShader->setFloat("waterHeight", WATER_HEIGHT);
    waterShader->setFloat("time", static_cast<float>(glfwGetTime()));
    waterShader->setInt("octaves", OCTAVES);
    waterShader->setFloatArray("steepness", OCTAVES, STEEPNESS);
    waterShader->setFloatArray("amplitude", OCTAVES, AMPLITUDE);
    waterShader->setFloatArray("wavelength", OCTAVES, WAVELENGTH);
    waterShader->setFloatArray("speed", OCTAVES, SPEED);
    waterShader->setVec2Array("direction", OCTAVES, DIRECTION);

    waterShader->setVec4("light.direction", LIGHT_DIRECTION);
    waterShader->setVec3("light.color", LIGHT_COLOR);
    waterShader->setVec3("material.ambient", AMBIENT);
    waterShader->setVec3("material.diffuse", DIFFUSE);
    waterShader->setVec3("material.specular", SPECULAR);
    waterShader->setFloat("material.shininess", SHININESS);

    waterShader->setFloat("fresnel.bias", FRESNEL_BIAS);
    waterShader->setFloat("fresnel.scale", FRESNEL_SCALE);
    waterShader->setFloat("fresnel.power", FRESNEL_POWER);

    // this is terrible and hacky
    // this is why we need a global texture manager
    // bruh
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[3]);

    // water mesh
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    waterShader->setMat4("model", model);
    waterMesh->draw();
}

void WaterScene::renderDebug()
{
    ImGui::Begin("WaterScene");

    ImGui::Text("Lighting");
    ImGui::ColorEdit3("Light Color", &LIGHT_COLOR[0]);
    ImGui::ColorEdit3("Ambient", &AMBIENT[0]);
    ImGui::ColorEdit3("Diffuse", &DIFFUSE[0]);
    ImGui::ColorEdit3("Specular", &SPECULAR[0]);
    ImGui::SliderFloat("Shininess", &SHININESS, 1.0f, 128.0f);

    ImGui::Separator();

    ImGui::Text("Fresnel");
    ImGui::SliderFloat("Fresnel Bias", &FRESNEL_BIAS, 0.0f, 1.0f);
    ImGui::SliderFloat("Fresnel Scale", &FRESNEL_SCALE, 0.0f, 1.0f);
    ImGui::SliderFloat("Fresnel Power", &FRESNEL_POWER, 1.0f, 10.0f);

    ImGui::Separator();

    ImGui::Text("Water");
    ImGui::SliderFloat("Water Height", &WATER_HEIGHT, -1.0f, 1.0f);
    ImGui::SliderInt("Octaves", &OCTAVES, 1, 8);
    for (int i = 0; i < OCTAVES; ++i)
    {
        ImGui::PushID(i);
        ImGui::Text("Octave %d", i + 1);
        ImGui::SliderFloat("Amplitude", &AMPLITUDE[i], 0.0f, 1.0f);
        ImGui::SliderFloat("Wavelength", &WAVELENGTH[i], 1.0f, 15.0f);
        ImGui::SliderFloat("Speed", &SPEED[i], 0.0f, 5.0f);
        ImGui::SliderFloat("Steepness", &STEEPNESS[i], 0.0f, 1.0f);
        ImGui::SliderFloat2("Direction", &DIRECTION[i].x, -1.0f, 1.0f);
        ImGui::PopID();
    }
    ImGui::End();
}

void WaterScene::cleanup()
{
    delete waterShader;
    delete waterMesh;
    delete cube;

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