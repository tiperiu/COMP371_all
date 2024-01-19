//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL

// IMPORTANT 1
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language

using namespace std;

// IMPORTANT 2
// Debug our opengl calls
// YOu can extend it to include a text message
void debug_gl(int place){
    GLenum e = glGetError();
    if(e!=GL_NO_ERROR){
      //  cout<<"We have an error! "<<place<<" "<<e<<" "<<getErrorString(e)<<endl;
        cout<<"We have an error! "<<place<<" "<<(void* )e<<endl;
    }
}

// HOW DO WE RENDER? shaders
const char* getVertexShaderSource()
{
    return
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;"
    "layout (location = 1) in vec3 col;"
    "out vec3 vertexColor;"
    ""
    "void main()"
    "{"
    "   gl_Position = vec4(aPos, 0, 1);"
    "   vertexColor = col;"
    "}";
}

const char* getFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "out vec3 FragColor;"
    ""
    "void main()"
    "{"
    "   FragColor = vertexColor;"
    "}";
}

const char* getVertexShaderSource2()
{
    return
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;"
    "layout (location = 1) in vec3 col;"
    "layout (location = 2) in vec2 p1;"
    "layout (location = 3) in vec2 p2;"
    "layout (location = 4) in vec2 p3;"
    "out vec3 vertexColor;"
    "out vec2 op1;"
    "out vec2 op2;"
    "out vec2 op3;"
    "out vec2 op;"
    ""
    "void main()"
    "{"
    "   gl_Position = vec4(aPos, 0, 1);"
    "   vertexColor = col;"
    "   op1 = p1;"
    "   op2 = p2;"
    "   op3 = p3;"
    "   op = aPos;"
    "}";
}

const char* getFragmentShaderSource2()
{
    return
    "#version 330 core\n"
    "in vec3 vertexColor;"
    "in vec2 op1;"
    "in vec2 op2;"
    "in vec2 op3;"
    "in vec2 op;"
    "out vec3 FragColor;"
    ""
    "void main()"
    "{"
    "    if (distance(op, op1) <= distance(op, op2)"
    "        && distance(op, op1) <= distance(op, op3)) {"
    ""
    "        FragColor = vec3(1, 0, 0);"
    ""
    "    } else if (distance(op, op2) <= distance(op, op3)) {"
    ""
    "        FragColor = vec3(0, 1, 0);"
    "    } else {"
    "        FragColor = vec3(0, 0, 1);"
    "    }"
    "}";
}

int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

void createRenderingData(unsigned int& VAO, unsigned int& VBO,unsigned int& CBO, unsigned int PBO[], unsigned int& EBO)
{

    // Define and upload geometry to the GPU here ...

    float vertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        0.0f,  0.75f
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2
    };

    float color[] = {
        1.0f,  0.0f, 0.0,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // 0 - create the vertex array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // create the vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // create the color
    glGenBuffers(1, &CBO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(float), color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(3, PBO);
    // create the triplet of inputs
    int offset = 2;

    for(int i=0;i<3;++i){

        glBindBuffer(GL_ARRAY_BUFFER, PBO[i]);

        float buffer[6];
        for(int j=0;j<3;++j){
            for(int k=0;k<2;++k){
                buffer[2*j+k] = vertices[2*i+k];
            }
        }

        glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), buffer, GL_STATIC_DRAW);

        glVertexAttribPointer(offset+i, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(offset+i);
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 01", NULL, NULL);
    if (window == NULL)
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

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    // Compile and link shaders here ...
    int vertexColorProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int voronoiProgram = compileAndLinkShaders(getVertexShaderSource2(), getFragmentShaderSource2());

    int shaderProgram = voronoiProgram;

    debug_gl(0);

    unsigned int VAO, VBO, CBO, EBO;
    unsigned int PBO[3];
    createRenderingData(VAO, VBO, CBO, PBO, EBO);

    glViewport(0,0,800,600);

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            shaderProgram = vertexColorProgram;
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            shaderProgram = voronoiProgram;
        }
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}





