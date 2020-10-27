#pragma once

#include "common.h"
#include "physics/RigidBody.h"

struct ContactSet {
    
    RigidBody* A;
    RigidBody* B;

    glm::vec3 p;  // Contact position in world space
    glm::vec3 N;  // Normal of collision plane
    glm::vec3 eA; // Edge direction of A
    glm::vec3 eB; // Edge direction of B

    bool vf; // True if point p and plane N are in contact

    ContactSet(
        RigidBody* A, 
        RigidBody* B, 
        const glm::vec3& touchPoint,
        const glm::vec3& collisionPlane,
        const glm::vec3& edgeA = glm::vec3(0),
        const glm::vec3& edgeB = glm::vec3(0),
        bool hasContact = true
    );
    ~ContactSet();

};
