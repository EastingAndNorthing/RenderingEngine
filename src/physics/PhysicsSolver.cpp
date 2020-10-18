#include <algorithm>
#include "core/Time.h"
#include "core/Renderer.h"
#include "physics/PhysicsSolver.h"
#include "math/Quaternion.h"
#include "math/CoordinateSystem.h"

glm::vec3 PhysicsSolver::constructPlaneFromPolygon(const Polygon& polygon) {
    const glm::vec3& A = polygon.vertices[0].position;
    const glm::vec3& B = polygon.vertices[1].position;
    const glm::vec3& C = polygon.vertices[2].position;
    return glm::normalize(glm::cross(B - A, C - A));
}

float PhysicsSolver::getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal) {
    return glm::dot(planeNormal, (pointPos - planePos));
}

glm::vec3 PhysicsSolver::getAfterCollisionPositionOffset(const float& signedDistance, const glm::vec3& N, const float& additionalOffset) {
    return N * (-signedDistance + PhysicsSolver::afterCollisionDistance + additionalOffset);
}

glm::vec3 PhysicsSolver::getLocalPointVelocity(const glm::vec3& pointPosL, const glm::vec3& angularVelocityW) {
    return glm::cross(angularVelocityW, pointPosL);
}

glm::vec3 PhysicsSolver::getWorldPointVelocity(const glm::vec3& pointPosW, const glm::vec3& originPosW, const glm::vec3& linearVelocityW, const glm::vec3& angularVelocityW) {
    return linearVelocityW + glm::cross(angularVelocityW, (pointPosW - originPosW));
}

std::pair<glm::vec3, glm::vec3> PhysicsSolver::elasticParticleCollision(
    const glm::vec3& v1,
    const glm::vec3& v2,
    const float& m1,
    const float& m2,
    const glm::vec3& collisionPlane,
    const bool& P1Dynamic,
    const bool& P2Dynamic,
    const float& energyConservation
) {
    // https://en.m.wikipedia.org/wiki/Elastic_collision
    // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution
    // https://en.m.wikipedia.org/wiki/Reflection_(mathematics)

    glm::vec3 new_v1 = v1;
    glm::vec3 new_v2 = v2;
    glm::vec3 N = glm::normalize(collisionPlane);

    float totalMass = m1 + m2; 
    float velocityRatio1 = (P2Dynamic) ? m2/totalMass : 1.0f;
    float velocityRatio2 = (P1Dynamic) ? m1/totalMass : 1.0f;

    if(P1Dynamic) new_v1 = glm::reflect(v1-v2, N) * velocityRatio1 * energyConservation;
    if(P2Dynamic) new_v2 = glm::reflect(v2-v1, N) * velocityRatio2 * energyConservation;
    // if(P1Dynamic) new_v1 = v1 - ( velocityRatio1 * 2.0f * glm::dot( v1-v2,  N) / glm::length2( N) *  N ) * energyConservation; 
    // if(P2Dynamic) new_v2 = v2 - ( velocityRatio2 * 2.0f * glm::dot( v2-v1, -N) / glm::length2(-N) * -N ) * energyConservation;

    return std::make_pair(new_v1, new_v2);
}

void PhysicsSolver::collide_SPHERE_PLANE(RigidBody* A, RigidBody* B) {

    auto SC = static_cast<SphereCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    float signedDistance = PhysicsSolver::getPointToPlaneDistance(A->position, B->position, PC->normal) - SC->radius;

    if(signedDistance <= 0.0f) {
        auto velocities = PhysicsSolver::elasticParticleCollision(A->velocity, B->velocity, A->mass, B->mass, PC->normal, A->isDynamic, B->isDynamic, A->bounciness * B->bounciness);

        if(A->isDynamic) {
            A->position += PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, PC->normal);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position -= PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, PC->normal);
            B->velocity = velocities.second;
        }
    }
}

