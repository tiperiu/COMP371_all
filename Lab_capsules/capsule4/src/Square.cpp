#include "Square.h"

const std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f)
};

const std::vector<glm::vec3> normals = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
};

const std::vector<GLuint> indices = {0, 1, 2, 2, 1, 3};

Square::Square()
{
        indexSize = generateGeometry(VAO, EBO, vertices, normals, indices);
}

Square::~Square()
{
}
