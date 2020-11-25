#include "physics/PhysicsSolver.h"
#include "core/Time.h"
#include "core/Renderer.h"

// Should be part of an intersection detection class
glm::vec3 PhysicsSolver::constructPlaneFromPolygon(const Polygon& polygon) {
    const glm::vec3& A = polygon.vertices[0].position;
    const glm::vec3& B = polygon.vertices[1].position;
    const glm::vec3& C = polygon.vertices[2].position;
    return glm::normalize(glm::cross(B - A, C - A));
}

float PhysicsSolver::getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal) {
    return glm::dot(planeNormal, (pointPos - planePos));
}

glm::vec3 PhysicsSolver::resolvePenetration(const float& signedDistance, const glm::vec3& N, const float& additionalOffset) {
    return N * (-signedDistance + PhysicsSolver::seperationDistance + additionalOffset);
}

glm::vec3 PhysicsSolver::getLocalPointVelocity(const glm::vec3& pointPosL, const glm::vec3& angularVelocityW) {
    return glm::cross(angularVelocityW, pointPosL);
}

glm::vec3 PhysicsSolver::getWorldPointVelocity(const glm::vec3& pointPosW, const glm::vec3& originPosW, const glm::vec3& linearVelocityW, const glm::vec3& angularVelocityW) {
    return linearVelocityW + glm::cross(angularVelocityW, (pointPosW - originPosW));
}

void PhysicsSolver::solveRestingContact(const std::vector<ContactSet>& contacts) {
    // Baraf: "ComputeContactForces()"

    unsigned int ncontacts = contacts.size();

    if(ncontacts > 0)
        return;

    for(int i = 0; i < ncontacts; i++) {
        RigidBody* A = contacts[i].A;
        RigidBody* B = contacts[i].B;
        const glm::vec3& N = contacts[i].N;

        A->applyForceW(glm::dot(A->forces, N)/ncontacts * N, contacts[i].p);
        B->applyForceW(-glm::dot(B->forces, N)/ncontacts * N, contacts[i].p);
    }

    // std::vector<std::vector<float>> amat(ncontacts, std::vector<float>(ncontacts));
    // std::vector<float> bvec(ncontacts);
    // std::vector<float> fvec(ncontacts);

    // // compute_a_matrix(contacts, ncontacts, amat);
    // // for(int i = 0; i < ncontacts; i++)
    // //     for(int j = 0; j < ncontacts; j++)
    // //         a[i,j] = compute_aij(contacts[i], contacts[j]);

    // // Well.......... qp_solve is not defined in the paper.
    // // compute_b_vector(contacts, ncontacts, bvec);
    // // qp_solve(amat, bmat, fvec);

    // for(int i = 0; i < ncontacts; i++) {

    //     glm::vec3 reactionForce = fvec[i] * contacts[i].N;

    //     contacts[i].A->applyForce(reactionForce);
    //     contacts[i].B->applyForce(-reactionForce);

    //     // RigidBody* A = contacts[i].A;
    //     // RigidBody* B = contacts[i].B;

    //     // A->force += f * n;
    //     // A->torque += (contacts[i].p - A->x) * (f*n);

    //     // B->force -= f * n;
    //     // B->torque -= (contacts[i].p - B->x) * (f*n);

    // }
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

    // if(P1Dynamic) new_v1 = v1 - ( velocityRatio1 * 2.0f * glm::dot( v1-v2,  N) / glm::length2( N) *  N ) * energyConservation; 
    // if(P2Dynamic) new_v2 = v2 - ( velocityRatio2 * 2.0f * glm::dot( v2-v1, -N) / glm::length2(-N) * -N ) * energyConservation;
    if(P1Dynamic) new_v1 = glm::reflect(v1-v2, N) * velocityRatio1 * energyConservation;
    if(P2Dynamic) new_v2 = glm::reflect(v2-v1, N) * velocityRatio2 * energyConservation;

    return std::make_pair(new_v1, new_v2);
}

void PhysicsSolver::collide_SPHERE_PLANE(RigidBody* A, RigidBody* B) {

    auto SC = static_cast<SphereCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    float signedDistance = PhysicsSolver::getPointToPlaneDistance(A->position, B->position, PC->normal) - SC->radius;

    if(signedDistance <= 0.0f) {
        auto velocities = PhysicsSolver::elasticParticleCollision(A->velocity, B->velocity, A->mass, B->mass, PC->normal, A->isDynamic, B->isDynamic, A->bounciness * B->bounciness);

        if(A->isDynamic) {
            A->position += PhysicsSolver::resolvePenetration(signedDistance, PC->normal);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position -= PhysicsSolver::resolvePenetration(signedDistance, PC->normal);
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
            A->position += PhysicsSolver::resolvePenetration(signedDistance, N);
            A->velocity = velocities.first;
        }
        if(B->isDynamic) {
            B->position -= PhysicsSolver::resolvePenetration(signedDistance, N);
            B->velocity = velocities.second;
        }
    }

}

