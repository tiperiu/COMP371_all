cmake_minimum_required(VERSION 3.8)

project(lab02) #The name of your choice for the project comes here

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME}  lab02.cpp) #The name of the cpp file and its path can vary

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW glfw)
