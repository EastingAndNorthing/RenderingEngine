#pragma once

#include "../common.h"
#include "core/Time.h"
#include "primitives/Polygon.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsHandler.h"
#include "math/Quaternion.h"
#include "math/CoordinateSystem.h"
// #include "physics/ContactSet.h"

#include "core/Renderer.h"

struct ContactSet {
    RigidBody* A = NULL;
    RigidBody* B = NULL;
    glm::vec3 p = glm::vec3(0.0f);
    glm::vec3 n = glm::vec3(0.0f);
    float d = 0.0f;
    glm::vec3 vrel = glm::vec3(0.0f);
    float vn = 0.0f;
    float e = 0.5f;
    float friction = 0.0f;
    float lambdaN = 0.0f;
};

struct CollisionPair {
    RigidBody* A = NULL;
    RigidBody* B = NULL;
};

namespace XPBDSolver {

    const int numSubSteps = 10;
    const int numPosIters = 1;

    void update(const std::vector<RigidBody*> rigidBodies, const double dt);

    std::vector<CollisionPair> getPossibleCollisions(const std::vector<RigidBody*> rigidBodies, const double& dt);
    std::vector<ContactSet> getContacts(const std::vector<CollisionPair>& collisions);

    void solvePositions(const std::vector<ContactSet>& contacts = {}, const double& h = 0.0f);
    void solveVelocities(const std::vector<ContactSet>& contacts = {}, const double& h = 0.0f);

    void applyBodyPairCorrection(
        // RigidBody* body0, 
        // RigidBody* body1, 
        ContactSet& contact,
        glm::vec3& corr, 
        float compliance, 
        float dt, 
        glm::vec3 pos0 = glm::vec3(0.0f), 
        glm::vec3 pos1 = glm::vec3(0.0f), 
        bool velocityLevel = false
    );

}
