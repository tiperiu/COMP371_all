cmake_minimum_required(VERSION 3.8)

project(capsule04)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH
    /encs # For ENCS lab computers
    /opt/local # Macports
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED
    HINTS /encs/pkg/glfw-3.3.4/root # ENCS installation of glfw
)

set(SRC
    src/main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Shape.cpp
    src/Square.cpp
    src/Cube.cpp
)

set(ALL_LIBS
    GLEW::glew
    OpenGL::GL
    glfw
)

# NOTE: ENCS glm installation is missing links to *.inl files so we need this line
include_directories(/encs/pkg/glm-0.9.9.8/root/include)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

file(COPY
    shaders/phong.frag.glsl
    shaders/phong.vert.glsl
    shaders/blinnPhong.frag.glsl
    shaders/blinnPhong.vert.glsl
    shaders/toon.frag.glsl
    shaders/toon.vert.glsl
    DESTINATION
    ${CMAKE_BINARY_DIR}
)
