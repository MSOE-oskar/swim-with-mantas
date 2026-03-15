/**
* A first project: Shader.h
 * With: OpenGL, GLFW, GLAD, CMake
 * By Oskar Sierzega
 * Created 3/2/2024
 * Edited  3/2/2024
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

/**
 * Basically this class takes all the code that we had to create, compile,
 * attach shaders to linking program, and use linking program.
 * This is a good way that we can modularize our code so that we can reuse!
 */
class Shader
{
    public:
        // the program ID
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        // use the shader
        void use() const;

        // functions to set uniforms
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void set4f(const std::string &name, float v0, float v1, float v2, float v3) const;
        void setMat4(const std::string &name, glm::mat4 val) const;
};

#endif //SHADER_H
