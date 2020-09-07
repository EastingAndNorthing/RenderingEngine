#include "../common.h"
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

    std::vector<Vertex> box {
        Vertex({ sqrt8over9component, 0, -oneThirdComponent }),
        Vertex({ -sqrt2over9component,  sqrt2over3component, -oneThirdComponent }), 
        Vertex({ -sqrt2over9component, -sqrt2over3component, -oneThirdComponent }),
        Vertex({ 0, 0, size }),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3,
        2, 0, 3,
        0, 1, 3
    };

    this->vertexBuffer.setVertices(box);
    this->indexBuffer.setIndices(indices);

}
