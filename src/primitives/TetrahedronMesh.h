#pragma once

#include "common.h"
#include "primitives/Mesh.h"

class TetrahedronMesh : public Mesh {
public:

    TetrahedronMesh(float size = 1.0f);

private:

    void generate(float &size);

};
    
