#include <vector>
#include <iostream>
#include "core/Vertex.h"
#include "core/Vec3.h"

Vertex::Vertex() {
    std::cout << "Default vertex constructor" << std::endl;
    position = { 0, 0, 0 };
}

Vertex::Vertex(Vec3 position)
    : position(position)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}

void Vertex::logPosition() {
    std::cout << position.x << std::endl;
}