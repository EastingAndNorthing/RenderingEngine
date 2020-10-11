#pragma once

#include "../common.h"
#include "../core/Vertex.h"

struct Polygon {
    std::vector<Vertex> vertices;
    Polygon(const std::vector<Vertex> &vertices) : vertices(vertices) {};
    ~Polygon() = default;
};
