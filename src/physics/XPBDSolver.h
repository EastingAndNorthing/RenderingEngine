#pragma once

#include "../common.h"
#include "core/Time.h"
#include "primitives/Polygon.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsHandler.h"
#include "physics/ContactSet.h"
#include "math/Quaternion.h"
#include "math/CoordinateSystem.h"

#include "core/Renderer.h"

namespace XPBDSolver {

    const int numSubSteps = 2;
    const int numConstraintIterations = 1;

    const float AABBMargin = 2.0f;                  // Chapter 3.5

    void update(const std::vector<RigidBody*> rigidBodies, const double dt);

    std::vector<ContactSet> getCollisionPairs(const std::vector<RigidBody*> rigidBodies, const double& dt);

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
