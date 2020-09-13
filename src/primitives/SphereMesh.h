#pragma once

#include "common.h"
#include "primitives/Mesh.h"

class SphereMesh : public Mesh {
public:

    SphereMesh(float size = 1.0f, unsigned int segments = 22);

private:

    void generate(float &width, unsigned int segments = 22);

};
    
