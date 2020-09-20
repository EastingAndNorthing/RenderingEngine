#include <random>
#include "../common.h"
#include "PlaneMesh.h"
#include "Mesh.h"

PlaneMesh::PlaneMesh(float size) : Mesh() {
    this->generate(size, size);
}

PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
    this->generate(length, width);
}

void PlaneMesh::generate(float &length, float &width) {
    
    float x_dist = length/2;
    float y_dist = width/2;
    
    std::vector<Vertex> geometry {
        Vertex(Vec3( -x_dist, -y_dist, 0), Vec3(0.0f, 0.0f, 1.0f)),
        Vertex(Vec3(  x_dist, -y_dist, 0), Vec3(0.0f, 0.0f, 1.0f)),
        Vertex(Vec3( -x_dist,  y_dist, 0), Vec3(0.0f, 0.0f, 1.0f)),
        Vertex(Vec3(  x_dist,  y_dist, 0), Vec3(0.0f, 0.0f, 1.0f)),
    }; 

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2
    };

    this->vertexBuffer.setVertices(geometry);
    this->indexBuffer.setIndices(indices);

}
