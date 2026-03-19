/**
 * A first project: Shader.cpp
 * With: OpenGL, GLFW, GLAD, CMake
 * By Oskar Sierzega
 * Created 3/2/2024
 * Edited  3/2/2024
 */
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * The constructor for our shaders.
 * @param vertexPath Path of the vertex shader source file
 * @param fragmentPath Path of the fragment shader source file
 */
Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // Declare variables.
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    // Convert to C string so that OpenGL can use to compile
    const char *vCode = vertexCode.c_str();
    const char *fCode = fragmentCode.c_str();

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);

    // attach the source code to the shader and compile; must be compiled at runtime
    // compiling at runtime seems inefficient...?
    glShaderSource(vertex, 1, &vCode, nullptr);
    glCompileShader(vertex);

    // check if the shader compiled correctly. probably smart to do this
    // glGetShader is what checks if the compilation succeeded or not.
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Create fragment shader in similar way.
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, nullptr);
    glCompileShader(fragment);

    // Check for success again just to be safe.
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Shader Program
    ID = glCreateProgram();
    // Attach the shaders we made and link them
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print any linking errors we get.
    // We get linking errors if the inputs and outputs of our shaders do not match.
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // Delete the shaders to free up memory; once they are linked we don't need anymore
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/**
 * This is a simple method that lets us use the shader program.
 */
void Shader::use() const
{
    glUseProgram(ID);
}

/**
 * The following methods are used to alter uniforms in our shaders.
 */
void Shader::setBool(const std::string &name, const bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, const int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set4f(const std::string &name, const float v0, const float v1, const float v2, const float v3) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void Shader::setMat4(const std::string &name, glm::mat4 val) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setVec2(const std::string &name, glm::vec2 val) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), val.x, val.y);
}

void Shader::setVec3(const std::string &name, glm::vec3 val) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z);
}

void Shader::setVec4(const std::string &name, glm::vec4 val) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), val.x, val.y, val.z, val.w);
}
