#ifndef COMP371_SHAPE_H_
#define COMP371_SHAPE_H_

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Shader.h"

class Shape
{
    public:
        Shape();
        virtual ~Shape();

        void draw();

        void translate(glm::vec3 t);
        void rotate(float rotation, glm::vec3 axis);
        void scale(glm::vec3 s);

        glm::mat4 getTransform() const;

        void setColor(float r, float g, float b);
        glm::vec3 getColor() const;

    protected:
        GLuint VAO;
        GLuint EBO;
        std::size_t indexSize;

        static std::size_t generateGeometry(GLuint& VAO, GLuint& EBO,
                const std::vector<glm::vec3>& vertices,
                const std::vector<glm::vec3>& normals,
                const std::vector<GLuint>& indices);

    private:
        glm::mat4 transform;

        glm::vec3 color;
};

#endif // COMP371_SHAPE_H_
