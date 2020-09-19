#include "common.h"
#include "core/Camera.h"
#include "input/Mouse.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(GLFWwindow* window) 
    : window(window)
{
    this->projectionMatrix = glm::perspective(glm::radians(g_settings.fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.001f, 100.0f);
}

Camera::~Camera() {}

void Camera::update() {

    double _time = glfwGetTime();

    if(this->autoRotation) {
        this->autoRotate();
    } else {

        double dt = _time - this->time;
        float posDelta = this->speed * dt;
        
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        glm::vec3 cameraDirection = glm::normalize(this->position - this->lookAtPos);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->position += posDelta * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->position -= posDelta * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * posDelta;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->position += glm::normalize(glm::cross(cameraFront, cameraUp)) * posDelta;

        this->viewMatrix = glm::lookAt(this->position, this->position + cameraFront, cameraUp);

    }
    
    this->time = _time;

    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setProjection(float frameBufferWidth, float frameBufferHeight) {
    this->frameBufferWidth = frameBufferWidth;
    this->frameBufferHeight = frameBufferHeight;
    this->projectionMatrix = glm::perspective(glm::radians(this->fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.1f, 100.0f);
}

void Camera::autoRotate() {
    const float camRadius = 1.5f;
    float camX = sin(glfwGetTime()) * camRadius;
    float camZ = cos(glfwGetTime()) * camRadius;

    this->viewMatrix = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setEulerAngles(float x, float y, float z) {
    
}
