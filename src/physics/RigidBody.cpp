#include "RigidBody.h"

RigidBody::RigidBody(Mesh* mesh, Collider* collider) {

    if(mesh) {
        this->mesh = mesh;
        this->mesh->managedByRigidBody = true;
        this->position = mesh->position;
        this->rotation = mesh->rotation;
    }

    if(collider) {
        this->collider = collider;
    }
}

void RigidBody::makeStatic() {
    this->isDynamic = false;
    this->bounciness = 1.0f;
    // this->mass = std::numeric_limits<float>::max(); // lol
    this->mass = 99999.0f;
    this->inertiaTensor = glm::mat3(9999.0f);
}

void RigidBody::applyForce(glm::vec3 worldForce, glm::vec3 localPosition) {
    this->applyForce(RigidBodyForce(worldForce, localPosition));
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    if(this->isDynamic) {
        this->inertiaTensorW = glm::matrixCompMult(glm::mat3_cast(this->rotation), this->inertiaTensor);
        // this->inertiaTensorW = glm::mat3_cast(this->rotation) * this->inertiaTensor;
        this->inverseInertiaTensor = glm::inverse(this->inertiaTensor);
        this->inverseInertiaTensorW = glm::inverse(this->inertiaTensorW);

        this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));
        
        this->acceleration = glm::vec3(0.0f);
        this->angularAcceleration = glm::vec3(0.0f);
        this->torque = glm::vec3(0.0f);

        for (auto force: this->externalForces) {
            this->acceleration += force.force / this->mass; // F = ma
            this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position)); // T = F x r
        }

        this->velocity += this->acceleration * (float) deltaTime;
        this->position += this->velocity * (float) deltaTime;

        // glm::vec3 normalForce = this->externalForces[0].force;
        // glm::vec3 perpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->velocity);
        // float mu = (glm::length(this->velocity) > 0.1) ? this->dynamicFriction : this->staticFriction;
        // this->velocity -= 1000 * mu * normalForce * (float) deltaTime;

        // this->angularAcceleration += this->torque / this->inertia; // a = T/I (or T * inv_I)
        this->angularVelocity += this->angularAcceleration * (float) deltaTime;
        this->rotation *= glm::quat(this->angularVelocity * (float) deltaTime);
        this->rotation = glm::normalize(this->rotation);

        this->externalForces.clear();

        this->updateGeometry();
        // if(glm::length(this->velocity) >= this->sleepVelocity || glm::length(this->velocity) >= this->sleepAngularVelocity) {
        // }
    }
}

void RigidBody::updateGeometry() {
    if(this->mesh) {
        this->mesh->_worldPosMatrixNeedsUpdate = true;
        this->mesh->position = this->position;
        this->mesh->rotation = this->rotation;
    }
}

glm::mat4 RigidBody::getWorldPositionMatrix() { // Same as Mesh...
    if(this->_worldPosMatrixNeedsUpdate) {
        this->_worldPositionMatrix = glm::translate(glm::mat4(1), this->position) * glm::mat4_cast(this->rotation); // @TODO scale
    }
    this->_worldPosMatrixNeedsUpdate = false;
    
    return this->_worldPositionMatrix;
}
