#pragma once

#include "common.h"
#include "core/Vec2.h"

class Mouse {
public:

    Vec2 pos = { 0, 0 };
    Vec2 scrollpos = { 0, 0 };

	static Mouse& Instance();

	Mouse(const Mouse&) = delete;
    ~Mouse();

    Vec2 getDelta();

    Vec2 getScreenCoords();

    void update(GLFWwindow* window, double xpos, double ypos);

private:

    Mouse();

    int windowWidth;
    int windowHeight;

    Vec2 lastPos = { 0, 0 };

    double time = 0;

};
