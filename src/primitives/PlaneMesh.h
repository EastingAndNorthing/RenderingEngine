#pragma once

#include "common.h"
#include "primitives/Mesh.h"

class PlaneMesh : public Mesh {
public:

    PlaneMesh(float size = 1.0f);
    PlaneMesh(float length, float width);

private:

    void generate(float &length, float &width);

};
    
