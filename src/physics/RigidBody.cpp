#include "RigidBody.h"

RigidBody::RigidBody(Mesh* mesh) {
    this->mesh = mesh;
    this->mesh->managedByRigidBody = true;

    this->position = mesh->position;
    this->rotation = mesh->rotation;

    if(this->isDynamic) {
        this->applyForce(glm::vec3(0.0f, this->mass * this->gravity, 0.0f));
    }

}

void RigidBody::applyForce(glm::vec3 worldForce, glm::vec3 localPosition) {
    
    // worldForce = this->rotation * worldForce;
    // std::cout << worldForce.y << std::endl;

    this->applyForce({ worldForce, localPosition });
}

void RigidBody::applyForce(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::updatePhysics(const double &deltaTime) {

    if(this->isDynamic) {
        
        this->acceleration.x = 0.0f;
        this->acceleration.y = 0.0f;
        this->acceleration.z = 0.0f;

        // this->angularAcceleration.x = 0.0f;
        // this->angularAcceleration.y = 0.0f;
        // this->angularAcceleration.z = 0.0f;
        // this->angularAcceleration.w = 0.0f;

        // https://gafferongames.com/post/physics_in_3d/
        for (auto force: this->externalForces) {
            this->acceleration += force.force / this->mass; // F = ma

            this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position)); // T = F x r
            
            if(this->externalForces.size() > 1){
                this->externalForces.erase(this->externalForces.begin() + 1, this->externalForces.end()); // Remove forces, keep gravity
            }
        }

        this->velocity += this->acceleration * (float) deltaTime;
        this->position += this->velocity * (float) deltaTime;


        //https://www.euclideanspace.com/physics/kinematics/angularvelocity/index.htm
        // this->angularAcceleration += glm::quat(totalTorque) * 1000.0f; //; // a = T/I (or T * inv_I)
        // this->angularVelocity += this->angularAcceleration * (float) deltaTime;
        // this->angularVelocity.x += 1.0f; // This simply aligns the object to the x axis

        this->rotation += this->angularVelocity * (float) deltaTime;
        this->rotation = glm::normalize(this->rotation);

        this->updateGeometry();

        if(glm::length(this->velocity) >= this->sleepVelocity || glm::length(this->angularVelocity) >= this->sleepVelocity) {
            this->updateGeometry();
        }
    }
}

void RigidBody::updateGeometry() {
    this->mesh->_worldPosMatrixNeedsUpdate = true;
    this->mesh->position = this->position;
    this->mesh->rotation = this->rotation;
}
