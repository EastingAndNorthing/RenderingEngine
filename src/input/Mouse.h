#pragma once

#include "common.h"
#include "core/Vec2.h"

class Mouse {
public:

    Vec2 pos = { 0, 0 };
    Vec2 absolutePos = { 0, 0 };
    Vec2 scrollpos = { 0, 0 };

    float sensitivity = 75.0f;

	static Mouse& Instance();

	Mouse(const Mouse&) = delete;
    ~Mouse() = default;

    Vec2 getDelta();

    void update(GLFWwindow* window, double& dt);

private:

    Mouse() = default;

    int windowWidth;
    int windowHeight;

    Vec2 lastPos = { 0, 0 };
    Vec2 deltaPosOffset = { 0, 0 };

    double deltaTime = 0;

};
