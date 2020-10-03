#include "input/Mouse.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

Vec2 Mouse::getDelta() {

    float posDelta = this->sensitivity * this->deltaTime;
    
    double dx = this->sensitivity * (pos.x - this->lastPos.x);
    double dy = this->sensitivity * (pos.y - this->lastPos.y);

    return Vec2(dx, dy);
}

void Mouse::update(GLFWwindow* window, double& deltaTime) {

    this->deltaTime = deltaTime;

    glfwGetCursorPos(window, &this->absolutePos.x, &this->absolutePos.y);
    glfwGetWindowSize(window, &this->windowWidth, &this->windowHeight);

    this->lastPos.x = this->pos.x;
    this->lastPos.y = this->pos.y;

    this->pos.x = (this->absolutePos.x / this->windowWidth) * 2 - 1;
    this->pos.y = (this->absolutePos.y / this->windowHeight) * 2 - 1;

}
