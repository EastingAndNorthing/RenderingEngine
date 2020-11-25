#include "RigidBody.h"
#include "math/CoordinateSystem.h"
#include "math/Quaternion.h"

RigidBody::RigidBody(Mesh* mesh, Collider* collider)
    : mesh(mesh), collider(collider) {

    if(mesh) {
        this->mesh->managedByRigidBody = true;
        this->state.position = mesh->position;
        this->state.rotation = mesh->rotation;
    }

}

void RigidBody::makeStatic() {
    
    this->isDynamic = false;
    this->mass = 99999.0f;
    this->state.inertiaTensor = glm::mat3(99999.0f);

}

void RigidBody::applyForceW(RigidBodyForce force) {
    this->applyForceL(force.force, CoordinateSystem::worldToLocal(force.position, this->state._inverseRotation));
}

void RigidBody::applyForceW(glm::vec3 worldForce, glm::vec3 positionW) {
    this->applyForceL(RigidBodyForce(worldForce, CoordinateSystem::worldToLocal(positionW, this->state._inverseRotation)));
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
    
    this->state.velocity += impulse;
    
    glm::vec3 localImpulse = CoordinateSystem::worldToLocal(impulse, this->state._inverseRotation, glm::vec3(0.0f));
    glm::vec3 localPosition = CoordinateSystem::worldToLocal(position, this->state._inverseRotation, this->state.position);
    
    this->state.angularVelocity += this->state._inverseInertiaTensor * glm::cross(localImpulse, -localPosition);

}

glm::vec3 RigidBody::getAngularVelocityW() {
    
    return CoordinateSystem::localToWorld(this->state.angularVelocity, this->state.rotation);   

}

glm::vec3 RigidBody::getPointVelocityL(const glm::vec3& localPoint) {
    
    return glm::cross(this->state.angularVelocity, localPoint);

}

glm::vec3 RigidBody::getPointVelocityW(glm::vec3 point, bool isPointInLocalSpace) {

    if(!isPointInLocalSpace) 
        point = CoordinateSystem::worldToLocal(point, this->state._inverseRotation, this->state.position);

    return this->state.velocity + CoordinateSystem::localToWorld(this->getPointVelocityL(point), this->state.rotation);

}

void RigidBody::updatePhysics(const double &deltaTime) {

    this->previousState = this->state;

    this->rebuildPrecomputedValues();

    if(this->isDynamic) {
        
        this->state.acceleration          = glm::vec3(0.0f);
        this->state.angularAcceleration   = glm::vec3(0.0f);
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

        this->state.acceleration += this->forces * this->state._inverseMass;
        this->state.velocity += this->state.acceleration * dt;  
        this->state.position += this->state.velocity * dt;

        this->state.angularAcceleration += this->torque * this->state._inverseInertiaTensor; // a = T/I
        glm::vec3 halve = 0.5f * this->state.angularVelocity * dt;
        this->state.rotation *= glm::quat(1.0, halve.x, halve.y, halve.z);
        this->state.rotation = glm::normalize(this->state.rotation);

        this->state.angularVelocity -= this->state.angularVelocity * this->rotationalDamping * dt;

        if(glm::length(this->state.velocity) >= this->sleepVelocity || glm::length(this->state.angularVelocity) >= this->sleepAngularVelocity) {
            
            _inertiaNeedsUpdate = true;
            _rotationNeedsUpdate = true;

            this->rebuildPrecomputedValues();
            this->updateGeometry();

        } else {

            this->state.velocity -= 0.7f * this->state.velocity * dt;
            this->state.angularVelocity -= 0.7f * this->state.velocity * dt;
            
        }
    }
}

void RigidBody::updateGeometry() {
    if(this->mesh) {
        this->mesh->_worldPosMatrixNeedsUpdate = true;
        this->mesh->position = this->state.position;
        this->mesh->rotation = this->state.rotation;
    }
}

void RigidBody::rebuildPrecomputedValues() {
    if(this->_massNeedsUpdate)
        this->state._inverseMass = 1 / this->mass;

    if(this->_rotationNeedsUpdate) {
        this->state._inverseRotation = glm::inverse(this->state.rotation);
    }

    if(this->_inertiaNeedsUpdate) {
        // this->_inertiaTensorW = this->inertiaTensor * glm::mat3_cast(this->rotation);
        this->state._inverseInertiaTensor = glm::inverse(this->state.inertiaTensor);
        // this->_inverseInertiaTensorW = glm::inverse(this->_inertiaTensorW);
    }

    this->_rotationNeedsUpdate = false;
    this->_inertiaNeedsUpdate = false;
    this->_massNeedsUpdate = false;
}
