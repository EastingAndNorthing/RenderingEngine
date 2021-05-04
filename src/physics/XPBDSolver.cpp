#include "physics/XPBDSolver.h"

void XPBDSolver::update(const std::vector<RigidBody*> rigidBodies, const double dt) {

    // Implementation of XPBD algorithm 2.
    
    // let dt = timeStep / numSubsteps;
    const double h = dt / XPBDSolver::numSubSteps;

    auto collisions = XPBDSolver::getPossibleCollisions(rigidBodies, dt); // AABBs

    for(volatile int i = 0; i < XPBDSolver::numSubSteps; i++) {

        auto contacts = XPBDSolver::getContacts(collisions); // Actual collision points

        for(volatile int j = 0; j < rigidBodies.size(); j++) {
            rigidBodies[j]->integrate(h);
        }

        XPBDSolver::solvePositions(contacts, h);

        for(volatile int k = 0; k < rigidBodies.size(); k++) {
            rigidBodies[k]->update(h);
        }

        // for (auto& body: rigidBodies) {
        //     body->integrate(h);
        // }

        // for(int i = 0; i < XPBDSolver::numConstraintIterations; i++) {
        //     XPBDSolver::solvePositions(contacts, h);
        // }

        // for (auto& body: rigidBodies) {
        //     body->update(h);
        // }

        XPBDSolver::solveVelocities(contacts, h);

    }
}

