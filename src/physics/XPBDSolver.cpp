#include "physics/XPBDSolver.h"

void XPBDSolver::update(const std::vector<RigidBody*> rigidBodies) {

    // Implementation of XPBD algorithm 2.

    Time &time = Time::Instance();
    
    const double h = time.dt / XPBDSolver::numSubSteps;

    auto contacts = XPBDSolver::getCollisionPairs(rigidBodies);

    for(int i; i < XPBDSolver::numSubSteps; i++) {

        for (auto& body: rigidBodies) {
            body->updatePhysics(h);
        }
        
        for(int i = 0; i < XPBDSolver::numConstraintIterations; i++) {
            XPBDSolver::solvePositions(contacts);
        }

        for (auto& body: rigidBodies) {
            body->computeActualState(h);
        }

        XPBDSolver::solveVelocities(contacts);

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


void XPBDSolver::solvePositions(const std::vector<ContactSet>& contacts) {

    for (auto contact: contacts) {
        if(contact.A->isDynamic) {
            contact.A->position -= contact.N * (contact.d * contact.A->_inverseMass);
        } else {
            contact.A->position -= contact.N * contact.d;
        }

        if(contact.B->isDynamic) {
            contact.B->position += contact.N * (contact.d * contact.B->_inverseMass);
        } else {
            contact.B->position -= contact.N * contact.d;
        }
    }
}

void XPBDSolver::solveVelocities(const std::vector<ContactSet>& contacts) {
    
}
