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
    this->mass = 9999999999.0f;
    this->inertiaTensor = glm::mat3(9999999999.0f);
}

void RigidBody::applyForce(glm::vec3 worldForce, glm::vec3 localPosition) {
    this->applyForce(RigidBodyForce(worldForce, localPosition));
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    this->rebuildPrecomputedValues();

    if(this->isDynamic) {
        this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));
        
        this->acceleration = glm::vec3(0.0f);
        this->angularAcceleration = glm::vec3(0.0f);
        this->torque = glm::vec3(0.0f);

        for (auto force: this->externalForces) {
            this->acceleration += force.force / this->mass; // F = ma
            // this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position)); // T = F x r
        }

        this->velocity += this->acceleration * (float) deltaTime;
        this->position += this->velocity * (float) deltaTime;

        // this->angularAcceleration += this->torque / this->inertia; // a = T/I (or T * inv_I)
        // this->rotation *= glm::quat(this->angularVelocity * (float) deltaTime);
        glm::vec3 halve = this->angularVelocity * (float) deltaTime * 0.5f; // vector of half angle
        glm::quat deltaQuat = glm::quat(1.0, halve.x, halve.y, halve.z);
        this->rotation *= deltaQuat;
        this->rotation = glm::normalize(this->rotation);

        float velocity = glm::length(this->velocity);
        float angularVelocity = glm::length(this->angularVelocity);

        this->externalForces.clear();

        // if(velocity >= this->sleepVelocity || angularVelocity >= this->sleepAngularVelocity) {
            _inertiaNeedsUpdate = true;
            _rotationNeedsUpdate = true;
            this->rebuildPrecomputedValues();
            this->updateGeometry();
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

void RigidBody::rebuildPrecomputedValues() {
    if(this->_massNeedsUpdate)
        this->inverseMass = 1 / this->mass;

    if(this->_rotationNeedsUpdate)
        this->inverseRotation = glm::inverse(this->rotation);

    if(this->_inertiaNeedsUpdate) {
        this->inertiaTensorW = this->inertiaTensor * glm::mat3_cast(this->rotation);
        this->inverseInertiaTensor = glm::inverse(this->inertiaTensor);
        this->inverseInertiaTensorW = glm::inverse(this->inertiaTensorW);
    }

    this->_rotationNeedsUpdate = false;
    this->_inertiaNeedsUpdate = false;
    this->_massNeedsUpdate = false;
}
