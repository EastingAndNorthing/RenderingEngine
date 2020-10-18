#pragma once

#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Material.h"
#include "primitives/PrimitiveGenerator.h"

class Mesh {
public: 

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    Material* material = NULL;
    
    bool managedByRigidBody = false;

    Mesh();
    Mesh(const VertexBuffer& vertexBuffer, const IndexBuffer &indexBuffer);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int> &indices = {});
    virtual ~Mesh();

    void assignMaterial(Material* material);
    void Bind();
    void Unbind();
    
    void setPosition(glm::vec3 position);

    void setRotation(const glm::vec3& eulerRotation);
    void setRotation(const glm::quat& rotation);
    void alignRotation(const glm::vec3& directionVector, const glm::vec3& up = glm::vec3(0, 1, 0));
    
    void setScale(const glm::vec3& scale);
    
    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 direction);

    glm::vec3 getPosition();
    glm::quat getRotation();
    glm::vec3 getScale();

    glm::mat4 getWorldPositionMatrix();

private:
    friend RigidBody;
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0, 0, 0);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    bool _worldPosMatrixNeedsUpdate = true;

    glm::mat4 _worldPositionMatrix = glm::mat4(1.0f);
};

#include "primitives/BoxMesh.h"
#include "primitives/PlaneMesh.h"
#include "primitives/SphereMesh.h"
#include "primitives/TetrahedronMesh.h"
