#include "RigidBody.h"

RigidBody::RigidBody(Mesh* mesh) {
    this->mesh = mesh;
    this->mesh->managedByRigidBody = true;

    this->position = mesh->position;
    // this->rotation = mesh->rotation;

    this->applyForce({glm::vec3( 0.0f, this->mass * this->gravity, 0.0f )});
}

void RigidBody::update(double &time, double &deltaTime) {

    for (auto force: this->externalForces) {
        this->acceleration += force.force / this->mass; // F = ma
        // this->angularAcceleration += (force.force * glm::distance(this->position, force.position)) / I; // a = T/I
    }

    this->velocity += this->acceleration * (float) deltaTime;
    glm::vec3 newPos = this->position + this->velocity * (float) deltaTime;

    if((newPos.y - this->boundingBox.y/2) <= 0) {
        this->position.y = this->boundingBox.y/2;
        this->acceleration.y = 0;
        this->velocity.y *= -this->bounciness;
        this->updateGeometry();
    } else {
        this->position = newPos;
    }

    if(glm::length(this->velocity) >= 0.000001) {
        this->updateGeometry();
    }

}

void RigidBody::applyForce(glm::vec3 force, glm::vec3 position) {
    this->applyForce({ force, position });
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::handleCollision() {

}

void RigidBody::updateGeometry() {
    this->mesh->_worldPosMatrixNeedsUpdate = true;
    this->mesh->position = this->position;
//  this->mesh->rotation = this->rotation;
}
