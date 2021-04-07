#include "physics/PhysicsHandler.h"

PhysicsHandler& PhysicsHandler::Instance() {
    static PhysicsHandler instance;
    return instance;
}

PhysicsHandler::PhysicsHandler() {}

void PhysicsHandler::Enqueue(Mesh* mesh) {
    this->Enqueue(new RigidBody(mesh));
}

void PhysicsHandler::Enqueue(RigidBody* body) {
    this->bodies.push_back(body);
}

// This entire loop should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
void PhysicsHandler::update() {

    Time &time = Time::Instance();

    switch(PhysicsHandler::solverType) {

        case SolverType::XPBD :
            XPBDSolver::update(this->bodies, time.dt);
        default : 
            
        break;
    }

}
