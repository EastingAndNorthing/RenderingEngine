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
    if(body->mesh->material != NULL) {
        this->bodies.push_back(body);
    } else {
        Log("No material assigned to mesh, skipped.", LogLevel::ERROR);
    }
}

void PhysicsHandler::update() {

    Time &time = Time::Instance();
    
    for (auto& body: this->bodies) {

        this->handleCollisions(body);

        body->update(time.time, time.dt);

    }
}

void PhysicsHandler::handleCollisions(RigidBody* body) {

    // This could be inefficient, checking the same body pairs multiple times. Consider setting a 'isChecked' boolean?
    for (auto otherBody: this->bodies) {

        if (body != otherBody) {
            body->handleCollision(otherBody);
        }

    }

}
