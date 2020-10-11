#pragma once

#include "common.h"
#include "primitives/Polygon.h"
#include "primitives/PrimitiveGenerator.h"

class Collider {
public:

    std::vector<Polygon> polygons;

    glm::vec3 relativePos = glm::vec3(0.0f);

    Collider();
    ~Collider() = default;

private:
};
