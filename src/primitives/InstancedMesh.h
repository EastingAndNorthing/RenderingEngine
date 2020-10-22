#pragma once

#include "primitives/Mesh.h"

struct InstancedMesh : public Mesh {
    unsigned int instanceBuffer = 0;
    unsigned int instanceCount = 1;

    InstancedMesh(const PrimitiveMesh& primitiveMesh, unsigned int instanceCount);

    void Bind();
    void Unbind();
    
};

