#include "primitives/Mesh.h"
#include "primitives/TetrahedronMesh.h"

TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
    PrimitiveMesh geometry = PrimitiveGenerator::Tetrahedron::generate(size);
    this->vertexBuffer.setVertices(geometry.vertices);
    this->indexBuffer.setIndices(geometry.indices);
}
