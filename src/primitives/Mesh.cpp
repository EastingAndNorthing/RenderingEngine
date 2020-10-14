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
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        this->position = glm::vec3(x, y, z);
    }
}

glm::vec3 Mesh::getPosition() {
    return this->position;
}

void Mesh::setRotation(float x, float y, float z) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        this->rotation = glm::rotate(this->rotation, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        this->rotation = glm::normalize(this->rotation);
    }
}

void Mesh::rotate(float angle, glm::vec3 direction) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        this->rotation = glm::rotate(this->rotation, glm::radians(angle), direction);
        this->rotation = glm::normalize(this->rotation);
    }
}

glm::quat Mesh::getRotation() {
    return this->rotation;
}

glm::mat4 Mesh::getWorldPositionMatrix() {
    if(this->_worldPosMatrixNeedsUpdate) {
        this->_worldPositionMatrix = glm::translate(glm::mat4(1), this->position) * glm::mat4_cast(this->rotation); // @TODO scale
    }
    this->_worldPosMatrixNeedsUpdate = false;
    
    return this->_worldPositionMatrix;
}
