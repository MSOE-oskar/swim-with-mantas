//
// Created by sierzegao on 3/16/2026.
//
#include "SceneManager.hpp"
#include "MainScene.hpp"
#include "Scene.hpp"

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
    // for now we just have one scene, but we can easily add more later
    currentScene = new MainScene();
    currentScene->init();
    scenes.push_back(currentScene);
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