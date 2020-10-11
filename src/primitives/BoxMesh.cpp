#include "primitives/Mesh.h"
#include "primitives/BoxMesh.h"

BoxMesh::BoxMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Box::generate(size, size, size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}

BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Box::generate(width, height, depth);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}
