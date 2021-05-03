#pragma once

#include "common.h"
#include "physics/RigidBody.h"

struct ContactSet {
    
    RigidBody* A = NULL;
    RigidBody* B = NULL;

    glm::vec3 P = glm::vec3(0.0f);      // Contact position in world space
    glm::vec3 N = glm::vec3(0.0f);      // Normal of collision plane

    float vn = 0.0f;                    // Normal velocity
    float signedDistance = 0.0f;        // Signed distance
    float d = 0.0f;
    
    float lambdaN = 0.0f;
    float lambdaT = 0.0f;

    ContactSet(
        RigidBody* A, 
        RigidBody* B, 
        const glm::vec3& touchPoint,
        const glm::vec3& collisionPlane,
        const float& signedDistance,
        const float& vn = 0.0f
    );
    ~ContactSet();

};
