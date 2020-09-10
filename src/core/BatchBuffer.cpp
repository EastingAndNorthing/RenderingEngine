#include "core/BatchBuffer.h"
#include "primitives/Mesh.h"


BatchBuffer::BatchBuffer() {
    
    // int size = 1024;

    // float x_dist = 1.0/2;
    // float y_dist = 1.0/2;
    // float z_dist = 1.0/2;
    
    // std::vector<Vertex> box {
    //     Vertex({-x_dist, -y_dist, -z_dist}), // Front
    //     Vertex({ x_dist, -y_dist, -z_dist}), // Front
    //     Vertex({ x_dist,  y_dist, -z_dist}), // Front
    //     Vertex({-x_dist,  y_dist, -z_dist}), // Front
    //     Vertex({ x_dist, -y_dist,  z_dist}), // Back
    //     Vertex({ x_dist,  y_dist,  z_dist}), // Back
    //     Vertex({-x_dist,  y_dist,  z_dist}), // Back
    //     Vertex({-x_dist, -y_dist,  z_dist}), // Back
    // }; 

    // std::vector<unsigned int> indices = {
    //     2, 1, 0, // Front
    //     3, 2, 0, // Front
    //     1, 5, 4, // Right
    //     1, 2, 5, // Right
    //     4, 5, 6, // Back
    //     4, 6, 7, // Back
    //     7, 6, 3, // Left
    //     7, 3, 0, // Left
    //     0, 1, 7, // Bottom
    //     1, 4, 7, // Bottom
    //     2, 3, 6, // Top
    //     2, 6, 5, // Top
    // };

    // glGenVertexArrays(1, &this->vao);
    // glBindVertexArray(this->vao);

    // glGenBuffers(1, &this->vbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    // glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_DYNAMIC_DRAW);
    
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // glEnableVertexAttribArray(1);

    // glGenBuffers(1, &this->ibo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &this->vao);
    
    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->size, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &this->ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

}

void BatchBuffer::addGeometry(Mesh* mesh) {
    
    auto vertices = mesh->vertexBuffer.vertices;
    auto indices = mesh->indexBuffer.indices;

    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, this->currentVerticesSize, mesh->vertexBuffer.getSize(), &vertices);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->currentIndicesSize, mesh->indexBuffer.getSize(), &indices);
    
    this->currentVerticesSize += mesh->vertexBuffer.getSize();
    this->currentIndicesSize += mesh->indexBuffer.getSize();
}

// void BatchBuffer::addGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
//     this->vertices.insert(this->vertices.end(), vertices.begin(), this->vertices.end());
//     this->indices.insert(this->indices.end(), indices.begin(), this->indices.end());
// }

void BatchBuffer::setMaterial(Material* material) {
    this->material = material;
}

void BatchBuffer::Bind() {
    this->material->Bind();
    glBindVertexArray(this->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    // Testing
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

void BatchBuffer::UnBind() {
    glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
}

BatchBuffer::~BatchBuffer() {
    glDeleteBuffers(1, &this->vbuffer);
    glDeleteBuffers(1, &this->ibuffer);
    glDeleteBuffers(1, &this->vao);
}