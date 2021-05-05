#pragma once

#include "common.h"
#include "primitives/Mesh.h"
#include "physics/Collider.h"
#include "physics/Pose.h"
#include "math/CoordinateSystem.h"
#include "math/Quaternion.h"

class RigidBody {
public:

    Pose pose = Pose();
    Pose prevPose = Pose();

    bool isDynamic = true;                                      // Whether physics applies to this body
    bool useVelocitySolve = false;

    Mesh* mesh = NULL;                                          // Non-physical mesh that is rendered to the screen
    Collider* collider = NULL;                                  // Physics representation of shape. https://en.m.wikipedia.org/wiki/Convex_hull
    glm::vec3 boundingBox = glm::vec3(1.0f);                    // https://en.m.wikipedia.org/wiki/Bounding_volume
    
    glm::vec3 vel = glm::vec3(0);                            
    glm::vec3 omega = glm::vec3(0);                             // https://en.m.wikipedia.org/wiki/Angular_velocity
    
    float invMass = 1.0f;
    glm::vec3 invInertia = glm::vec3(1.0f);                     // https://en.m.wikipedia.org/wiki/Moment_of_inertia Tetrahedron of unit size with unit mass = 0.1331712

    glm::vec3 force = glm::vec3(0);
    glm::vec3 torque = glm::vec3(0);                            // https://en.m.wikipedia.org/wiki/Torque

    float gravity = -9.81f;
    float staticFriction = 0.40f;                               // https://en.m.wikipedia.org/wiki/Friction
    float dynamicFriction = 0.30f;                              // https://en.m.wikipedia.org/wiki/Friction
    float bounciness = 0.5f;                                    // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution
    
    RigidBody() = default;
    RigidBody(Mesh* mesh = NULL, Collider* collider = NULL);
    ~RigidBody() = default;

    void applyRotation(glm::vec3 rot, float scale = 1.0);
    void applyCorrection(glm::vec3 corr, glm::vec3 pos = glm::vec3(0), bool velocityLevel = false);

    void integrate(const float &deltaTime);
    void update(const double &deltaTime);
    
    float getInverseMass(glm::vec3& normal, glm::vec3 pos = glm::vec3(0));
    glm::vec3 getVelocityAt(glm::vec3& pos);

    void makeStatic();

    void applyForce(const glm::vec3& force, const glm::vec3& position = glm::vec3(0));
    // void applyForceL(RigidBodyForce force);
    // void applyForceL(glm::vec3 forceW, glm::vec3 positionL = glm::vec3(0.0f));
    // void applyForceW(RigidBodyForce force);

    void updateGeometry();

private:
    void updateCollider();
};
