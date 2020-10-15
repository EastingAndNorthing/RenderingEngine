#include "physics/PhysicsSolver.h"

glm::vec3 PhysicsSolver::constructPlaneFromPolygon(const Polygon& polygon) {
    const glm::vec3& A = polygon.vertices[0].position;
    const glm::vec3& B = polygon.vertices[1].position;
    const glm::vec3& C = polygon.vertices[2].position;

    return glm::normalize(glm::cross(B - A, C - A));
}

float PhysicsSolver::getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal) {
    return glm::dot(planeNormal, (pointPos - planePos));
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

        if(A->isDynamic) A->velocity = velocities.first;
        if(B->isDynamic) B->velocity = velocities.second;

        A->position += PC->normal * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
    }
}

void PhysicsSolver::collide_SPHERE_SPHERE(RigidBody* A, RigidBody* B) {

    auto SCA = static_cast<SphereCollider*>(A->collider);
    auto SCB = static_cast<SphereCollider*>(B->collider);

    float signedDistance = glm::distance(A->position, B->position) - SCA->radius - SCB->radius;

    if(signedDistance <= 0.0f) {
        glm::vec3 N = (A->position - B->position);
        auto velocities = PhysicsSolver::elasticParticleCollision(A->velocity, B->velocity, A->mass, B->mass, N, A->isDynamic, B->isDynamic, A->bounciness * B->bounciness);
        
        A->velocity = velocities.first;
        B->velocity = velocities.second;

        A->position += N * (abs(signedDistance) + PhysicsSolver::afterCollisionDistance);
    }

}

void PhysicsSolver::collide_SPHERE_MESH(RigidBody* A, RigidBody* B) {}

void PhysicsSolver::collide_MESH_PLANE(RigidBody* A, RigidBody* B) {
    // https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
    // https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT
    // https://www.youtube.com/watch?v=EzD7FY62A20

    auto MC = static_cast<MeshCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);
    
    for(int i = 0; i < MC->uniqueIndices.size(); i++) {
        const Vertex& v = MC->vertices[MC->uniqueIndices[i]];

        glm::vec3 v_worldPos = (A->rotation * v.position) + A->position; // (?)
        const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(v_worldPos, B->position, PC->normal);

        if(signedDistance <= 0.0f) {

            // https://en.m.wikipedia.org/wiki/Collision_response#Impulse-based_reaction_model
            // https://www.youtube.com/watch?v=SHinxAhv1ZE

            // glm::vec3 localCOGDistanceVector = glm::abs(A->position - v_worldPos);
            // glm::vec3 localCOGDistanceVector = glm::abs(glm::vec3(0.0f) - v.position);
            // Log(localCOGDistanceVector);
            // A->position += PC->normal * ( glm::dot(PC->normal, -localCOGDistanceVector) + (abs(signedDistance)) );

            // A->velocity.y *= 0.4f;
            // A->velocity -= glm::normalize(localCOGDistanceVector) * glm::dot(A->velocity, localCOGDistanceVector);

            // A->applyForce(glm::vec3(A->mass * A->gravity) * PC->normal, v.position); // Normal force

            break;
        }
    }
}
