#include "Shape.h"

#include <glm/ext/matrix_transform.hpp>

Shape::Shape(): transform(glm::mat4(1.0f)), color(glm::vec3(1.0f))
{
}

Shape::~Shape()
{
}

void Shape::draw()
{
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::size_t Shape::generateGeometry(GLuint& VAO, GLuint& EBO,
                const std::vector<glm::vec3>& vertices,
                const std::vector<glm::vec3>& normals,
                const std::vector<GLuint>& indices)
{
        GLuint VBO;
        GLuint NBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &NBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, NBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return indices.size();
}

void Shape::translate(glm::vec3 t)
{
        transform = glm::translate(transform, t);
}

void Shape::rotate(float rotation, glm::vec3 axis)
{
        transform = glm::rotate(transform, rotation, axis);
}

void Shape::scale(glm::vec3 s)
{
        transform = glm::scale(transform, s);
}

glm::mat4 Shape::getTransform() const
{
        return transform;
}

void Shape::setColor(float r, float g, float b)
{
        color = glm::vec3(r, g, b);
}

glm::vec3 Shape::getColor() const
{
        return color;
}

