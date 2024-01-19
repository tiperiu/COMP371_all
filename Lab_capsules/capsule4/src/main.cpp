#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"

#include "Shape.h"
#include "Square.h"
#include "Cube.h"

const int width = 1024;
const int height = 768;
const float pi = std::acos(-1.0f);

const std::string phongVertex = "phong.vert.glsl";
const std::string phongFragment = "phong.frag.glsl";

const std::string blinnPhongVertex = "blinnPhong.vert.glsl";
const std::string blinnPhongFragment = "blinnPhong.frag.glsl";

const std::string toonVertex = "toon.vert.glsl";
const std::string toonFragment = "toon.frag.glsl";

Shader phong;
Shader blinnPhong;
Shader toon;

Shader* currentShader;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action != GLFW_PRESS)
    {
        return;
    }

    switch(key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_P:
            currentShader = &phong;
            currentShader->use();
            break;
        case GLFW_KEY_B:
            currentShader = &blinnPhong;
            currentShader->use();
            break;
        case GLFW_KEY_T:
            currentShader = &toon;
            currentShader->use();
            break;
    }
}

int main(int argc, char** argv)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Capsule 04 - Toon Shader", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);

    try
    {
        phong.build(phongVertex, phongFragment);
        blinnPhong.build(blinnPhongVertex, blinnPhongFragment);
        toon.build(toonVertex, toonFragment);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    currentShader = &phong;

    std::vector<Shape*> shapes;

    float lastTime = glfwGetTime();
    float dt; // Delta Time

    float rot = 0.0f;

    glm::mat4 viewProjection; 

    Square floor;
    floor.translate(glm::vec3(0.0f, -0.5f, 0.0f));
    floor.rotate(-pi/2, glm::vec3(1.0f, 0.0f, 0.0f));
    floor.scale(glm::vec3(10.0f));

    shapes.push_back(&floor);

    Cube red;
    red.setColor(1.0f, 0.0f, 0.0f);

    Cube green;
    green.setColor(0.0f, 1.0f, 0.0f);
    green.translate(glm::vec3(2.0f, 1.0f, 0.5f));
    green.rotate(pi/2, glm::vec3(std::sqrt(2.0f)/2, std::sqrt(2.0f)/2, 0.0f));
    green.scale(glm::vec3(1.5f));

    Cube blue;
    blue.setColor(0.0f, 0.0f, 1.0f);
    blue.translate(glm::vec3(-1.5f, 1.0f, -1.5f));
    blue.rotate(-pi/4, glm::vec3(1.0f, 0.0f, 0.0f));

    Cube cyan;
    cyan.setColor(0.0f, 1.0f, 1.0f);
    cyan.translate(glm::vec3(-0.5f, 2.0f, 0.0f));
    cyan.rotate(pi/4, glm::vec3(std::sqrt(2.0f)/2, std::sqrt(2.0f)/2, 0.0f));

    shapes.push_back(&red);
    shapes.push_back(&green);
    shapes.push_back(&blue);
    shapes.push_back(&cyan);

    Camera camera;
    camera.setAspectRatio((float) width/height);
    camera.setPosition(glm::vec3(0.0f, 1.0f, -5.0f));

    glm::vec3 lightPos{5.0f, 1.0f, 0.0f};

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

    currentShader->use();

    while (!glfwWindowShouldClose(window))
    {
        dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        rot += dt * pi/2; // pi/2 per second
        rot = std::fmod(rot, 2*pi); // overflow prevention

        lightPos.x = 5 * std::cos(rot);

        viewProjection = camera.getProjMatrix() * camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentShader->setVar(lightPos, "lightPos");
        currentShader->setVar(camera.getPosition(), "viewPos");

        for (auto shape : shapes)
        {
            currentShader->setVar(viewProjection * shape->getTransform(), "mvp");
            currentShader->setVar(shape->getTransform(), "m");
            currentShader->setVar(shape->getColor(), "objectColor");
            shape->draw();
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    currentShader = nullptr;

    glfwTerminate();

    return 0;
}
