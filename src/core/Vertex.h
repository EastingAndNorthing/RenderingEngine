#pragma once

#include "core/Vec3.h"
#include "core/Vec4.h"

struct Vertex {
    Vec3 position = { 0, 0, 0 };
    Vec4 color = { 0, 0, 0, 0 };

    Vertex();
    Vertex(Vec3 position, Vec4 color);
    Vertex(float x, float y, float z);

};