void PhysicsSolver::collide_SPHERE_MESH(RigidBody* A, RigidBody* B) {}

void PhysicsSolver::collide_MESH_PLANE(RigidBody* A, RigidBody* B) {

    auto MC = static_cast<MeshCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    const glm::vec3& N = glm::normalize(PC->normal);

    std::vector<ContactSet> contacts;
    
    for(int i = 0; i < MC->uniqueIndices.size(); i++) {
        const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
        
        const glm::vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->rotation, A->position);
        
        const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(contactPointW, B->position, N);

        if(signedDistance < 0.0f) {

            const glm::vec3 totalVelocity1 = (A->isDynamic) ? A->getPointVelocityW(contactPointW) : glm::vec3(0.0f);
            const glm::vec3 totalVelocity2 = (B->isDynamic) ? B->getPointVelocityW(contactPointW) : glm::vec3(0.0f);
            const glm::vec3 relativeVelocity = totalVelocity2 - totalVelocity1;

            const float relativeVelocityToNormal = glm::dot(relativeVelocity, N);

            if(relativeVelocityToNormal > PhysicsSolver::restingContactVelocity) {
                // Impulse based collision reaction (David Baraff: Rigid Body Simulation)
                // https://en.m.wikipedia.org/wiki/Collision_response#Impulse-based_reaction_model
                // https://www.youtube.com/watch?v=SHinxAhv1ZE

                const glm::vec3 r1 = contactPointW - A->position;
                const glm::vec3 r2 = contactPointW - B->position;

                // Bug: World inertia tensor does not seem to be aligned properly, causing bouncing if local/world coordinate systems are not nearly aligned
                // https://physics.stackexchange.com/questions/468941/inertia-tensor-of-rigid-body-in-generalized-coordinate-frame
                // Somehow using the local inertia tensor makes the simulation more stable / predictable 
                const glm::vec3 inertiaInfluenceA = (A->isDynamic) ? glm::cross(glm::cross(r1, N), r1) * A->_inverseInertiaTensor : glm::vec3(0.0f); // Static bodies have 'infinite' inertia, reciprocal is zero
                const glm::vec3 inertiaInfluenceB = (B->isDynamic) ? glm::cross(glm::cross(r2, N), r2) * B->_inverseInertiaTensor : glm::vec3(0.0f);
                
                const float inverseMassA = (A->isDynamic) ? A->_inverseMass : 0.0f; // Static bodies have 'infinite' mass, reciprocal is zero
                const float inverseMassB = (B->isDynamic) ? B->_inverseMass : 0.0f;

                const float combinedCOR = (A->bounciness + B->bounciness) / 2.0f; // Average coefficient of restitution 
                
                const glm::vec3 impulse = N * glm::dot(-(1.0f + combinedCOR) * relativeVelocity, N) / ( inverseMassA + inverseMassB +
                    glm::dot(inertiaInfluenceA + inertiaInfluenceB, N)
                );

                if(A->isDynamic) {
                    A->applyWorldImpulse(-impulse, contactPointW);

                    Renderer &renderer = Renderer::Instance();
                    renderer.debugVector->setPosition(contactPointW);
                    renderer.debugVector->setScale(glm::vec3(glm::length(impulse) * 2.0f));
                    renderer.debugVector->setRotation(Quaternion::createFromTwoVectors(
                        glm::vec3(0.0f, 1.0f, 0.0f),
                        -impulse
                    ));
                }

                if(B->isDynamic) B->applyWorldImpulse(impulse, contactPointW);
                
            } if(relativeVelocityToNormal > 0 && relativeVelocityToNormal <= PhysicsSolver::restingContactVelocity) {

                contacts.push_back(ContactSet(A, B, contactPointW, N)); // Resting contact point to be solved with all other points

            }

            if(A->isDynamic) A->position += PhysicsSolver::resolvePenetration(signedDistance, N);
            if(B->isDynamic) B->position -= PhysicsSolver::resolvePenetration(signedDistance, N);
        }

        // if(contacts.size() >= 4)
        //     break;

    }

    PhysicsSolver::solveRestingContact(contacts);
}

// void PhysicsSolver::collide_MESH_MESH(RigidBody* A, RigidBody* B) {
// https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
// https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT
// https://www.youtube.com/watch?v=EzD7FY62A20
// }
