#pragma once

#include "../common.h"
#include "primitives/Polygon.h"
#include "physics/RigidBody.h"

namespace PhysicsSolver {

    const float afterCollisionDistance = 0.0008f;

    glm::vec3 constructPlaneFromPolygon(const Polygon& polygon);

    float getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal);

    glm::vec3 getAfterCollisionPositionOffset(const float& signedDistance, const glm::vec3& N);

    glm::vec3 getPointVelocity(const glm::vec3& pointPos, const glm::vec3& linearVelocity, const glm::vec3& angularVelocity);

    std::pair<glm::vec3, glm::vec3> elasticParticleCollision(
        const glm::vec3& v1,
        const glm::vec3& v2,
        const float& m1,
        const float& m2,
        const glm::vec3& collisionPlane,
        const bool& P1Dynamic = true,
        const bool& P2Dynamic = true,
        const float& energyConservation = 1.0f
    );

    void collide_SPHERE_PLANE(RigidBody* sphere, RigidBody* plane);
    void collide_SPHERE_SPHERE(RigidBody* A, RigidBody* B);
    void collide_SPHERE_MESH(RigidBody* A, RigidBody* B);
    void collide_MESH_PLANE(RigidBody* A, RigidBody* B);
    void collide_MESH_MESH(RigidBody* A, RigidBody* B);
}