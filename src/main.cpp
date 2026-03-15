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

// our custom classes
#include "Shader.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "AxisAlignedBoundingBox.hpp"
#include "Cube.hpp"

// stb_image for reading in images
#include "stb_image.hpp"

// GLM is a library for math in OpenGL
// Installed in msys2 MinGW with this command:
// pacman -S mingw-w64-x86_64-glm
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void loadTextureImage(const std::string &path, unsigned int textureID, bool alpha);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void setWindowFps(GLFWwindow *window, double currentTime);

bool moshing = false, firstMouse = true;

// delta time helps us keep things consistent across system
float deltaTime = 0.0f, lastFrame = 0.0f;
// for fps counter
int frameCount = 0;
double lastFpsUpdate = 0.0;

// Camera Vars
float lastX, lastY;

// Player
Player *player = new Player(glm::vec3(0.0f, 1.0f, 3.0f), 2.0f, 8.0f, 2.0f);
Camera *camera = player->camera;

// window constants
constexpr glm::vec3 BACKGROUND_COLOR = glm::vec3(0.0f, 0.0f, 0.50f);
constexpr int WINDOW_HEIGHT = 800, WINDOW_WIDTH = 1400;
bool isFullscreen = false;      // Track fullscreen state
GLFWmonitor *monitor = nullptr; // Default monitor
const GLFWvidmode *mode;        // Monitor video mode

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
    // callback for cursor moving
    glfwSetCursorPosCallback(window, mouse_callback);
    // callback for scroll
    glfwSetScrollCallback(window, scroll_callback);
    // enable vsync - set to 0 to disable, 1 to enable.
    glfwSwapInterval(1);

    // Shader Class. This handles all our shader stuff.
    const Shader ourShader("../shaders/shader.vert", "../shaders/shader.frag");

    /**
     * TEXTURES
     * Let's create a TEXTURE from an image.
     */
    unsigned int textures[1];
    glGenTextures(1, textures);

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
    loadTextureImage("../textures/sand.png", textures[0], true);

    ourShader.use();
    ourShader.setInt("sandTexture", 0);

    // enable depth testing so that triangles don't draw over each other in 3D.
    glEnable(GL_DEPTH_TEST);

    // array to store all our cubes
    Cube *cubes[] = {
        new Cube(
            glm::vec3(-50.0f, -1.0f, -50.0f),
            glm::vec3(100.0f, 1.0f, 100.0f),
            std::vector<Texture>{Texture{textures[0]}}),
    };

    /**
     * !! IMPORTANT !!
     * This is the render loop. This keeps running until we tell glfw to stop.
     * Without this the window would close instantly.
     * This allows us to take input and update the screen!!
     */
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // update deltaTime
        const float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // update fps counter
        setWindowFps(window, currentFrame);

        // Background
        if (!moshing)
        {
            glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        glClear(GL_DEPTH_BUFFER_BIT);

        /**
         * DRAW COOL SHIT!
         */

        // manipulate the transformation matrices

        // view matrix - transform the world so that it appears in front of the camera.
        // basically instead of moving the camera we move the world
        const glm::mat4 view = camera->GetViewMatrix();

        // projection matrix with perspective
        // FOV, aspect ratio, near plane, far plane.
        const glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT), 0.01f, 100.0f);

        // send transformation matrices to uniforms.
        // done each frame since they change a lot.
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("viewPos", camera->Position);
        ourShader.setVec3("fogColor", BACKGROUND_COLOR);
        ourShader.use();

        std::vector<glm::vec3> collisions;

        // draw each cube
        for (const auto cube : cubes)
        {
            // bind model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube->getPosition());
            model = glm::scale(model, cube->getScale());
            ourShader.setMat4("model", model);

            // draw the cube
            cube->draw();

            // check collisions
            glm::vec3 collision = player->AABB->checkCollision(cube->AABB);
            if (collision.x != 0 || collision.y != 0 || collision.z != 0)
            {
                collisions.push_back(collision);
            }
        }

        // update player's position
        player->UpdatePlayer(deltaTime, collisions);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        // Check if any events are triggered, like keyboard or mouse
        glfwPollEvents();
    }

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

    // moshing easter egg :)
    if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
        moshing = true;
    else
        moshing = false;

    // movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        player->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        player->ProcessKeyboard(DOWN, deltaTime);

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
}

/**
 * Load a texture from the thing
 * @param path path of the texture
 * @param textureID id to store texture in
 * @param alpha true for pngs, false for jpgs.
 */
void loadTextureImage(const std::string &path, const unsigned int textureID, const bool alpha)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Load in image with STB_IMAGE what a goat
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels; // nrChannels = number of color channels.
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // Set the image to the currently bound texture! yay!
        // most of these parameters will stay the same for other textures i got a feeling.
        // esp if we use STB_IMAGE to load images.
        if (alpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // creates a mipmap.
        // Mipmap makes it easier for openGL to sample color from a texture
        // when it is far away.
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    // free the memory from reading the image. we made the texture already.
    stbi_image_free(data);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y goes from bottom to top
    lastX = xpos;
    lastY = ypos;

    constexpr float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(yoffset);
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