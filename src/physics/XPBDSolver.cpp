#include "physics/XPBDSolver.h"

void XPBDSolver::update(const std::vector<RigidBody*> rigidBodies, const double dt) {

    // Implementation of XPBD algorithm 2
    
    const double h = dt / XPBDSolver::numSubSteps;

    auto collisions = XPBDSolver::getPossibleCollisions(rigidBodies, dt); // AABBs

    for(volatile int i = 0; i < XPBDSolver::numSubSteps; i++) {

        auto contacts = XPBDSolver::getContacts(collisions); // Actual collision points

        for (auto& body: rigidBodies) {
            body->integrate(h);
        }

        for(int i = 0; i < XPBDSolver::numPosIters; i++) {
            XPBDSolver::solvePositions(contacts, h);
        }

        for (auto& body: rigidBodies) {
            body->update(h);
        }

        XPBDSolver::solveVelocities(contacts, h);

    }

    for (auto& body: rigidBodies) {
        body->force = glm::vec3(0.0f);
        body->torque = glm::vec3(0.0f);
        body->updateGeometry();
    }
}

std::vector<CollisionPair> XPBDSolver::getPossibleCollisions(const std::vector<RigidBody*>& rigidBodies, const double& dt) {

    // @TODO Chunking / octree, prevent checking body pairs multiple times
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    // @TODO Broad phase collision detection using bounding box proximity check

    std::vector<CollisionPair> contacts = {};

    for (auto A: rigidBodies) {
        for (auto B: rigidBodies) {
            
            if ((A == B) || (!A->isDynamic && !B->isDynamic))
                continue;

            // k*dt*vbody (3.5)
            const float collisionMargin = 2.0f * (float) dt * glm::length(A->vel - B->vel);

            switch(A->collider->colliderType) {
                case ColliderType::ConvexMesh :
                    switch(B->collider->colliderType) {
                        case ColliderType::Plane : {
                            
                            auto MC = static_cast<MeshCollider*>(A->collider);
                            auto PC = static_cast<PlaneCollider*>(B->collider);

                            const glm::vec3& N = glm::normalize(PC->normal); // Maybe assume N to always be normalized for plane colliders

                            // This should be a simple AABB check instead of actual loop over all vertices
                            for(int i = 0; i < MC->uniqueIndices.size(); i++) {
                                const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
                                glm::vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);
                                    
                                const float signedDistance = glm::dot(N, (contactPointW - B->pose.p)); // Simple point-to-plane dist

                                if(signedDistance < collisionMargin) {
                                    const float e = 0.5f * (A->bounciness + B->bounciness);
                                    const float friction = 0.5f * (A->staticFriction + B->staticFriction);
                                    contacts.push_back({ A, B, e, friction });
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

std::vector<ContactSet*> XPBDSolver::getContacts(const std::vector<CollisionPair>& collisions) {
    
    std::vector<ContactSet*> contacts = {};

    for (auto collision: collisions) {
            
        RigidBody* A = collision.A;
        RigidBody* B = collision.B;

        if ((A == B) || (!A->isDynamic && !B->isDynamic))
            continue;

        switch(A->collider->colliderType) {
            case ColliderType::ConvexMesh :
                switch(B->collider->colliderType) {
                    case ColliderType::Plane : {
                        
                        auto MC = static_cast<MeshCollider*>(A->collider);
                        auto PC = static_cast<PlaneCollider*>(B->collider);

                        const glm::vec3& N = PC->normal;

                        float deepestPenetration = 0.0f;
                        ContactSet* contactSet = new ContactSet(A, B);

                        // @TODO check if vertex is actually inside plane size :)
                        for(int i = 0; i < MC->uniqueIndices.size(); i++) {
                            const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
                            glm::vec3 point = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);
                                
                            const float signedDistance = glm::dot(N, (point - B->pose.p));

                            if(signedDistance < deepestPenetration) {
                                deepestPenetration = signedDistance;
                                
                                contactSet->p = point;
                                contactSet->d = signedDistance;
                                contactSet->n = N;
                            }
                        }

                        if(deepestPenetration < 0.0f) {
                            glm::vec3 vrel = A->getVelocityAt(contactSet->p) - B->getVelocityAt(contactSet->p);
                            contactSet->vn = glm::dot(contactSet->n, vrel);

                            contactSet->e = collision.e;
                            contactSet->friction = collision.friction;

                            contacts.push_back(contactSet);
                        }

                        break;
                    }
                    default: break;
                }
            break;
        }
    }

    return contacts;
}

void XPBDSolver::solvePositions(const std::vector<ContactSet*>& contacts, const double& h) {

    for (auto contact: contacts) {
        
        if(contact->d >= 0.0f)
            continue; // Contact has been solved

        // contact->A->hasCollided = true;
        // contact->B->hasCollided = true;

        glm::vec3 posCorr = -contact->d * contact->n;

        XPBDSolver::applyBodyPairCorrection(
            // contact.A,
            // contact.B,
            contact,
            posCorr,
            0.0f,
            (float) h,
            contact->p,
            contact->p,
            false
        );
        
        // // @TODO eq. 27, 28
        // const glm::vec3 p1prev = contact.A->prevPose.p + contact.A->prevPose.q * r1;
        // const glm::vec3 p2prev = contact.B->prevPose.p + contact.B->prevPose.q * r2;
    }
}

void XPBDSolver::solveVelocities(const std::vector<ContactSet*>& contacts, const double& h) {
    
    for (auto contact: contacts) {

        glm::vec3 dv = glm::vec3(0.0f);
        
        // (29) Relative velocity
        glm::vec3 v = contact->A->getVelocityAt(contact->p) - contact->B->getVelocityAt(contact->p);
        float vn = glm::dot(contact->n, v);
        glm::vec3 vt = v - (contact->n * vn);
        float vt_length = glm::length(vt);

        // (30) Friction
        if(vt_length > 0.001f) {
            float h_squared = (float) h * (float) h;
            float Fn = -contact->lambdaN / h_squared;
            dv -= glm::normalize(vt) * std::min((float) h * contact->friction * Fn, vt_length);     
        }
        
        // (31, 32) @TODO dampening

        // (34), restitution
        // if(vn < 0.01f) {
            float e = (std::abs(vn) > (1.0f * 9.81f * h)) ? contact->e : 0.0f;
            const float vn_reflected = std::max(-e * contact->vn, 0.0f);
            // dv += contact->n * (-vn + std::max(-e * contact->vn, 0.0f));
            dv += contact->n * (vn_reflected - vn); // Remove velocity added by update step (only meaningful if no collisions have occured)
        // }

        XPBDSolver::applyBodyPairCorrection(
            // contact.A,
            // contact.B,
            contact,
            dv,
            0.0f,
            (float) h,
            contact->p,
            contact->p,
            true
        );

        // contact->A->hasCollided = false;
        // contact->B->hasCollided = false;

    }
}

void XPBDSolver::applyBodyPairCorrection(
    // RigidBody* body0, 
    // RigidBody* body1, 
    ContactSet* contact,
    glm::vec3& corr, 
    float compliance, 
    float dt, 
    glm::vec3 pos0,
    glm::vec3 pos1,
    bool velocityLevel
) {

    RigidBody* body0 = contact->A;
    RigidBody* body1 = contact->B;

    float C = glm::length(corr);
    
    if ( C < 0.00001f)
        return;

    glm::vec3 n = glm::normalize(corr);

    float w0 = body0->isDynamic ? body0->getInverseMass(n, pos0) : 0.0f;
    float w1 = body1->isDynamic ? body1->getInverseMass(n, pos1) : 0.0f;

    float w = w0 + w1;
    if (w == 0.0f)
        return;

    float dlambda = -C / (w + compliance / dt / dt);
    
    if(!velocityLevel)
        contact->lambdaN += dlambda; // Assuming n is in the normal direction of the collision plane

    n = n * -dlambda;

    body0->applyCorrection(n, pos0, velocityLevel);
    body1->applyCorrection(-n, pos1, velocityLevel);
}
