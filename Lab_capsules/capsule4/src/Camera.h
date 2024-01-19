#ifndef COMP371_CAMERA_H_
#define COMP371_CAMERA_H_

#include <glm/glm.hpp>

class Camera
{
    public:
        Camera();
        ~Camera();

        void setPosition(glm::vec3 p);
        void setLookAt(glm::vec3 l);
        void setUpVector(glm::vec3 u);

        glm::vec3 getPosition() const;

        void setFov(float f);
        void setAspectRatio(float ar);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjMatrix() const;

    private:
        glm::vec3 pos;
        glm::vec3 lookAt;
        glm::vec3 up;

        float fov;
        float aspectRatio;

        glm::mat4 projection;
        glm::mat4 view;
};

#endif // COMP371_CAMERA_H_
