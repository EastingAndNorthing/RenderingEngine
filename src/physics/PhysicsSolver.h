#pragma once

#include "../common.h"
#include "primitives/Polygon.h"
#include "physics/RigidBody.h"

namespace PhysicsSolver {

    const float seperationDistance = 0.0005f;

    glm::vec3 constructPlaneFromPolygon(const Polygon& polygon);

    float getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal);

    glm::vec3 resolvePenetration(const float& signedDistance, const glm::vec3& N, const float& additionalOffset = 0.0f);

    glm::vec3 getLocalPointVelocity(const glm::vec3& pointPosL, const glm::vec3& angularVelocityW);
    glm::vec3 getWorldPointVelocity(const glm::vec3& pointPosW, const glm::vec3& originPosW, const glm::vec3& linearVelocityW, const glm::vec3& angularVelocityW);

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