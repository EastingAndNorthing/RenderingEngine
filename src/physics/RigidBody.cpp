#include "RigidBody.h"
#include "math/CoordinateSystem.h"
#include "math/Quaternion.h"

RigidBody::RigidBody(Mesh* mesh, Collider* collider)
    : mesh(mesh), collider(collider) {

    if(mesh) {
        this->mesh->managedByRigidBody = true;
        this->position = mesh->position;
        this->rotation = mesh->rotation;
    }

}

void RigidBody::makeStatic() {
    
    this->isDynamic = false;
    this->mass = 99999.0f;
    this->inertiaTensor = glm::mat3(99999.0f);

}

void RigidBody::applyForceW(RigidBodyForce force) {
    this->applyForceL(force.force, CoordinateSystem::worldToLocal(force.position, this->_inverseRotation));
}

void RigidBody::applyForceW(glm::vec3 worldForce, glm::vec3 positionW) {
    this->applyForceL(RigidBodyForce(worldForce, CoordinateSystem::worldToLocal(positionW, this->_inverseRotation)));
}

void RigidBody::applyForceL(glm::vec3 worldForce, glm::vec3 positionL) {
    this->applyForceL(RigidBodyForce(worldForce, positionL));
}

void RigidBody::applyForceL(RigidBodyForce force) {
    this->externalForces.push_back(force);
}

void RigidBody::applyLocalImpulse(const glm::vec3& impulse, const glm::vec3& position) {
    
    std::cout << "applyLocalImpulse is not yet implemented" << std::endl;

}

void RigidBody::applyWorldImpulse(const glm::vec3& impulse, const glm::vec3& position) {
    
    this->velocity += impulse;
    
    glm::vec3 localImpulse = CoordinateSystem::worldToLocal(impulse, this->_inverseRotation, glm::vec3(0.0f));
    glm::vec3 localPosition = CoordinateSystem::worldToLocal(position, this->_inverseRotation, this->position);
    
    this->angularVelocity += this->_inverseInertiaTensor * glm::cross(localImpulse, -localPosition);

}

glm::vec3 RigidBody::getAngularVelocityW() {
    
    return CoordinateSystem::localToWorld(this->angularVelocity, this->rotation);   

}

glm::vec3 RigidBody::getPointVelocityL(const glm::vec3& localPoint) {
    
    return glm::cross(this->angularVelocity, localPoint);

}

glm::vec3 RigidBody::getPointVelocityW(glm::vec3 point, bool isPointInLocalSpace) {

    if(!isPointInLocalSpace) 
        point = CoordinateSystem::worldToLocal(point, this->_inverseRotation, this->position);

    return this->velocity + CoordinateSystem::localToWorld(this->getPointVelocityL(point), this->rotation);

}

void RigidBody::updatePhysics(const double &deltaTime) {

    this->rebuildPrecomputedValues();

    if(this->isDynamic) {
        
        this->acceleration          = glm::vec3(0.0f);
        this->angularAcceleration   = glm::vec3(0.0f);
        this->forces                = glm::vec3(0.0f);
        this->torque                = glm::vec3(0.0f);

        this->applyForceL(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));

        for (auto force: this->externalForces) {
            this->forces += force.force;
            this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position)); // T = F x r
        }
        this->externalForces.clear();

        // Implicit Euler integration
        const float dt = (float) deltaTime;

        this->acceleration += this->forces * this->_inverseMass;
        this->velocity += this->acceleration * dt;  
        this->position += this->velocity * dt;

        this->angularAcceleration += this->torque * this->_inverseInertiaTensor; // a = T/I
        glm::vec3 halve = 0.5f * this->angularVelocity * dt;
        this->rotation *= glm::quat(1.0, halve.x, halve.y, halve.z);
        this->rotation = glm::normalize(this->rotation);

        this->angularVelocity -= this->angularVelocity * this->rotationalDamping * dt;

        if(glm::length(this->velocity) >= this->sleepVelocity || glm::length(this->angularVelocity) >= this->sleepAngularVelocity) {
            
            _inertiaNeedsUpdate = true;
            _rotationNeedsUpdate = true;

            this->rebuildPrecomputedValues();
            this->updateGeometry();

        } else {

            this->velocity -= 0.7f * this->velocity * dt;
            this->angularVelocity -= 0.7f * this->velocity * dt;
            
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
    if(this->_massNeedsUpdate)
        this->_inverseMass = 1 / this->mass;

    if(this->_rotationNeedsUpdate) {
        this->_inverseRotation = glm::inverse(this->rotation);
    }

    if(this->_inertiaNeedsUpdate) {
        // this->_inertiaTensorW = this->inertiaTensor * glm::mat3_cast(this->rotation);
        this->_inverseInertiaTensor = glm::inverse(this->inertiaTensor);
        // this->_inverseInertiaTensorW = glm::inverse(this->_inertiaTensorW);
    }

    this->_rotationNeedsUpdate = false;
    this->_inertiaNeedsUpdate = false;
    this->_massNeedsUpdate = false;
}
