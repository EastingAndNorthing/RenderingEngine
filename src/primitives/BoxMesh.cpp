#include <random>
#include "../common.h"
#include "BoxMesh.h"
#include "Mesh.h"

BoxMesh::BoxMesh(float size) : Mesh() {
    this->generate(size, size, size);
}

BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
    this->generate(width, height, depth);
}

void BoxMesh::generate(float &width, float &height, float &depth) {
    
    float x_dist = width/2;
    float y_dist = height/2;
    float z_dist = depth/2;
    
    std::vector<Vertex> box {
        Vertex({-x_dist, -y_dist, -z_dist}), // Front
        Vertex({ x_dist, -y_dist, -z_dist}), // Front
        Vertex({ x_dist,  y_dist, -z_dist}), // Front
        Vertex({-x_dist,  y_dist, -z_dist}), // Front
        Vertex({ x_dist, -y_dist,  z_dist}), // Back
        Vertex({ x_dist,  y_dist,  z_dist}), // Back
        Vertex({-x_dist,  y_dist,  z_dist}), // Back
        Vertex({-x_dist, -y_dist,  z_dist}), // Back
    }; 

    // @TODO remove test code
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randR(0, 1);
    std::uniform_real_distribution<> randG(0, 1);
    std::uniform_real_distribution<> randB(0, 1);
    

    for (int i = 0; i < box.size(); i++) {
        box[i].color = Vec4(randR(gen), randG(gen), randB(gen), 1.0);
    }

    std::vector<unsigned int> indices = {
        0, 1, 2, // Front
        0, 2, 3, // Front
        1, 4, 5, // Right
        1, 5, 2, // Right
        4, 5, 6, // Back
        4, 7, 6, // Back
        7, 6, 3, // Left
        7, 0, 3, // Left
        0, 1, 7, // Bottom
        1, 4, 7, // Bottom
        2, 3, 6, // Top
        2, 5, 6, // Top
    };

    this->vertexBuffer.setVertices(box);
    this->indexBuffer.setIndices(indices);

}