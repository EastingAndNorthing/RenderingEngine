#include "../common.h"
#include "core/Vec3.h"
#include "TetrahedronMesh.h"
#include "Mesh.h"

TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
    this->generate(size);
}

void TetrahedronMesh::generate(float &size) {
    
    float sqrt8over9component = 0.9428090415820633658 * size;
    float sqrt2over9component = 0.4714045207910316829 * size;
    float sqrt2over3component = 0.8164965809277260327 * size;
    float oneThirdComponent = 0.333333333333333333 * size;

    std::vector<Vertex> tetra {
        Vertex(Vec3(0, -oneThirdComponent, sqrt8over9component)),
        Vertex(Vec3(sqrt2over3component, -oneThirdComponent, -sqrt2over9component)), 
        Vertex(Vec3(-sqrt2over3component, -oneThirdComponent, -sqrt2over9component)),
        Vertex(Vec3(0, size, 0)),
    };

    std::vector<unsigned int> indices = {
        2, 1, 0,
        1, 2, 3,
        0, 3, 2,
        1, 3, 0
    };

    this->vertexBuffer.setVertices(tetra);
    this->indexBuffer.setIndices(indices);

}
