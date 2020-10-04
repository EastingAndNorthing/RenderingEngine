#include "RigidBody.h"

RigidBody::RigidBody(Mesh* mesh) {
    this->mesh = mesh;
    this->mesh->managedByRigidBody = true;

    this->position = mesh->position;
    // this->rotation = mesh->rotation;

    this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f));

}

void RigidBody::update(double &time, double &deltaTime) {

    this->acceleration.x = 0;
    this->acceleration.y = 0;
    this->acceleration.z = 0;

    for (auto force: this->externalForces) {
        this->acceleration += force.force / this->mass; // F = ma

        // https://gafferongames.com/post/physics_in_3d/
        // A torque is also generated based on the cross product of the force vector and the point on the object relative to the center of mass of the object
        // this->angularAcceleration += (force.force * glm::distance(this->position, force.position)) / I; // a = T/I
    
        if(this->externalForces.size() > 1){
            this->externalForces.erase(this->externalForces.begin() + 1, this->externalForces.end()); // Remove forces, keep gravity
        }
    }

    if((this->position.y - this->boundingBox.y/2) <= 0) {
        this->position.y = this->boundingBox.y/2;
        this->velocity.y *= -this->bounciness;
        this->acceleration.y = 0;

        this->velocity.x *= 0.95;
        this->velocity.z *= 0.95;

        // glm::vec3 normalForce = this->externalForces[0].force;
        // glm::vec3 perpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->velocity);
        // float mu = (glm::length(this->velocity) > 0.1) ? this->dynamicFriction : this->staticFriction;
        // this->velocity -= 1000 * mu * normalForce * (float) deltaTime;
    }

    this->velocity += this->acceleration * (float) deltaTime;
    this->position += this->velocity * (float) deltaTime;

    if(glm::length(this->velocity) >= 0.0000001) {
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
