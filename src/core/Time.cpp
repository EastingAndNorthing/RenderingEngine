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

void Time::slower() {
    if(this->timeScale >= 0.01f)
        this->timeScale -= 0.01f;
        
    std::cout << this->timeScale << std::endl;
}

void Time::faster() {
    if(this->timeScale <= 5.0f)
        this->timeScale += 0.01f;
        
    std::cout << this->timeScale << std::endl;
}