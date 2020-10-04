#pragma once

#include "common.h"
#include "primitives/Mesh.h"

enum CollisionType {
    Box, Sphere, MeshGeometry
};

struct RigidBodyForce {
    glm::vec3 force = glm::vec3(0.0f);
    glm::vec3 position = glm::vec3(0.0f);
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
    glm::quat angularVelocity;
    glm::quat angularAcceleration;
    glm::vec3 I = glm::vec3(1.0f);

    std::vector<RigidBodyForce> externalForces = {};
    float gravity = -9.81f;

    float mass = 1.0f;
    float staticFriction = 0.6f;
    float dynamicFriction = 0.2f;
    float bounciness = 0.2f;

    RigidBody(Mesh* mesh);
    ~RigidBody() = default;

    void applyForce(RigidBodyForce force);
    void applyForce(glm::vec3 force, glm::vec3 position = glm::vec3(0.0f));
    
    void update(double &time, double &deltaTime);

private:
    void updateGeometry();
    void handleCollision();
};
