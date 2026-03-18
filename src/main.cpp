/**
 * Swim with Mantas
 * With: OpenGL, GLFW, GLAD, CMake
 * By Oskar Sierzega
 * Created 2/28/2024
 * Edited  3/16/2024
 */
#include <cmath>
#include <iostream>
#include <ostream>
#include <cstdio>
#include <fstream>
#include <memory>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM is a library for math in OpenGL
// Installed in msys2 MinGW with this command:
// pacman -S mingw-w64-x86_64-glm
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stb_image for reading in images
#include "stb_image/stb_image.hpp"

// noise library
#include "FastNoiseLite/FastNoiseLite.h"

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// our custom classes
#include "Shader.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "AxisAlignedBoundingBox.hpp"
#include "Cube.hpp"
#include "Chunk.hpp"
#include "Scenes/SceneManager.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void setWindowFps(GLFWwindow *window, double currentTime);

bool showDebug = false, showCursor = false;
float lastToggleTime = 0.0f, debounceTime = 0.2f;

// delta time helps us keep things consistent across system
float deltaTime = 0.0f, lastFrame = 0.0f;
// for fps counter
int frameCount = 0;
double lastFpsUpdate = 0.0;

// window constants
constexpr int WINDOW_HEIGHT = 800, WINDOW_WIDTH = 1400;
bool isFullscreen = false;      // Track fullscreen state
GLFWmonitor *monitor = nullptr; // Default monitor
const GLFWvidmode *mode;        // Monitor video mode

// scene manager the goat
SceneManager sceneManager;

int main()
{
    // Initialize GLFW. we need to do this before any glfw functions.
    glfwInit();
    // Sets required versions of OpenGL. Major is 3 and minor is 3 so we need openGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // We're using OpenGL core cuz remember that's the better one
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // This creates the window object.
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Swim with Mantas!", nullptr, nullptr);
    // If we fail to create the window, which we shouldn't. Just terminate it safely.
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // The window has a context associated. Make that context the current context
    // so that any OpenGL commands we do after will be applied to that context.
    glfwMakeContextCurrent(window);

    // load up glad and make sure it don't break
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // This sets where we will render things.
    // This can be smaller than the window itself. They don't have to
    // be equal.
    // Notice how we don't have to specify the window, since
    // we set the context to be the window!
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // See below; this attaches the method to be called every time
    // the window is resized.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // setup things for mouse movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set scene manager to take the input
    glfwSetWindowUserPointer(window, &sceneManager);
    // callback for cursor moving
    glfwSetCursorPosCallback(window, mouse_callback);
    // callback for scroll
    glfwSetScrollCallback(window, scroll_callback);
    // enable vsync - set to 0 to disable, 1 to enable.
    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // initialize the scene manager and load the first scene
    sceneManager.init();

    /**
     * !! IMPORTANT !!
     * This is the render loop. This keeps running until we tell glfw to stop.
     * Without this the window would close instantly.
     * This allows us to take input and update the screen!!
     */
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events are triggered, like keyboard or mouse
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (showDebug)
        {
            sceneManager.renderDebug(); // Show debug menu for current scene
        }

        // input
        processInput(window);

        // update deltaTime
        const float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lastToggleTime += deltaTime;

        // update fps counter
        setWindowFps(window, currentFrame);

        // render
        sceneManager.render();

        // update
        sceneManager.update(deltaTime);

        // render ImGui on top of everything else
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and render additional platform windows (multi-viewport)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(window); // Restore our context
        }

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
    }

    // cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Just like glfwInit, we need to call this when we are done with GLFW.
    glfwTerminate();
    return 0;
}

/**
 * This built-in function is called whenever the passed window is resized.
 * The width and height are given to you by GLFW.
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

/**
 * This one is pretty self-explanatory. we're processing input from the user!
 * @param window
 */
void processInput(GLFWwindow *window)
{
    // Looks to me that glfwGetKey will be very useful...
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // debug screen
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
    {
        if (lastToggleTime >= debounceTime)
        {
            showDebug = !showDebug;
            lastToggleTime = 0.0f;
        }
    }

    // toggle cursor
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
    {
        if (lastToggleTime >= debounceTime)
        {
            showCursor = !showCursor;
            glfwSetInputMode(window, GLFW_CURSOR, showCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            lastToggleTime = 0.0f;
        }
    }

    // toggle fullscreen with F11
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        if (isFullscreen)
        {
            // Switch to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0); // Windowed mode with size (800x600)
        }
        else
        {
            // Switch to fullscreen mode
            monitor = glfwGetPrimaryMonitor();
            mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        isFullscreen = !isFullscreen; // Toggle the fullscreen state
    }

    // pass input to scene manager
    sceneManager.processInput(window, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    auto *manager = static_cast<SceneManager *>(glfwGetWindowUserPointer(window));
    manager->onMouseMove(window, xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto *manager = static_cast<SceneManager *>(glfwGetWindowUserPointer(window));
    manager->onMouseScroll(window, xoffset, yoffset);
}

void setWindowFps(GLFWwindow *win, double currentTime)
{
    frameCount++;

    if (currentTime - lastFpsUpdate >= 1.0)
    {
        char title[256];
        title[255] = '\0';

        snprintf(title, 255,
                 "Swim with Mantas! - [FPS: %3.2f]",
                 (float)frameCount);

        glfwSetWindowTitle(win, title);

        frameCount = 0;
        lastFpsUpdate += 1.0;
    }
}