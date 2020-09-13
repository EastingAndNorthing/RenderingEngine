#include "common.h"
#include "core/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() {
    this->projectionMatrix = glm::perspective(glm::radians(g_settings.fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.1f, 100.0f);
}

Camera::~Camera() {
    
}

void Camera::update() {
    if(this->autoRotation) {
        this->autoRotate();
    } else {
        this->viewMatrix = glm::lookAt(this->position, this->lookAtPos, glm::vec3(0.0, 1.0, 0.0));
    }

    this->viewProjectionMatrix = this->projectionMatrix * this->viewMatrix;
}

void Camera::setProjection(float frameBufferWidth, float frameBufferHeight) {
    this->frameBufferWidth = frameBufferWidth;
    this->frameBufferHeight = frameBufferHeight;
    this->projectionMatrix = glm::perspective(glm::radians(this->fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.1f, 100.0f);
}

void Camera::autoRotate() {
    const float camRadius = 1.5f; // Should be a simple distance from the current pos to the lookAt pos.
    float camX = sin(glfwGetTime()) * camRadius;
    float camZ = cos(glfwGetTime()) * camRadius;

    this->viewMatrix = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::setEulerAngles(float x, float y, float z) {
    
}