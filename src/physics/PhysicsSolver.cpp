#include "physics/PhysicsSolver.h"
#include "core/Time.h"

glm::vec3 PhysicsSolver::constructPlaneFromPolygon(const Polygon& polygon) {
    const glm::vec3& A = polygon.vertices[0].position;
    const glm::vec3& B = polygon.vertices[1].position;
    const glm::vec3& C = polygon.vertices[2].position;
    return glm::normalize(glm::cross(B - A, C - A));
}

float PhysicsSolver::getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal) {
    return glm::dot(planeNormal, (pointPos - planePos));
}

glm::vec3 PhysicsSolver::getPointVelocity(const glm::vec3& pointPos, const glm::vec3& linearVelocity, const glm::vec3& angularVelocity) {
    // glm::vec3 radius = glm::vec3(0.0f) - pointPos;
    glm::vec3 radius = pointPos;
    return glm::vec3(linearVelocity + glm::cross(angularVelocity, radius));
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

    glm::vec3 new_v1 = v1;
    glm::vec3 new_v2 = v2;
    glm::vec3 N = glm::normalize(collisionPlane);

    float totalMass = m1 + m2; 
    float velocityRatio1 = (P2Dynamic) ? m2/totalMass : 1.0f;
    float velocityRatio2 = (P1Dynamic) ? m1/totalMass : 1.0f;
    
    if(P1Dynamic) new_v1 = v1 - ( velocityRatio1 * 2.0f * glm::dot( v1-v2,  N) / glm::length2( N) *  N ) * energyConservation; 
    if(P2Dynamic) new_v2 = v2 - ( velocityRatio2 * 2.0f * glm::dot( v2-v1, -N) / glm::length2(-N) * -N ) * energyConservation;

    return std::make_pair(new_v1, new_v2);
}

void PhysicsSolver::collide_SPHERE_PLANE(RigidBody* A, RigidBody* B) {

    auto SC = static_cast<SphereCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    float signedDistance = PhysicsSolver::getPointToPlaneDistance(A->position, B->position, PC->normal) - SC->radius;

    if(signedDistance <= 0.0f) {
        auto velocities = PhysicsSolver::elasticParticleCollision(A->velocity, B->velocity, A->mass, B->mass, PC->normal, A->isDynamic, B->isDynamic, A->bounciness * B->bounciness);

        if(A->isDynamic) {
            A->position += PC->normal * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position += PC->normal * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
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
            A->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
            B->velocity = velocities.second;
        }
    }

}

void PhysicsSolver::collide_SPHERE_MESH(RigidBody* A, RigidBody* B) {}

void PhysicsSolver::collide_MESH_PLANE(RigidBody* A, RigidBody* B) {

    auto MC = static_cast<MeshCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);
    
    for(int i = 0; i < MC->uniqueIndices.size(); i++) {
        const Vertex& v = MC->vertices[MC->uniqueIndices[i]];

        const glm::vec3& N = PC->normal;

        glm::vec3 point_worldPos = (A->rotation * v.position) + A->position; // (?)
        const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(point_worldPos, B->position, N);

        if(signedDistance <= 0.0f) {
            // https://en.m.wikipedia.org/wiki/Collision_response#Impulse-based_reaction_model
            // https://www.youtube.com/watch?v=SHinxAhv1ZE

            // Time &time = Time::Instance();
            // glm::vec3 impulse = v_totalVelocity * (float) time.dt;
            
            glm::vec3 r1 = v.position;
            glm::vec3 r2 = (point_worldPos - B->position); // @TODO check local point position for B

            glm::vec3 totalVelocity1 = getPointVelocity(r1, A->velocity, A->angularVelocity);
            glm::vec3 totalVelocity2 = getPointVelocity(r2, B->velocity, B->angularVelocity);

            // auto newVelocities = PhysicsSolver::elasticParticleCollision(totalVelocity1, totalVelocity2, A->mass, B->mass, N, A->isDynamic, B->isDynamic, 0.5f);

            glm::vec3 dv;
            float impulse; 

            // if(A->isDynamic) {
            //     dv = newVelocities.first - totalVelocity1;
            //     // impulse = glm::length(N * (dv * A->mass));
            //     impulse = N * (dv * A->mass);
            // } else {
            //     dv = newVelocities.second - totalVelocity2;
            //     // impulse = glm::length(N * (dv * B->mass));
            //     impulse = N * (dv * B->mass);
            // }

            // if(A->isDynamic) {
            //     A->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance + 0.0f);
            //     A->velocity = newVelocities.first;
            //     A->angularVelocity -= impulse*0.05f * glm::cross(r1, N); // Add I
            // }

            // if(B->isDynamic) {
            //     B->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance + 0.0f);
            //     B->velocity = newVelocities.second;
            //     B->angularVelocity += impulse*0.05f * glm::cross(r2, N); // Add I
            // }

            // impulse = glm::dot(-(1.0f + A->bounciness) * (totalVelocity1-totalVelocity2), N) / (1/A->mass + 1/B->mass + ((1/0.1f) * glm::cross(glm::cross(r1, N), r1) + (1/0.1f) * glm::cross(glm::cross(r2, N), r2)) * N);
            float combinedCOR = 1.0f; // A->bounciness + B->bounciness;
            impulse = glm::dot(
                (-(1.0f + combinedCOR) * (totalVelocity1-totalVelocity2)), N
            ) / (
                1/A->mass + 1/B->mass + glm::dot(
                    1/0.1f * glm::cross(
                        glm::cross(r1, N), r1
                    ) + 1/0.1f * glm::cross(
                        glm::cross(r2, N), r2
                    ), N
                )
            );
            
            if(A->isDynamic) {
                A->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance + 0.0f);
                A->velocity += impulse/A->mass * N;
            }

            if(B->isDynamic) {
                B->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance + 0.0f);
                B->velocity += impulse/B->mass * N;
            }

            break;
        }

        // if(i > 5) 
        //     break;
    }
}

// https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
// https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT
// https://www.youtube.com/watch?v=EzD7FY62A20
