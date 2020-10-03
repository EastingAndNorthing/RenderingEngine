#pragma once

#include "common.h"
#include "primitives/Mesh.h"

enum CollisionType {
    Box, Sphere, MeshGeometry
};

class RigidBody {
public:

    Mesh* mesh;
    CollisionType collisionType = Box;

    glm::vec3 boundingBox = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation;

    float mass = 1.0f;

    RigidBody();
    RigidBody(Mesh* mesh);
    ~RigidBody() = default;
    
    void update(double &time, double &deltaTime, float &gravity);

private:
    void updateGeometry();
};
