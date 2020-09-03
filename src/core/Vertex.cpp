#include <vector>
#include <iostream>
#include "core/Vertex.h"
#include "core/Vec3.h"

Vertex::Vertex() {
    std::cout << "Called default vertex constructor?" << std::endl;
    position = { 0, 0, 0 };
}

Vertex::Vertex(Vec3 position, Vec3 color)
    : position(position), color(color)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}
