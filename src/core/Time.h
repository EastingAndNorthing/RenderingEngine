#pragma once

#include "common.h"

class Time {
public:
    double time = 0;
    double dt = 0;
    
    float timeScale = 1.0f;
    float minTimeScale = 0.0001f;
    float maxTimeScale = 8.0f;
    float timeScaleStep = 0.0001f;

    bool isStepMode = false;

	static Time& Instance();

	Time(const Time&) = delete;
    ~Time() = default;

    void update();
    void slower();
    void faster();

    void setStepMode(bool mode = false);

private:
    Time() = default;

    double prevTime = 0;

};
