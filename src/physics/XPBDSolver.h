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

    const int numSubSteps = 20;
    const int numConstraintIterations = 1;

    const float AABBMargin = 2.0f;                  // Chapter 3.5

    void update(const std::vector<RigidBody*> rigidBodies, const double dt);

    std::vector<ContactSet> getCollisionPairs(const std::vector<RigidBody*> rigidBodies);

    void solvePositions(const std::vector<ContactSet>& contacts = {}, const double& h = 0.0f);
    void solveVelocities(const std::vector<ContactSet>& contacts = {}, const double& h = 0.0f);

}
