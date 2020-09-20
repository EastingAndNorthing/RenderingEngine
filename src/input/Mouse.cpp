#include "input/Mouse.h"
#include "input/WindowEventHandler.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

Mouse::Mouse() {}

Mouse::~Mouse() {}

Vec2 Mouse::getDelta() {

    double _time = glfwGetTime();

    double dt = _time - this->time;
    float posDelta = this->sensitivity * dt;
    
    double dx = this->sensitivity * (pos.x - this->lastPos.x);
    double dy = this->sensitivity * (pos.y - this->lastPos.y);

    return Vec2(dx, dy);
}

void Mouse::update(GLFWwindow* window) {

    glfwGetCursorPos(window, &this->absolutePos.x, &this->absolutePos.y);
    glfwGetWindowSize(window, &this->windowWidth, &this->windowHeight);

    this->lastPos.x = this->pos.x;
    this->lastPos.y = this->pos.y;

    this->pos.x = (this->absolutePos.x / this->windowWidth) * 2 - 1;
    this->pos.y = (this->absolutePos.y / this->windowHeight) * 2 - 1;

    this->time = glfwGetTime();
}
