#include "input/Mouse.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

glm::vec2 Mouse::getDelta() {

    float posDelta = this->sensitivity * this->deltaTime;
    
    double dx = this->sensitivity * (pos.x - this->lastPos.x);
    double dy = this->sensitivity * (pos.y - this->lastPos.y);

    return glm::vec2(dx, dy);
}

void Mouse::update(GLFWwindow* window, double& deltaTime) {

    this->deltaTime = deltaTime;

    double _absolutePosX;
    double _absolutePosY;

    glfwGetCursorPos(window, &_absolutePosX, &_absolutePosY);
    glfwGetWindowSize(window, &this->windowWidth, &this->windowHeight);

    this->absolutePos.x = (float) _absolutePosX;
    this->absolutePos.y = (float) _absolutePosY;

    this->lastPos.x = this->pos.x;
    this->lastPos.y = this->pos.y;

    this->pos.x = (this->absolutePos.x / this->windowWidth) * 2 - 1;
    this->pos.y = (this->absolutePos.y / this->windowHeight) * 2 - 1;

}
