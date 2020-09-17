#include "input/Mouse.h"
#include "input/WindowEventHandler.h"

Mouse& Mouse::Instance() {
    static Mouse instance;
    return instance;
}

Mouse::Mouse() {
    WindowEventHandler &windowEventHandler = WindowEventHandler::Instance();

    windowEventHandler.addMousePositionCallback(std::bind(&Mouse::update, this));
}

Mouse::~Mouse()
{}

Vec2 Mouse::getDelta() {
    return Vec2( 1, 2 );
}

void Mouse::update() {
    // glfwGetCursorPos(this->window, &this->pos.x, &this->pos.y); // FAKKKKKKKKKKKKKKK
}