std::vector<CollisionPair> XPBDSolver::getPossibleCollisions(const std::vector<RigidBody*> rigidBodies, const double& dt) {

    // @TODO Chunking / octree, prevent checking body pairs multiple times
    // https://github.com/mwarning/SimpleOctree/tree/master/src

    // @TODO Broad phase collision detection using bounding box proximity check
    // k*dt*vbody (3.5)
    // const float collisionMargin = 2.0f * (float) dt * glm::length(A->vel - B->vel);
                        
    // Use AABBs to do initial contact check. Then loop over all verticies in solvePositions() again

    std::vector<CollisionPair> contacts = {};

    for (auto A: rigidBodies) {
        for (auto B: rigidBodies) {
            
            if ((A == B) || (!A->isDynamic && !B->isDynamic))
                continue;

            switch(A->collider->colliderType) {
                case ColliderType::ConvexMesh :
                    switch(B->collider->colliderType) {
                        case ColliderType::Plane : {
                            
                            auto MC = static_cast<MeshCollider*>(A->collider);
                            auto PC = static_cast<PlaneCollider*>(B->collider);

                            const glm::vec3& N = glm::normalize(PC->normal); // Maybe assume N to always be normalized for planes.

                            // This should be a simple AABB check instead of actual loop over all vertices
                            for(int i = 0; i < MC->uniqueIndices.size(); i++) {
                                const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
                                glm::vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);
                                    
                                // const float& signedDistance = PhysicsSolver::getPointToPlaneDistance(contactPointW, B->pose.p, N);
                                const float signedDistance = glm::dot(N, (contactPointW - B->pose.p));

                                if(signedDistance < 0.1f)
                                    contacts.push_back({A, B}); // contacts.push_back(ContactSet(A, B, contactPointW, N, signedDistance, vn));
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

std::vector<ContactSet> XPBDSolver::getContacts(const std::vector<CollisionPair>& collisions) {
    
    std::vector<ContactSet> contacts = {};

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

                        const glm::vec3& N = glm::normalize(PC->normal); // Maybe assume N to always be normalized for planes.

                        float deepestPenetration = 0.0f;
                        ContactSet contactSet = { A, B, glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f };

                        for(int i = 0; i < MC->uniqueIndices.size(); i++) {
                            const Vertex& v = MC->vertices[MC->uniqueIndices[i]];
                            glm::vec3 contactPointW = CoordinateSystem::localToWorld(v.position, A->pose.q, A->pose.p);
                                
                            const float signedDistance = glm::dot(N, (contactPointW - B->pose.p));

                            // if(signedDistance < 0.1f) {
                            //     Time &time = Time::Instance();
                            //     time.setStepMode(true);
                            // }

                            if(signedDistance < deepestPenetration) {
                                deepestPenetration = signedDistance;

                                glm::vec3 vrel = A->getVelocityAt(contactPointW) - B->getVelocityAt(contactPointW);
                                float vn = glm::dot(N, vrel);
                                
                                contactSet.p = contactPointW;
                                contactSet.n = N;
                                contactSet.d = signedDistance;
                                contactSet.vn = vn;
                            }
                        }

                        if(deepestPenetration < 0.0f)
                            contacts.push_back(contactSet); // Insert actual collision pair if penetration happened

                        break;
                    }
                    default: break;
                }
            break;
        }
    }

    return contacts;
}

void XPBDSolver::solvePositions(const std::vector<ContactSet>& contacts, const double& h) {

    for (auto contact: contacts) {

        glm::vec3 r1 = contact.A->pose.p - contact.p;
        glm::vec3 r2 = contact.B->pose.p - contact.p;

        // const glm::vec3 p1 = contact.A->pose.p + contact.A->pose.q * r1;
        // const glm::vec3 p2 = contact.B->pose.p + contact.B->pose.q * r2;
        // contact.d = glm::dot(contact.N, (p1 - p2)); // D seems to be way off

        // contact.d = -glm::dot(contact.N, (contact.P - contact.B->pose.p));
        
        if(contact.d >= 0.0f) {
            continue; // Contact has been solved
        }

        glm::vec3 posCorr = -contact.d * contact.n;

        // Renderer &renderer = Renderer::Instance();
        // renderer.debugVector->setPosition(contact.P);
        // renderer.debugVector->setScale(glm::vec3(glm::length(posCorr) * 1.0f));
        // renderer.debugVector->setRotation(Quaternion::createFromTwoVectors(
        //     glm::vec3(0.0f, 1.0f, 0.0f),
        //     contact.N
        // ));

        XPBDSolver::applyBodyPairCorrection(
            // contact.A,
            // contact.B,
            contact,
            posCorr,
            0.0f,
            (float) h,
            contact.p,
            contact.p,
            false
        );
        
        // // @TODO eq. 27, 28
        // const glm::vec3 p1prev = contact.A->prevPose.p + contact.A->prevPose.q * r1;
        // const glm::vec3 p2prev = contact.B->prevPose.p + contact.B->prevPose.q * r2;
    }
}

void XPBDSolver::solveVelocities(const std::vector<ContactSet>& contacts, const double& h) {
    
    for (auto contact: contacts) {
        
        // (29)
        // @TODO check if result is same as eq. 29
        // glm::vec3 v = contact.A->getPointVelocityW(contact.P) - contact.B->getPointVelocityW(contact.P);
        glm::vec3 v = contact.A->getVelocityAt(contact.p) - contact.B->getVelocityAt(contact.p);
        float vn = glm::dot(contact.n, v);
        glm::vec3 vt = v - contact.n * vn;

        // Maybe check contact.d instead
        if(vn >= 0.0f) {
            continue; // Contact has been solved
        }

        glm::vec3 dv = glm::vec3(0.0f);

        // Calculate average friction
        float staticFriction = 0.5 * (contact.A->staticFriction + contact.A->staticFriction);
        float dynamicFriction = 0.5 * (contact.A->dynamicFriction + contact.A->dynamicFriction);

        // (30) Friction
        // float h_squared = (float) h * (float) h;
        // glm::vec3 Fn = contact.lambdaN / h_squared;
        // float vt_length = glm::length(vt);
        // glm::vec3 normalizedVt = (vt_length > 0.0001f) ? glm::normalize(vt) : glm::vec3(0.0f);
        // dv += -normalizedVt * min((float) h * dynamicFriction * Fn, vt_length);     
        
        // (31, 32) @TODO dampening
        // dv -= *contact.A->velocity - contact.B->velocity) * min((float) h * staticFriction, 1); // (31) ????
        // domega = .........

        // (34), restitution
        // To handle restitution we also need v(bar)n, the normal velocity before
        // the PBD velocity update. We compute this quantity by applying
        // Eqn 29 to the pre-update velocities
        float e = 0.5f * (contact.A->bounciness + contact.B->bounciness);
        if(glm::length(vn) < (2.0f * -9.81f * h)) {
            e = 0.0f;
        }
        // dv += contact.N * (-vn + glm::max(-e * contact.N * contact.vn, glm::vec3(0.0f)));
        dv += contact.n * (-vn + (-e * contact.n * contact.vn));

        XPBDSolver::applyBodyPairCorrection(
            // contact.A,
            // contact.B,
            contact,
            dv,
            0.0f,
            (float) h,
            contact.p,
            contact.p,
            true
        );

    }
}

void XPBDSolver::applyBodyPairCorrection(
    // RigidBody* body0, 
    // RigidBody* body1, 
    ContactSet& contact,
    glm::vec3& corr, 
    float compliance, 
    float dt, 
    glm::vec3 pos0,
    glm::vec3 pos1,
    bool velocityLevel
) {

    RigidBody* body0 = contact.A;
    RigidBody* body1 = contact.B;

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
    // float alpha = compliance / (dt*dt);
    // float dlambda = (-C - alpha * contact.lambdaN) / (w + alpha);
    // contact.lambdaN += dlambda;

    n = n * -dlambda; // Why is this negative
    if (body0->isDynamic)
        body0->applyCorrection(n, pos0, velocityLevel);
    if (body1->isDynamic) {
        body1->applyCorrection(n * -1.0f, pos1, velocityLevel);
    }
}
