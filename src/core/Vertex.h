#pragma once

#include "core/Vec3.h"

struct Vertex {
    Vec3 position;

    Vertex();
    Vertex(Vec3 position);
    Vertex(float x, float y, float z);

    void logPosition();
};