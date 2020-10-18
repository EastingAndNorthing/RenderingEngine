#pragma once

#include "common.h"
#include "primitives/Mesh.h"
#include "physics/Collider.h"

struct RigidBodyForce {
    glm::vec3 force    = glm::vec3(0.0f);
    glm::vec3 position = glm::vec3(0.0f);
    RigidBodyForce(glm::vec3 force, glm::vec3 position) : force(force), position(position) {}
};

class RigidBody {
public:

    Mesh* mesh = NULL;                                          // Non-physical mesh that is rendered to the screen
    Collider* collider = NULL;                                  // Physics representation of shape. https://en.m.wikipedia.org/wiki/Convex_hull

    bool isDynamic = true;                                      // Whether physics applies to this body

    glm::vec3 boundingBox = glm::vec3(1.0f);                    // https://en.m.wikipedia.org/wiki/Bounding_volume

    glm::vec3 position = glm::vec3(0);                          // In WORLD space. https://en.m.wikipedia.org/wiki/Equations_of_motion
    glm::vec3 velocity = glm::vec3(0);                          // In WORLD space. https://en.m.wikipedia.org/wiki/Equations_of_motion
    glm::vec3 acceleration = glm::vec3(0, 0, 0);                // In WORLD space. https://en.m.wikipedia.org/wiki/Equations_of_motion
    
    glm::quat rotation = glm::quat(1, 0, 0, 0);                 // In WORLD space. https://en.m.wikipedia.org/wiki/Quaternion
    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);             // In LOCAL space. https://en.m.wikipedia.org/wiki/Angular_velocity
    glm::vec3 angularVelocityW = glm::vec3(0, 0, 0);            // In WORLD space. Precomputed from angularVelocity.
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);         // In LOCAL space. https://en.m.wikipedia.org/wiki/Angular_acceleration
    
    // glm::mat3 inertiaTensor = glm::mat3(0.1331712f);             // In LOCAL space. https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size, unit mass = 0.1331712
    glm::mat3 inertiaTensor = glm::mat3(2.0f);                 // In LOCAL space. https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size with unit mass = 0.1331712       

    float mass = 1.0f;
    float gravity = -9.81f;                                     // In WORLD space.
    float staticFriction = 0.50f;                               // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;                              // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.5f;                                    // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution
    float rotationalDamping = 0.996f;
    
    float _inverseMass;
    glm::mat3 _inertiaTensorW;                                  // In WORLD space. Precomputed from inertiaTensor.
    glm::mat3 _inverseInertiaTensor;                            // In WORLD space. Precomputed from inertiaTensor.
    glm::mat3 _inverseInertiaTensorW;                           // In WORLD space. Precomputed from inertiaTensor.     
    glm::quat _inverseRotation = glm::quat(0, 0, 0, 0);         // In WORLD space. https://en.m.wikipedia.org/wiki/Quaternion

    std::vector<RigidBodyForce> externalForces = {};            // In WORLD space.
    glm::vec3 torque = glm::vec3(0);                            // In LOCAL space. https://en.m.wikipedia.org/wiki/Torque

    RigidBody() = default;
    RigidBody(Mesh* mesh = NULL, Collider* collider = NULL);
    ~RigidBody() = default;

    void applyForce(RigidBodyForce force);
    void applyForce(glm::vec3 forceW, glm::vec3 localPosition = glm::vec3(0.0f));
    
    void applyLocalImpulse(const glm::vec3& impulse, const glm::vec3& position);
    void applyWorldImpulse(const glm::vec3& impulse, const glm::vec3& position);

    void rebuildPrecomputedValues();

    void updatePhysics(const double &deltaTime);

    void makeStatic();

private:
    float sleepVelocity = 0.05f;
    float sleepAngularVelocity = 0.08f;

    bool _inertiaNeedsUpdate   = true;
    bool _massNeedsUpdate      = true;
    bool _rotationNeedsUpdate  = true;

    void updateGeometry();
};
