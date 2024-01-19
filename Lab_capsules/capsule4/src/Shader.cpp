#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#if __cplusplus >= 201703L
#include <filesystem>
#endif

#include <glm/glm.hpp>

Shader::Shader()
{}

Shader::~Shader() {}

void Shader::build(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
    std::string vertexSource = readFileContents(vertexFilepath); 
    std::string fragmentSource = readFileContents(fragmentFilepath); 

    const GLchar* vsrc = vertexSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsrc, NULL);
    glCompileShader(vertexShader);

    checkShaderError(vertexShader, "Vertex Shader");

    const GLchar* fsrc = fragmentSource.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsrc, NULL);
    glCompileShader(fragmentShader);

    checkShaderError(fragmentShader, "Fragment Shader");

    // link shaders
    programId = glCreateProgram();
    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);
    
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed...\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(programId);
}

template<typename T>
void Shader::setVar(T var, std::string varname)
{
    GLint location = glGetUniformLocation(programId, varname.c_str());
    glUniform1f(location, var);
}

template <>
void Shader::setVar<glm::mat4>(glm::mat4 var, std::string varname)
{
    GLint location = glGetUniformLocation(programId, varname.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &var[0][0]);
}

template <>
void Shader::setVar<glm::vec3>(glm::vec3 var, std::string varname)
{
    GLint location = glGetUniformLocation(programId, varname.c_str());
    glUniform3fv(location, 1, &var[0]);
}

template <>
void Shader::setVar<glm::vec2>(glm::vec2 var, std::string varname)
{
    GLint location = glGetUniformLocation(programId, varname.c_str());
    glUniform2fv(location, 1, &var[0]);
}

std::string Shader::readFileContents(const std::string& filepath)
{
#if __cplusplus >= 201703L
    if(!std::filesystem::exists(filepath))
    {
        std::stringstream error;
        error << "File read error: " << std::filesystem::current_path() / filepath << " not found.";
        throw std::runtime_error(error.str());
    }
#endif

    std::string contents;
    std::ifstream infile(filepath, std::ios::ate);

#if __cplusplus < 201703L
    if(!infile.is_open())
    {
        std::stringstream error;
        error << "File read error: " << filepath << " not found.";
        throw std::runtime_error(error.str());
    }
#endif

    std::ios::pos_type end = infile.tellg();
    contents.resize(end);
    infile.seekg(0, std::ios::beg);
    infile.read(&contents[0], end);
    infile.close();

    return contents;
}

void Shader::checkShaderError(GLuint shaderId, std::string type)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cerr << type << " compilation error...\n" << infoLog << std::endl;
    }
}
