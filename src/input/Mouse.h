#pragma once

#include "common.h"
#include "core/Vec2.h"

class Mouse {
public:

    Vec2 pos = { 0, 0 };

	static Mouse& Instance();

	Mouse(const Mouse&) = delete;
    ~Mouse();

    Vec2 getDelta();

    void update();

private:
    Mouse();

    double time = 0;

    float lastx = 0;
    float lasty = 0;

    float scrollx = 0;
    float scrolly = 0;
    
};
