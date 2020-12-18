#pragma once

#include "common.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsSolver.h" // Remove

struct ContactSet {
    
    RigidBody* A = NULL;
    RigidBody* B = NULL;

    glm::vec3 p = glm::vec3(0.0f);  // Contact position in world space
    glm::vec3 N = glm::vec3(0.0f);  // Normal of collision plane

    float d = 0.0f;

    ContactSet(
        RigidBody* A, 
        RigidBody* B, 
        const glm::vec3& touchPoint,
        const glm::vec3& collisionPlane,
        const float& signedDistance
    );
    ~ContactSet();

};
