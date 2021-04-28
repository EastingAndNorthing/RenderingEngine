#include "RigidBody.h"
#include "core/Renderer.h"

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
    this->gravity = 0.0f;
    this->mass = 99999.0f;
    this->inertiaTensor = glm::mat3(99999.0f);

}

// For applyForce, why not directly compute F.net and T?
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

// @TODO also update planeCollider after rotation
void RigidBody::updatePhysics(const float &deltaTime) {

    if(this->isDynamic) {
        
        this->rebuildPrecomputedValues(); // Maybe try running this each frame, rather than each substep/iteration. 

        // xprev ← x;
        // qprev ← q;
        this->previousState.position = this->position;
        this->previousState.rotation = this->rotation;
        this->previousState._inverseRotation = this->_inverseRotation;
        
        this->acceleration                = glm::vec3(0.0f);
        this->angularAcceleration         = glm::vec3(0.0f); // Not used
        this->forces                      = glm::vec3(0.0f);
        this->torque                      = glm::vec3(0.0f);

        this->applyForceL(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));

        for (auto force: this->externalForces) {
            this->forces += force.force;
            this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position));
        }
        this->externalForces.clear();

        // Euler step
        this->acceleration += this->forces * this->_inverseMass;
        this->velocity += this->acceleration * deltaTime;  
        this->position += this->velocity * deltaTime;

        // this->angularVelocity += deltaTime * this->_inverseInertiaTensor * (this->torque - glm::cross(this->angularVelocity, this->inertiaTensor * this->angularVelocity));
        // this->rotation += deltaTime * 0.5f * glm::quat(1.0, this->angularVelocity.x, this->angularVelocity.y, this->angularVelocity.z) * this->rotation;
        this->rotation += deltaTime * 0.5f * glm::quat(1.0f, this->angularVelocity.x, this->angularVelocity.y, this->angularVelocity.z) * this->rotation;
        this->rotation = glm::normalize(this->rotation);
        
        // this->angularVelocity -= this->angularVelocity * this->rotationalDamping * deltaTime;

        // if(glm::length(this->velocity) >= this->sleepVelocity || glm::length(this->angularVelocity) >= this->sleepAngularVelocity) {
            
            _inertiaNeedsUpdate = true;
            _rotationNeedsUpdate = true;

            this->rebuildPrecomputedValues();
            this->updateCollider();
            this->updateGeometry();

        // }
    }
}

void RigidBody::computeActualState(const float &deltaTime) {

    // Algorithm 2

    // v ← (x−xprev)/deltaTime;
    // this->velocity = (this->position - this->previousState.position) / deltaTime; 

    // // Δq ← q*q.inv_prev
    // glm::quat dq = this->rotation * this->previousState._inverseRotation;

    // // omega ← 2[Δqx,Δqy,Δqz]/deltaTime;
    // // Why does this seem to dampen / lose energy over time?
    // this->angularVelocity = 2.0f * glm::vec3(dq.x, dq.y, dq.z) / deltaTime;

    // // omega ← Δqw ≥ 0 ? omega : −omega;
    // if(dq.w < 0) this->angularVelocity *= -1;

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
        // this->_inertiaTensorW = this->inertiaTensor * glm::mat3_cast(this->rotation); // Seems like world inertia is not needed
        this->_inverseInertiaTensor = glm::inverse(this->inertiaTensor);
        // this->_inverseInertiaTensorW = glm::inverse(this->_inertiaTensorW); // Seems like world inertia is not needed
    }

    this->_rotationNeedsUpdate = false;
    this->_inertiaNeedsUpdate = false;
    this->_massNeedsUpdate = false;
}

void RigidBody::updateCollider() {

    switch(this->collider->colliderType) {

        case ColliderType::Plane :
            auto PC = static_cast<PlaneCollider*>(this->collider);
            PC->normal = this->rotation * glm::vec3(0.0f, 0.0f, 1.0f); // @TODO store initial up direction of plane collider normal
            
            // Renderer &renderer = Renderer::Instance();
            // renderer.debugVector->setPosition(this->position);
            // renderer.debugVector->setScale(10.0f);
            // renderer.debugVector->setRotation(Quaternion::createFromTwoVectors(
            //     glm::vec3(0.0f, 1.0f, 0.0f),
            //     PC->normal
            // ));
            
        break;
        
    }
}
