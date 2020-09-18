#include "input/Mouse.h"
#include "input/WindowEventHandler.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

Mouse::Mouse() {}

Mouse::~Mouse() {}

Vec2 Mouse::getDelta() {
    return this->pos;
}

Vec2 Mouse::getScreenCoords() {
    return Vec2(
        (pos.x / windowWidth) * 2 - 1,
        (pos.y / windowHeight) * 2 - 1
    );
}

void Mouse::update(GLFWwindow* window, double xpos, double ypos) {
    
    glfwGetWindowSize(window, &this->windowWidth, &this->windowHeight);

    this->lastPos.x = this->pos.x;
    this->lastPos.y = this->pos.y;

    this->pos.x = xpos;
    this->pos.y = ypos;

    // Vec2 relativePos = this->getScreenCoords();
    // std::cout << relativePos.x << " " << relativePos.y << std::endl;

    this->time = glfwGetTime();
}
