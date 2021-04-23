#include "physics/XPBDSolver.h"

void XPBDSolver::update(const std::vector<RigidBody*> rigidBodies, const double dt) {

    // Implementation of XPBD algorithm 2.
    
    const double h = dt / XPBDSolver::numSubSteps;

    auto contacts = XPBDSolver::getCollisionPairs(rigidBodies);

    for(volatile int i; i < XPBDSolver::numSubSteps; i++) {

        for (auto& body: rigidBodies) {
            body->updatePhysics(h);
        }

        for(int i = 0; i < XPBDSolver::numConstraintIterations; i++) {
            XPBDSolver::solvePositions(contacts, h);
        }

        for (auto& body: rigidBodies) {
            body->computeActualState(h);
        }

        XPBDSolver::solveVelocities(contacts, h);

    }

    for(volatile int i; i < XPBDSolver::numSubSteps; i++) {
        for (auto& contact: contacts) {
            contact.lambda = 0.0f;
            contact.lambdaN =glm::vec3(0.0f);
            contact.lambdaT = glm::vec3(0.0f);
        }
    }
}

std::vector<ContactSet> XPBDSolver::getCollisionPairs(const std::vector<RigidBody*> rigidBodies) {

    // @TODO Chunking / octree, prevent checking body pairs multiple times
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    // @TODO Broad phase collision detection using bounding box proximity check, use AABBMargin (k) from chapter 3.5

    std::vector<ContactSet> contacts = {};

    for (auto A: rigidBodies) {
        for (auto B: rigidBodies) {
            
            if ((A == B) || (!A->isDynamic && !B->isDynamic))
                continue;

            switch(A->collider->colliderType) {

                // case ColliderType::Sphere :
                //     switch(otherBody->collider->colliderType) {
                //         case ColliderType::Plane      : PhysicsSolver::collide_SPHERE_PLANE(body, otherBody);   break;
                //         case ColliderType::Sphere     : PhysicsSolver::collide_SPHERE_SPHERE(body, otherBody);  break;
                //         case ColliderType::ConvexMesh : PhysicsSolver::collide_SPHERE_MESH(body, otherBody);    break;
                //         default: break;
                //     }
                // break;

                case ColliderType::ConvexMesh :
                    switch(B->collider->colliderType) {
                        case ColliderType::Plane : {
                        
                            // Move collision detection code somewhere else, only push contactset if found
                            
                            auto MC = static_cast<MeshCollider*>(A->collider);
                            auto PC = static_cast<PlaneCollider*>(B->collider);

                            const glm::vec3& N = glm::normalize(PC->normal); // Maybe assume N to always be normalized for planes.

                            for(int i = 0; i < MC->uniqueIndices.size(); i++) {
                                const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
                                
                                const glm::vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->rotation, A->position);
                                    
                                const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(contactPointW, B->position, N);

                                if(signedDistance < 0.1f) {
                                    Time &time = Time::Instance();
                                    time.setStepMode(true);
                                }

                                if(signedDistance < 0.0f) {
                                    contacts.push_back(ContactSet(A, B, contactPointW, N, signedDistance));
                                }
                            }

                            break;
                        }

                        default: break;
                    }

                break;
            }

        }

    }

    return contacts;
}


