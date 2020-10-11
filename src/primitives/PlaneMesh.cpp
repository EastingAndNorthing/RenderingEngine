#include "primitives/Mesh.h"
#include "primitives/PlaneMesh.h"

PlaneMesh::PlaneMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Plane::generate(size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Plane::generate(length, width);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}
