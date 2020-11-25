#include "physics/XPBDSolver.h"

// Runs every frame
void XPBDSolver::update() {

    Time &time = Time::Instance();
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();
    
    const int h = time.dt / XPBDSolver::numSubSteps; // Store inverse

    auto contacts = XPBDSolver::getCollisionPairs();

    for(int i; i < XPBDSolver::numSubSteps; i++) {

        for (auto& body: physicsHandler.bodies) {

            body->rebuildPrecomputedValues(); // Maybe test doing this only per timestep, not substep.

            body->updatePhysics(time.dt);
            
        }

    }
}

std::vector<ContactSet> XPBDSolver::getCollisionPairs() {

    std::vector<ContactSet> contacts = {};
    
    PhysicsHandler &physicsHandler = PhysicsHandler::Instance();

    for (auto body: physicsHandler.bodies) {

        // @TODO Chunking / octree, prevent checking body pairs multiple times
        for (auto otherBody: physicsHandler.bodies) {
            
            if (!body->isDynamic && !otherBody->isDynamic)
                continue;

            if (body == otherBody)
                continue;

            // @TODO Broad phase collision detection using bounding box proximity check
            // if(body->boundingBox.intersects(otherBody.boundingBox)) {}
            
            // @TODO Need to get proper touch point and collision plane
            contacts.push_back(ContactSet(body, otherBody, glm::vec3(0.0f), glm::vec3(0.0f)));

        }

    }

    return contacts;
}
