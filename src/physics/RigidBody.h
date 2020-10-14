#pragma once

#include "common.h"
#include "primitives/Mesh.h"
#include "physics/Collider.h"

struct RigidBodyForce {
    glm::vec3 force    = glm::vec3(0.0f);
    glm::vec3 position = glm::vec3(0.0f);
};

class RigidBody {
public:

    Mesh* mesh;                                             // Non-physical mesh that is rendered to the screen
    Collider* collider;                                     // Physics representation of shape. https://en.m.wikipedia.org/wiki/Convex_hull

    bool isDynamic = true;                                  // Whether physics apply to this object

    glm::vec3 boundingBox = glm::vec3(1.0f, 1.0f, 1.0f);    // https://en.m.wikipedia.org/wiki/Bounding_volume

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::quat rotation;                                     // https://en.m.wikipedia.org/wiki/Quaternion
    glm::quat angularVelocity;                              // https://en.m.wikipedia.org/wiki/Angular_velocity
    glm::quat angularAcceleration;                          // https://en.m.wikipedia.org/wiki/Angular_acceleration
    glm::vec3 I = glm::vec3(1.0f);                          // https://en.m.wikipedia.org/wiki/Moment_of_inertia

    float mass = 1.0f;
    float gravity = -9.81f;
    float staticFriction = 0.50f;                           // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;                          // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.8f;                                // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution

    std::vector<RigidBodyForce> externalForces = {};

    RigidBody(Mesh* mesh);
    ~RigidBody() = default;

    void applyForce(RigidBodyForce force);
    void applyForce(glm::vec3 force, glm::vec3 position = glm::vec3(0.0f));
    
    void updatePhysics(const double &deltaTime);

private:
    float sleepVelocity = 0.1f;

    void updateGeometry();
};
