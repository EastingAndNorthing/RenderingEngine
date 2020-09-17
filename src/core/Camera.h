#pragma once

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:

    enum ProjectionType {
        Perspective, Orthographic
    };

    float frameBufferWidth;
    float frameBufferHeight;
    
    float fov = g_settings.fov;
    float speed = 1.0f;
    float zoomspeed = 0.5f;
    bool autoRotation = false;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, -2.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 lookAtPos = glm::vec3(0.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    Camera();
    ~Camera();

    void setEulerAngles(float x, float y, float z);
    void autoRotate();
    void update();
    void setProjection(float frameBufferWidth, float frameBufferHeight);

private:
    friend Renderer;

    glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
    
    double time = 0;

};