void XPBDSolver::solvePositions(const std::vector<ContactSet>& contacts, const double& h) {

    // 3.3.1. Positional Constraints

    for (auto contact: contacts) {

        const glm::vec3 r1 = contact.A->position - contact.P;
        const glm::vec3 r2 = contact.B->position - contact.P;

        // (1, 2) - Generalized inverse masses
        const float w1 = contact.A->_inverseMass + glm::length(glm::cross(r1, contact.N) * contact.A->_inverseInertiaTensorW * glm::cross(r1, contact.N)); 
        const float w2 = contact.B->_inverseMass + glm::length(glm::cross(r2, contact.N) * contact.B->_inverseInertiaTensorW * glm::cross(r2, contact.N));

        // (4, 5) - Lagrange multiplier
        float alpha = contact.compliance/(h*h);
        float dlambda = (-contact.signedDistance - (alpha * contact.lambda)) / (w1 + w2 + alpha);
        contact.lambda += dlambda;
        contact.lambdaN = contact.N * contact.lambda; // ???
        contact.lambdaT = contact.lambda - contact.N * contact.lambdaN; // ???

        // (before 6) - Impulse p = Δλ*n
        glm::vec3 impulse = dlambda * contact.N;

        if(contact.A->isDynamic) {
            contact.A->position += impulse * contact.A->_inverseMass; // (6)
            contact.A->rotation += 0.5f * glm::quat(0, contact.A->_inverseInertiaTensorW * glm::cross(r1, impulse)) * contact.A->rotation; // (8)

            Renderer &renderer = Renderer::Instance();
            renderer.debugVector->setPosition(contact.P);
            renderer.debugVector->setScale(glm::vec3(glm::length(impulse) * 1000.0f));
            renderer.debugVector->setRotation(Quaternion::createFromTwoVectors(
                glm::vec3(0.0f, 1.0f, 0.0f),
                impulse
            ));
        }

        if(contact.B->isDynamic) {
            contact.B->position -= impulse * contact.B->_inverseMass; // (7)
            contact.B->rotation -= 0.5f * glm::quat(0, contact.B->_inverseInertiaTensorW * glm::cross(r2, impulse)) * contact.B->rotation; // (9)
        }
    }
}

void XPBDSolver::solveVelocities(const std::vector<ContactSet>& contacts, const double& h) {
    
    for (auto contact: contacts) {

        // SAME R1 and R2 are calculated twice. Maybe store these in ContactSet?
        const glm::vec3 r1 = contact.A->position - contact.P;
        const glm::vec3 r2 = contact.B->position - contact.P;

        // SAME as generalized mass equation as (2) and (3), might be wrong
        const float w1 = contact.A->_inverseMass + glm::length(glm::cross(r1, contact.N) * contact.A->_inverseInertiaTensorW * glm::cross(r1, contact.N)); 
        const float w2 = contact.B->_inverseMass + glm::length(glm::cross(r2, contact.N) * contact.B->_inverseInertiaTensorW * glm::cross(r2, contact.N));

        // Calculate average friction
        float staticFriction = 0.5 * (contact.A->staticFriction + contact.A->staticFriction);
        float dynamicFriction = 0.5 * (contact.A->dynamicFriction + contact.A->dynamicFriction);
        
        // (29)
        // Looks like a generic point velocity calculation, v + omega*r?
        glm::vec3 v = (contact.A->velocity + glm::cross(contact.A->angularVelocity, r1)) - (contact.B->velocity + glm::cross(contact.B->angularVelocity, r2));
        glm::vec3 vn = contact.N * v;
        glm::vec3 vt = v - contact.N * vn;

        // (30)
        float h_squared = (float) h * (float) h;
        glm::vec3 Fn = contact.lambdaN / h_squared;
        glm::vec3 dv = -vt/glm::abs(vt) * glm::min((float) h * dynamicFriction * Fn, glm::abs(vt));     
        
        // (31, 32)
        // glm::vec3 dv = *contact.A->velocity - contact.B->velocity) * glm::min(staticFriction * (float) h, 1); // (31) ???? static friction?
        // @TODO apply angular dampening (32)
        
        // (34), restitution, normal velocity before PBD velocity update == A->velocity
        // Why not just do dv*e?
        // @TODO set 
        // dv += contact.N * (-vn + glm::max(-e * contact.N * contact.A->velocity, 0));

        // (33), velocity update
        glm::vec3 impulse = dv / (w1+w2); 
        // contact.A->velocity += impulse * contact.A->_inverseMass;
        // contact.B->velocity -= impulse * contact.B->_inverseMass;
        // contact.A->angularVelocity += contact.A->_inverseInertiaTensorW * (r1 * impulse);
        // contact.B->angularVelocity -= contact.B->_inverseInertiaTensorW * (r2 * impulse);

    }
}
