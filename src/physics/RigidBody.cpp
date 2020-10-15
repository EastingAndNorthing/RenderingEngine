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
    this->mass = std::numeric_limits<float>::max(); // lol
}

void RigidBody::applyForce(glm::vec3 worldForce, glm::vec3 localPosition) {
    this->applyForce(RigidBodyForce(worldForce, localPosition));
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    if(this->isDynamic) {

        this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));
        
        // https://gafferongames.com/post/physics_in_3d/
        // https://gamedev.stackexchange.com/questions/147409/rotate-from-current-rotation-to-another-using-angular-velocity
        // https://www.euclideanspace.com/physics/kinematics/angularvelocity/index.htm
        // https://stackoverflow.com/questions/33182258/how-to-scale-the-rotation-of-a-quaternion

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
        // this->angularVelocity += this->angularAcceleration * (float) deltaTime;
        // this->rotation *= glm::quat(this->angularVelocity * (float) deltaTime);
        // this->rotation = glm::normalize(this->rotation);

        this->externalForces.clear();

        if(glm::length(this->velocity) >= this->sleepVelocity || glm::length(this->velocity) >= this->sleepVelocity) {
            this->updateGeometry();
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
