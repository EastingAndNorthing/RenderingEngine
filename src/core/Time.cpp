#include "core/Time.h"

Time& Time::Instance() {
    static Time instance;
    return instance;
}

void Time::update() {
    this->prevTime = time;
    this->time = glfwGetTime();
    this->dt = (this->time - this->prevTime) * this->timeScale;
}
