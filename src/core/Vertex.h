#pragma once

#include "core/Vec3.h"

struct Vertex {
    Vec3 position;
    int textureID;

    Vertex();
    Vertex(Vec3 position);
    Vertex(float x, float y, float z);

};