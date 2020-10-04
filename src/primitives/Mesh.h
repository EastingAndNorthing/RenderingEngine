#pragma once

#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Material.h"

class Mesh {
public: 

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    Material* material = NULL;
    
    bool managedByRigidBody = false;

    Mesh();
    Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer);
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
    Mesh(std::vector<Vertex> &vertices);
    Mesh(std::vector<Vertex> vertices);
    virtual ~Mesh();

    void assignMaterial(Material &material);
    void Bind();
    
    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 direction);

    glm::vec3 getPosition();
    glm::mat4 getRotation();
    glm::vec3 getScale();

    glm::mat4 getWorldPositionMatrix();

private:
    friend RigidBody;
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    bool _worldPosMatrixNeedsUpdate = true;

    glm::mat4 _worldPositionMatrix = glm::mat4(1.0f);
};
