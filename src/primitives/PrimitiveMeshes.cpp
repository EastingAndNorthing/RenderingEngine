#include "primitives/PrimitiveMeshes.h"

BoxMesh::BoxMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Box(size, size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Box(width, height, depth);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Plane(size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Plane(length, width);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

SphereMesh::SphereMesh(float size, unsigned int segments) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Sphere(size, segments);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Tetrahedron(size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

ArrowMesh::ArrowMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Arrow(size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}
