#include "primitives/PrimitiveMeshes.h"

BoxMesh::BoxMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Box(size, size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Box(width, height, depth);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Plane(size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Plane(length, width);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

SphereMesh::SphereMesh(float size, unsigned int segments) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Sphere(size, segments);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Tetrahedron(size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

ArrowMesh::ArrowMesh(float size) : Mesh() {

}
