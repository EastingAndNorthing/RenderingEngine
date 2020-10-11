#pragma once

#include "primitives/Mesh.h"

class PlaneMesh : public Mesh {
public:
    PlaneMesh(float size = 1.0f);
    PlaneMesh(float length, float width);
};
    
