#include "core/BatchBuffer.h"
#include "core/Vertex.h"
#include "primitives/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


BatchBuffer::BatchBuffer() {
    this->generateBuffers();
}

void BatchBuffer::generateBuffers() {

    std::cout << "Generating new BatchBuffer" << std::endl;

    glDeleteBuffers(1, &this->vbuffer);
    glDeleteBuffers(1, &this->ibuffer);
    glDeleteBuffers(1, &this->vao);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->size, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &this->ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size, nullptr, GL_DYNAMIC_DRAW);

    std::cout << this->verticesCount << std::endl;
    std::cout << this->verticesSize << std::endl;
    std::cout << this->indicesCount << std::endl;
    std::cout << this->indicesSize << std::endl;

}

void BatchBuffer::addGeometry(Mesh* mesh) {

    auto meshVertices = mesh->vertexBuffer.vertices;
    auto meshIndices = mesh->indexBuffer.indices;

    this->vertices.insert(this->vertices.end(), meshVertices.begin(), meshVertices.end());
    this->indices.insert(this->indices.end(), meshIndices.begin(), meshIndices.end());      // Should probably increment all indices
    
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferSubData(GL_ARRAY_BUFFER, this->verticesSize, mesh->vertexBuffer.getSize(), &meshVertices[0]);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->indicesSize, mesh->indexBuffer.getSize(), &meshIndices[0]);
    
    this->verticesCount += meshVertices.size();
    this->verticesSize += meshVertices.size() * sizeof(Vertex);
    this->indicesCount += meshIndices.size();
    this->indicesSize += meshIndices.size() * sizeof(unsigned int);
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
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibuffer);

    // Testing
    // // How to handle u_mvp? Use glBufferSubData() each frame, bake vertex locations?
    // glm::mat4 model = glm::translate(glm::mat4(1), mesh->position) * mesh->rotation;
    // glm::mat4 u_mvp = model;
    // glUniformMatrix4fv(this->material->shader->getProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(u_mvp));
    
    std::cout << this->indicesCount << std::endl;
    glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
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