//
// Created by sierzegao on 3/16/2026.
//
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "WaterScene.hpp"
#include "Scene.hpp"

#include <imgui/imgui.h>

SceneManager::SceneManager()
{
    currentScene = nullptr;
}

SceneManager::~SceneManager()
{
    cleanup();
}

void SceneManager::init()
{
    // create all scenes and add them to the list
    scenes.push_back(new MainScene());
    scenes.push_back(new WaterScene());
    currentScene = scenes[0];
    currentScene->init();
}

void SceneManager::update(float deltaTime)
{
    if (currentScene)
    {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render()
{
    if (currentScene)
    {
        currentScene->render();
    }
}

void SceneManager::renderDebug()
{
    ImGui::Begin("Scene Switcher");
    // obviously this is terrible but i just wanna see it work lol
    if (ImGui::Button("Main Scene"))
    {
        if (currentScene)
        {
            currentScene->cleanup();
        }
        currentScene = scenes[0];
        currentScene->init();
    }
    if (ImGui::Button("Water Scene"))
    {
        if (currentScene)
        {
            currentScene->cleanup();
        }
        currentScene = scenes[1];
        currentScene->init();
    }
    ImGui::End();
    if (currentScene)
    {
        currentScene->renderDebug();
    }
}

void SceneManager::cleanup()
{
    if (currentScene)
    {
        currentScene->cleanup();
    }
    for (const auto scene : scenes)
    {
        delete scene;
    }
    scenes.clear();
}

void SceneManager::processInput(GLFWwindow *window, float deltaTime)
{
    if (currentScene)
    {
        currentScene->processInput(window, deltaTime);
    }
}

void SceneManager::onMouseMove(GLFWwindow *window, double xpos, double ypos)
{
    if (currentScene)
    {
        currentScene->onMouseMove(window, xpos, ypos);
    }
}

void SceneManager::onMouseScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    if (currentScene)
    {
        currentScene->onMouseScroll(window, xoffset, yoffset);
    }
}