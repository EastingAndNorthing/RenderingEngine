#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "primitives/Mesh.h"

Mesh::Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer)
    : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), material(nullptr)
{}

Mesh::Mesh()
{}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) 
    : vertexBuffer(vertices), indexBuffer(indices), material(nullptr)
{}

Mesh::Mesh(std::vector<Vertex> &vertices) 
    : vertexBuffer(vertices), indexBuffer(), material(nullptr)
{}

Mesh::Mesh(std::vector<Vertex> vertices) 
    : vertexBuffer(vertices), indexBuffer(), material(nullptr)
{}

Mesh::~Mesh()
{}

void Mesh::assignMaterial(Material &material) {
    this->material = &material;
}

void Mesh::Bind() {
    this->vertexBuffer.Bind();
    this->indexBuffer.Bind();
    this->material->Bind();
}

void Mesh::setPosition(float x, float y, float z) {
    this->worldPosMatrixNeedsUpdate = true;
    this->_position = glm::vec3(x, y, z);
}

glm::vec3 Mesh::getPosition() {
    return this->_position;
}

void Mesh::setRotation(float x, float y, float z) {
    this->worldPosMatrixNeedsUpdate = true;
    this->_rotation = glm::rotate(this->_rotation, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
    this->_rotation = glm::rotate(this->_rotation, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->_rotation = glm::rotate(this->_rotation, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Mesh::rotate(float angle, glm::vec3 direction) {
    this->worldPosMatrixNeedsUpdate = true;
    this->_rotation = glm::rotate(this->_rotation, glm::radians(angle), direction);
}

glm::mat4 Mesh::getRotation() {
    return this->_rotation;
}

glm::mat4 Mesh::getWorldPositionMatrix() {
    if(this->worldPosMatrixNeedsUpdate) {
        this->_worldPositionMatrix = glm::translate(glm::mat4(1), this->_position) * this->_rotation; // @TODO scale
    }
    this->worldPosMatrixNeedsUpdate = false;
    
    return this->_worldPositionMatrix;
}
