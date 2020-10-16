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
    this->inertiaTensor = glm::mat3(99999.0f);
}

void RigidBody::applyForce(glm::vec3 worldForce, glm::vec3 localPosition) {
    this->applyForce(RigidBodyForce(worldForce, localPosition));
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    if(this->isDynamic) {

        this->rebuildPrecomputedValues();

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

        // this->angularAcceleration += this->torque / this->inertia; // a = T/I (or T * inv_I)
        this->angularVelocity += this->angularAcceleration * (float) deltaTime;
        this->rotation *= glm::quat(this->angularVelocity * (float) deltaTime);
        this->rotation = glm::normalize(this->rotation);

        this->externalForces.clear();

        this->updateGeometry();

        float velocity = glm::length(this->velocity);
        float angularVelocity = glm::length(this->velocity);

        if(velocity >= this->sleepVelocity || angularVelocity >= this->sleepAngularVelocity) {
            _inertiaNeedsUpdate = true;
            this->rebuildPrecomputedValues();
        }
    }
}

void RigidBody::updateGeometry() {
    if(this->mesh) {
        this->mesh->_worldPosMatrixNeedsUpdate = true;
        this->mesh->position = this->position;
        this->mesh->rotation = this->rotation;
    }
}

void RigidBody::rebuildPrecomputedValues() {
    if(this->_massNeedsUpdate) { 
        this->inverseMass = 1 / this->mass;
    }

    if(this->_inertiaNeedsUpdate) {
        // this->inertiaTensorW = glm::mat3(glm::toMat4(this->rotation) * glm::mat4(this->inertiaTensor)); // Kinda same result as just glm::toMat3(this->rotation) * this->inertiaTensor
        this->inertiaTensorW = glm::toMat3(this->rotation) * this->inertiaTensor;
        this->inverseInertiaTensor = glm::inverse(this->inertiaTensor);
        this->inverseInertiaTensorW = glm::inverse(this->inertiaTensorW);
    }

    this->_inertiaNeedsUpdate = false;
    this->_massNeedsUpdate = false;
}
