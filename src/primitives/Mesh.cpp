#include "primitives/Mesh.h"

Mesh::Mesh(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
    : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), material(nullptr)
{}

Mesh::Mesh()
{}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) 
    : vertexBuffer(vertices), indexBuffer(indices), material(nullptr)
{}

Mesh::~Mesh()
{}

void Mesh::assignMaterial(Material* material) {
    this->material = material;
}

void Mesh::Bind() {
    this->vertexBuffer.Bind();
    this->indexBuffer.Bind();
    this->material->Bind();
}

void Mesh::Unbind() {
    this->vertexBuffer.Unbind();
    this->indexBuffer.Unbind();
}

void Mesh::setPosition(glm::vec3 position) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        this->position = position;
    }
}

glm::vec3 Mesh::getPosition() {
    return this->position;
}

void Mesh::alignRotation(const glm::vec3& directionVector, const glm::vec3& up) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        
        // Builds quaternion from 2 vectors @TODO move to a better place
        // http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
        glm::vec3 u = glm::normalize(directionVector);
        glm::vec3 v = glm::normalize(up);
        
        float m = sqrt(2.0f + 2.0f * dot(u, v));
        glm::vec3 w = (1.0f / m) * cross(u, v);
        this->rotation = glm::normalize(glm::quat(0.5f * m, w.x, w.y, w.z));
    }
}

void Mesh::setRotation(const glm::quat& rotation) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;
        this->rotation = glm::normalize(rotation);
    }
}

void Mesh::setRotation(const glm::vec3& eulerRotation) {
    if(!managedByRigidBody) {
        this->_worldPosMatrixNeedsUpdate = true;

        // There must be a faster way
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        this->rotation = glm::rotate(this->rotation, glm::radians(eulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        this->rotation = glm::normalize(this->rotation);
    }
}

void Mesh::setScale(const glm::vec3& scale) {
    this->scale = scale;
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
        this->_worldPositionMatrix = glm::scale(glm::translate(glm::mat4(1), this->position) * glm::mat4_cast(this->rotation), this->scale);
        this->_worldPosMatrixNeedsUpdate = false;
    }
    
    return this->_worldPositionMatrix;
}
