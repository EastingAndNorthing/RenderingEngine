#pragma once

#include "core/Vec3.h"

struct Vertex {
    Vec3 position;
    Vec3 color;

    Vertex();
    Vertex(Vec3 position, Vec3 color = {});
    Vertex(float x, float y, float z);

};