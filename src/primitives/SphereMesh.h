#pragma once

#include "primitives/Mesh.h"

class SphereMesh : public Mesh {
public:
    SphereMesh(float size = 1.0f, unsigned int segments = 22);
};
    
