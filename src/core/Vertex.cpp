#include <vector>
#include <iostream>
#include "core/Vertex.h"
#include "core/Vec3.h"

Vertex::Vertex() {
    position = { 0, 0, 0 };
}

Vertex::Vertex(Vec3 position, Vec4 color)
    : position(position), color(color)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}
