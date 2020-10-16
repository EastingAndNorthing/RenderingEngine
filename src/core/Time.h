#pragma once

#include "common.h"

class Time {
public:
    double time = 0;
    double dt = 0;
    double timeScale = 1.0f;

	static Time& Instance();

	Time(const Time&) = delete;
    ~Time() = default;

    void update();

private:
    Time() = default;

    double prevTime = 0;

};
