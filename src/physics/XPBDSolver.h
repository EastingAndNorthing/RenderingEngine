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

    void update();

    std::vector<ContactSet> getCollisionPairs();


}
