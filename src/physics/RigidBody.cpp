#include "RigidBody.h"

RigidBody::RigidBody(Mesh* mesh) {
    this->mesh = mesh;
    this->mesh->managedByRigidBody = true;

    this->position = mesh->position;
    // this->rotation = mesh->rotation;

    if(this->isDynamic) {
        this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f));
    }

}

void RigidBody::applyForce(glm::vec3 force, glm::vec3 position) {
    this->applyForce({ force, position });
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    if(this->isDynamic) {
        
        this->acceleration.x = 0.0f;
        this->acceleration.y = 0.0f;
        this->acceleration.z = 0.0f;

        for (auto force: this->externalForces) {
            this->acceleration += force.force / this->mass; // F = ma

            // https://gafferongames.com/post/physics_in_3d/
            // A torque is also generated based on the cross product of the force vector and the point on the object relative to the center of mass of the object
            // this->angularAcceleration += (force.force * glm::distance(this->position, force.position)) / I; // a = T/I
        
            if(this->externalForces.size() > 1){
                this->externalForces.erase(this->externalForces.begin() + 1, this->externalForces.end()); // Remove forces, keep gravity
            }
        }

        this->velocity += this->acceleration * (float) deltaTime;
        this->position += this->velocity * (float) deltaTime;

        if(glm::length(this->velocity) >= this->sleepVelocity) {
            this->updateGeometry();
        }
    }
}

void RigidBody::updateGeometry() {
    this->mesh->_worldPosMatrixNeedsUpdate = true;
    this->mesh->position = this->position;
//  this->mesh->rotation = this->rotation;
}
