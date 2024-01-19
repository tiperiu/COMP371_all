#ifndef COMP371_SHADER_H_
#define COMP371_SHADER_H_

#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>

class Shader
{
    public:
        Shader();
        ~Shader();

        void build(const std::string& vertexFilepath, const std::string& fragmentFilepath);
        void use();

        template<typename T>
        void setVar(T var, std::string varname);

    private:
        GLuint programId;

        std::string vertexSource;
        std::string fragmentSource;

        std::string readFileContents(const std::string& filepath);
        void checkShaderError(GLuint shaderId, std::string type);
};

#endif // COMP371_SHADER_H_
