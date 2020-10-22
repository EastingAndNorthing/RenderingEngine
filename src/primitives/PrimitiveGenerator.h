#pragma once

#include "../common.h"
#include "../core/Vertex.h"

struct PrimitiveMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    PrimitiveMesh() = default;
    PrimitiveMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices = {});
};

namespace PrimitiveGenerator {

    PrimitiveMesh Box(const float &width = 1.0f, const float &height = 1.0f, const float &depth = 1.0f);

    PrimitiveMesh Sphere(const float &size = 1.0f, const unsigned int segments = 20);

    PrimitiveMesh Plane(const float &length = 1.0f, const float &width = 1.0f);

    PrimitiveMesh Tetrahedron(const float &size = 1.0f);

    PrimitiveMesh Arrow(const float &size = 1.0f);
    
};
