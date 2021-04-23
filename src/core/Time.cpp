#include "core/Time.h"

Time& Time::Instance() {
    static Time instance;
    return instance;
}

void Time::update() {
    this->prevTime = time;    
    this->time = glfwGetTime();
    this->dt = (this->time - this->prevTime) * this->timeScale;

    if(this->isStepMode) {
        this->dt = this->timeScale;
    }
}

void Time::slower() {
    if(this->timeScale >= this->minTimeScale)
        this->timeScale -= this->timeScaleStep;
        
    std::cout << "TimeScale: " << this->timeScale << std::endl;
}

void Time::faster() {
    if(this->timeScale <= this->maxTimeScale)
        this->timeScale += this->timeScaleStep;
        
    std::cout << "TimeScale: " << this->timeScale << std::endl;
}

void Time::setStepMode(bool mode) {
    this->isStepMode = mode;

    if(this->isStepMode == false) {
        this->timeScale = 1.0f;
        this->time = glfwGetTime();
        this->prevTime = this->time;
    } else {
        this->timeScale = this->minTimeScale;
    }

    std::cout << "Stepmode: " << this->isStepMode << std::endl;
    std::cout << "TimeScale: " << this->timeScale << std::endl;
}
