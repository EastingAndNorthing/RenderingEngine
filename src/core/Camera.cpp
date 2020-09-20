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
    Mouse &mouse = Mouse::Instance();

    if(this->autoRotation) {
        this->autoRotate();
    } else {

        double dt = _time - this->time;
        float posDelta = this->speed * dt;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->position += posDelta * this->front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->position -= posDelta * this->front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->position -= glm::normalize(glm::cross(this->front, this->up)) * posDelta;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            this->position += glm::normalize(glm::cross(this->front, this->up)) * posDelta;

        Vec2 mouseDelta = mouse.getDelta();

        this->eulerRotation.x += mouseDelta.x;
        this->eulerRotation.y -= mouseDelta.y;

        if (this->eulerRotation.y > 89.5f)
            this->eulerRotation.y = 89.5f;
        if (this->eulerRotation.y < -89.5f)
            this->eulerRotation.y = -89.5f;

        glm::vec3 _front;
        _front.x = cos(glm::radians(this->eulerRotation.x)) * cos(glm::radians(this->eulerRotation.y));
        _front.y = sin(glm::radians(this->eulerRotation.y));
        _front.z = sin(glm::radians(this->eulerRotation.x)) * cos(glm::radians(this->eulerRotation.y));

        this->front = glm::normalize(_front);
        this->right = glm::normalize(glm::cross(this->front, glm::vec3(0.0f, 1.0f, 0.0f)));
        this->up    = glm::normalize(glm::cross(this->right, this->front));

        this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);

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
