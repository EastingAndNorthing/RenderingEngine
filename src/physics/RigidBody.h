#pragma once

#include "common.h"
#include "primitives/Mesh.h"
#include "physics/Collider.h"
#include "physics/Pose.h"
#include "math/CoordinateSystem.h"
#include "math/Quaternion.h"

struct RigidBodyForce {
    glm::vec3 force    = glm::vec3(0.0f);   // WORLD space
    glm::vec3 position = glm::vec3(0.0f);   // LOCAL position
    RigidBodyForce(glm::vec3 force, glm::vec3 position) : force(force), position(position) {}
};

class RigidBody {
public:

    Mesh* mesh = NULL;                                          // Non-physical mesh that is rendered to the screen
    Collider* collider = NULL;                                  // Physics representation of shape. https://en.m.wikipedia.org/wiki/Convex_hull
    glm::vec3 boundingBox = glm::vec3(1.0f);                    // https://en.m.wikipedia.org/wiki/Bounding_volume

    Pose pose = Pose();
    Pose prevPose = Pose();
    
    glm::vec3 vel = glm::vec3(0.0f);                            
    glm::vec3 omega = glm::vec3(0.0f);                          // https://en.m.wikipedia.org/wiki/Angular_velocity
    
    float invMass = 1.0f;
    glm::vec3 invInertia = glm::vec3(1.0f);                     // https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size with unit mass = 0.1331712
    
    bool isDynamic = true;                                      // Whether physics applies to this body

    std::vector<RigidBodyForce> externalForces = {};            // In WORLD space.
    glm::vec3 forces = glm::vec3(0);                            // In WORLD space.
    glm::vec3 torque = glm::vec3(0);                            // In LOCAL space. https://en.m.wikipedia.org/wiki/Torque

    float mass = 1.0f;
    float gravity = -9.81f;                                     // In WORLD space.
    float staticFriction = 0.50f;                               // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;                              // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.5f;                                    // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution
    float rotationalDamping = 0.996f;
    
    RigidBody() = default;
    RigidBody(Mesh* mesh = NULL, Collider* collider = NULL);
    ~RigidBody() = default;

    void applyRotation(glm::vec3 rot, float scale = 1.0);
    void applyCorrection(glm::vec3 corr, glm::vec3 pos = glm::vec3(0.0f), bool velocityLevel = false);

    void integrate(const float &deltaTime);
    void update(const float &deltaTime);

    void updateCollider();
    
    void rebuildPrecomputedValues();
    float getInverseMass(glm::vec3& normal, glm::vec3 pos = glm::vec3(0.0f));

    glm::vec3 getVelocityAt(glm::vec3& pos);

    void makeStatic();

    // void applyForceL(RigidBodyForce force);
    // void applyForceL(glm::vec3 forceW, glm::vec3 positionL = glm::vec3(0.0f));
    // void applyForceW(RigidBodyForce force);
    // void applyForceW(glm::vec3 forceW, glm::vec3 positionW = glm::vec3(0.0f));
    
    // void applyLocalImpulse(const glm::vec3& impulse, const glm::vec3& position);
    // void applyWorldImpulse(const glm::vec3& impulse, const glm::vec3& position);

    // glm::vec3 getAngularVelocityW();
    glm::vec3 getPointVelocityL(const glm::vec3& localPoint);
    glm::vec3 getPointVelocityW(glm::vec3 point, bool isPointInLocalSpace = false);



private:
    float sleepVelocity = 0.05f;
    float sleepAngularVelocity = 0.08f;

    bool _inertiaNeedsUpdate   = true;
    bool _massNeedsUpdate      = true;
    bool _rotationNeedsUpdate  = true;

    void updateGeometry();
};
