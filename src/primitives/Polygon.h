#pragma once

#include "core/Vertex.h"
#include "core/Vec3.h"

struct Polygon {
    Vertex v1, v2, v3;

    Polygon();

    Polygon(Vertex v1, Vertex v2, Vertex v3);
};
