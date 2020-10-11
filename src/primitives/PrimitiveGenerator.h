#pragma once

#include "../common.h"
#include "../core/Vertex.h"

struct PrimitiveMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    PrimitiveMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
};

namespace PrimitiveGenerator {

    namespace Box {
        PrimitiveMesh generate(const float &width, const float &height, const float &depth);
    }

    namespace Sphere {
        PrimitiveMesh generate(const float &size, const unsigned int segments);
    }

    namespace Plane {
        PrimitiveMesh generate(const float &length, const float &width);
    }

    namespace Tetrahedron {
        PrimitiveMesh generate(const float &size);
    }
};
