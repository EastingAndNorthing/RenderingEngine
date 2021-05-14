#pragma once

#include "primitives/Mesh.h"
#include "primitives/GeometryGenerator.h"

struct TetrahedronMesh : public Mesh {
    TetrahedronMesh(float size = 1.0f);
};

struct BoxMesh : public Mesh {
    BoxMesh(float size = 1.0f);
    BoxMesh(float width, float height, float depth);
};

struct PlaneMesh : public Mesh {
    PlaneMesh(float size = 1.0f);
    PlaneMesh(float length, float width);
};

struct SphereMesh : public Mesh {
    SphereMesh(float size = 1.0f, unsigned int segments = 20);
};

struct ArrowMesh : public Mesh {
    ArrowMesh(float size = 1.0f);
};
