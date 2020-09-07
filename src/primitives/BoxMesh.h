#pragma once

#include "common.h"
#include "primitives/Mesh.h"

class BoxMesh : public Mesh {
public:

    BoxMesh(float size = 1.0f);

    BoxMesh(float width, float height, float depth);

private:

    void generate(float &width, float &height, float &depth);

};
    
