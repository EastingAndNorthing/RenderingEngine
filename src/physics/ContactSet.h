#pragma once

#include "common.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsSolver.h" // Remove

struct ContactSet {
    
    RigidBody* A = NULL;
    RigidBody* B = NULL;

    glm::vec3 P = glm::vec3(0.0f);      // Contact position in world space
    glm::vec3 N = glm::vec3(0.0f);      // Normal of collision plane

    float signedDistance = 0.0f;        // Signed distance
    
    float compliance = 5000.0f;

    float lambda = 0.0f;
    glm::vec3 lambdaN = glm::vec3(0.0f);
    glm::vec3 lambdaT = glm::vec3(0.0f);

    ContactSet(
        RigidBody* A, 
        RigidBody* B, 
        const glm::vec3& touchPoint,
        const glm::vec3& collisionPlane,
        const float& signedDistance,
        const float compliance = 0.0f
    );
    ~ContactSet();

};
