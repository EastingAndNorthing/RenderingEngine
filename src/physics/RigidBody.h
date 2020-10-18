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

    bool isDynamic = true;                                      // Whether physics applies to this object

    glm::vec3 boundingBox = glm::vec3(1.0f);                    // https://en.m.wikipedia.org/wiki/Bounding_volume

    glm::vec3 position = glm::vec3(0);                          // https://en.m.wikipedia.org/wiki/Equations_of_motion
    glm::vec3 velocity = glm::vec3(0);                          // https://en.m.wikipedia.org/wiki/Equations_of_motion
    glm::vec3 acceleration = glm::vec3(0, 0, 0);                // https://en.m.wikipedia.org/wiki/Equations_of_motion
    
    glm::quat rotation = glm::quat(1, 0, 0, 0);                 // https://en.m.wikipedia.org/wiki/Quaternion
    glm::quat inverseRotation = glm::quat(0, 0, 0, 0);          // https://en.m.wikipedia.org/wiki/Quaternion
    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);             // https://en.m.wikipedia.org/wiki/Angular_velocity LOCAL angular velocity vector 
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);         // https://en.m.wikipedia.org/wiki/Angular_acceleration

    glm::vec3 torque = glm::vec3(0);                            // https://en.m.wikipedia.org/wiki/Torque
    
    glm::mat3 inertiaTensor = glm::mat3(0.1331712);             // https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size, unit mass = 0.1331712
    // glm::mat3 inertiaTensor = glm::mat3(1.0f);             // https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size with unit mass = 0.1331712
    glm::mat3 inertiaTensorW;                                   // Precomputed world space inertia
    glm::mat3 inverseInertiaTensor;                             // https://stackoverflow.com/questions/18290798/calculating-rigid-body-inertia-tensor-world-coordinates
    glm::mat3 inverseInertiaTensorW;                            // Precomputed inverse world space inertia              

    float mass = 1.0f;
    float inverseMass;
    float gravity = -9.81f;
    float staticFriction = 0.50f;                               // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;                              // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.5f;                                    // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution

    std::vector<RigidBodyForce> externalForces = {};

    RigidBody() = default;
    RigidBody(Mesh* mesh = NULL, Collider* collider = NULL);
    ~RigidBody() = default;

    void applyForce(RigidBodyForce force);
    void applyForce(glm::vec3 forceW, glm::vec3 localPosition = glm::vec3(0.0f));
    
    void applyImpulse(const glm::vec3& impulse, const glm::vec3& position);

    void rebuildPrecomputedValues();

    void updatePhysics(const double &deltaTime);

    void makeStatic();

private:
    float sleepVelocity = 0.08f;
    float sleepAngularVelocity = 0.01f;

    bool _inertiaNeedsUpdate   = true;
    bool _massNeedsUpdate      = true;
    bool _rotationNeedsUpdate  = true;

    void updateGeometry();
};