void PhysicsSolver::collide_SPHERE_SPHERE(RigidBody* A, RigidBody* B) {

    auto SCA = static_cast<SphereCollider*>(A->collider);
    auto SCB = static_cast<SphereCollider*>(B->collider);

    float signedDistance = glm::distance(A->position, B->position) - SCA->radius - SCB->radius;

    if(signedDistance <= 0.0f) {
        glm::vec3 N = (A->position - B->position);
        auto velocities = PhysicsSolver::elasticParticleCollision(A->velocity, B->velocity, A->mass, B->mass, N, A->isDynamic, B->isDynamic, A->bounciness * B->bounciness);
        
        if(A->isDynamic) {
            A->position += PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, N);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position -= PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, N);
            B->velocity = velocities.second;
        }
    }

}

void PhysicsSolver::collide_SPHERE_MESH(RigidBody* A, RigidBody* B) {}

void PhysicsSolver::collide_MESH_PLANE(RigidBody* A, RigidBody* B) {

    auto MC = static_cast<MeshCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    const glm::vec3& N = glm::normalize(PC->normal);

    unsigned int touchPoints = 0;
    
    for(int i = 0; i < MC->uniqueIndices.size(); i++) {
        const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
        
        const glm::vec3 point_worldPos = CoordinateSystem::localToWorld(v.position, A->rotation, A->position);
        
        const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(point_worldPos, B->position, N);

        if(signedDistance < 0.0f) {
            // https://en.m.wikipedia.org/wiki/Collision_response#Impulse-based_reaction_model
            // https://www.youtube.com/watch?v=SHinxAhv1ZE

            touchPoints++;

            const glm::vec3 r1 = point_worldPos - A->position;
            const glm::vec3 r2 = point_worldPos - B->position;
            
            // Must be a way to optimize getWorldPointVelocity. Now we're re-calculating r1 -> A, which is basically v.position (+ rotation)
            const glm::vec3 totalVelocity1 = (A->isDynamic) ? getWorldPointVelocity(r1, A->position, A->velocity, B->angularVelocityW) : glm::vec3(0.0f);
            const glm::vec3 totalVelocity2 = (B->isDynamic) ? getWorldPointVelocity(r2, A->position, B->velocity, B->angularVelocityW) : glm::vec3(0.0f);
            const glm::vec3 relativeVelocity = totalVelocity2 - totalVelocity1; // Relative to normal?

            const float combinedCOR = 1.0f + (A->bounciness * B->bounciness); // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution
            // const glm::vec3 inertiaInfluenceA = glm::cross(glm::cross(r1, N), r1) * A->inverseInertiaTensorW;
            // const glm::vec3 inertiaInfluenceB = glm::cross(glm::cross(r2, N), r2) * B->inverseInertiaTensorW;
            // float impulse = glm::dot(-combinedCOR * (totalVelocity2-totalVelocity1), N) / ( A->inverseMass + B->inverseMass +
            //     glm::dot(inertiaInfluenceA + inertiaInfluenceB, N)
            // );

            const glm::vec3 impulse = N * glm::dot(-combinedCOR * relativeVelocity, N) / ( A->_inverseMass + B->_inverseMass +
            glm::dot(
                A->_inverseInertiaTensorW * glm::cross(
                    glm::cross(r1, N), r1
                ) + B->_inverseInertiaTensorW * glm::cross(
                    glm::cross(r2, N), r2
                ), N
            ));

            if(A->isDynamic) {
                A->applyWorldImpulse(-impulse, point_worldPos);

                Renderer &renderer = Renderer::Instance();
                renderer.debugVector->setPosition(point_worldPos);
                renderer.debugVector->setScale(glm::vec3(2.0f, glm::length(impulse) * 2.5f, 2.0f));
                renderer.debugVector->setRotation(Quaternion::createFromTwoVectors(
                    glm::vec3(0.0f, 1.0f, 0.0f),
                    impulse
                ));
            }

            if(B->isDynamic) B->applyWorldImpulse(impulse, point_worldPos);

            if(!B->isDynamic) A->position += PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, N, 0.0f);
            if(!A->isDynamic) B->position -= PhysicsSolver::getAfterCollisionPositionOffset(signedDistance, N, 0.0f);
        }

        if(touchPoints >= 3) {
            break;
        }

    }
}

// void PhysicsSolver::collide_MESH_MESH(RigidBody* A, RigidBody* B) {
// https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
// https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT
// https://www.youtube.com/watch?v=EzD7FY62A20
// }
