#include "primitives/Mesh.h"
#include "primitives/SphereMesh.h"

SphereMesh::SphereMesh(float size, unsigned int segments) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Sphere::generate(size, segments);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}
