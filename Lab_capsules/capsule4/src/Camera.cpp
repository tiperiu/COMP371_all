#include "Camera.h"

#include <glm/ext.hpp>

Camera::Camera()
{
    fov = 90.0f;
    aspectRatio = 1.0f;

    pos = glm::vec3(0.0f, 0.0f, -1.0f);
    lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera(){}

void Camera::setPosition(glm::vec3 p)
{
    pos = p;
}

void Camera::setLookAt(glm::vec3 l)
{
    lookAt = l;
}

void Camera::setUpVector(glm::vec3 u)
{
    up = u;
}

glm::vec3 Camera::getPosition() const
{
    return pos;
}

void Camera::setFov(float f)
{
    fov = f;
}

void Camera::setAspectRatio(float ar)
{
    aspectRatio = ar;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(pos, lookAt, up);
}

glm::mat4 Camera::getProjMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

