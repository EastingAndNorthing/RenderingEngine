#pragma once

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:

    float frameBufferWidth;
    float frameBufferHeight;
    
    float speed = 2.0f;
    float fov = g_settings.fov;
    float zoomspeed = 0.05f;

    bool autoRotate = true;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 lookAtPos = glm::vec3(0.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 eulerRotation = glm::vec3(-90.0f, 0.0f, 0.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    Camera(GLFWwindow* window);
    ~Camera();

    void update();
    void setProjection(float frameBufferWidth, float frameBufferHeight);

private:
    friend Renderer;
	GLFWwindow* window;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
    
    double time = 0;

    void _autoRotate();

};
