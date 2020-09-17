#include "common.h"
#include "core/Vertex.h"
#include "core/Vec3.h"

Vertex::Vertex() {}

Vertex::Vertex(Vec3 position, Vec3 normal, Vec4 color)
    : position(position), normal(normal), color(color)
{}

Vertex::Vertex(float x, float y, float z)
    : position { x, y, z }
{}

void Vertex::setAttribPointers() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, normal));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}