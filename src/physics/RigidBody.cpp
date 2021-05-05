#include "RigidBody.h"
#include "core/Renderer.h"

RigidBody::RigidBody(Mesh* mesh, Collider* collider)
    : mesh(mesh), collider(collider) {

    if(mesh) {
        this->mesh->managedByRigidBody = true;
        this->pose.p = mesh->position;
        this->pose.q = mesh->rotation;
    }
}

void RigidBody::makeStatic() {
    this->isDynamic = false;
    this->gravity = 0.0f;
    this->invMass = 0.0f;
    this->invInertia = glm::vec3(0.0f);

    this->updateGeometry();
    this->updateCollider();
}

float RigidBody::getInverseMass(glm::vec3& normal, glm::vec3 pos) {
    // glm::vec3 n = glm::vec3(0.0f);
    // if (glm::length(pos) < 0.00001f) {
    //     // @TODO check epsilon
    //     std::cout << "pos is zero" << std::endl;
    //     n = normal;
    // } else {
    //     n = pos - this->pose.p;
    //     n = glm::cross(n, normal);
    // }
    // this->pose.invRotate(n);
    // float w = 
    //     n.x * n.x * this->invInertia.x +
    //     n.y * n.y * this->invInertia.y +
    //     n.z * n.z * this->invInertia.z;
    // if (glm::length(pos) < 0.00001f)
    //     w += this->invMass;
    // return w;

    glm::vec3 r = pos - this->pose.p;
    glm::vec3 rcross = glm::cross(r, normal);

    glm::vec3 I = glm::conjugate(this->pose.q) * this->invInertia;

    float inertiaInfluence = 
        rcross.x * rcross.x * I.x +
        rcross.y * rcross.y * I.y +
        rcross.z * rcross.z * I.z;

    float w = this->invMass + inertiaInfluence;

    return w;
}

glm::vec3 RigidBody::getVelocityAt(glm::vec3& pos) {	
    glm::vec3 vel = glm::vec3(0.0f);	
    if(this->isDynamic) {
        glm::vec3 r1 = pos - this->pose.p;
        vel = this->vel + glm::cross(this->omega, r1);
    }
    return vel;
}

void RigidBody::applyRotation(glm::vec3 rot, float scale) {

    // safety clamping. This happens very rarely if the solver
    // wants to turn the body by more than 30 degrees in the
    // orders of milliseconds

    const float maxPhi = 0.5;
    const float phi = glm::length(rot);

    if (phi * scale > maxPhi) 
        scale = maxPhi / phi;
        
    glm::quat dq = glm::quat(0.0f, rot.x * scale, rot.y * scale, rot.z * scale);					
    dq = dq * this->pose.q;
    
    this->pose.q = glm::quat(
        this->pose.q.w + 0.5 * dq.w, 
        this->pose.q.x + 0.5 * dq.x,
        this->pose.q.y + 0.5 * dq.y, 
        this->pose.q.z + 0.5 * dq.z
    );
    this->pose.q = glm::normalize(this->pose.q);
}

void RigidBody::integrate(const float &dt) {

    if(this->isDynamic) {

        this->prevPose.p = pose.p;
        this->prevPose.q = pose.q;

        this->forces = glm::vec3(0.0f);
        this->torque = glm::vec3(0.0f);

        // this->applyForceL(glm::vec3(0.0f, this->mass * this->gravity, 0.0f), glm::vec3(0, 0, 0));

        // for (auto force: this->externalForces) {
        //     this->forces += force.force;
        //     this->torque += glm::cross(force.force, (glm::vec3(0.0f) - force.position));
        // }
        // this->externalForces.clear();

        // Euler step
        this->vel += glm::vec3(0, this->gravity, 0) * dt;			
        this->pose.p += this->vel * dt;
        this->applyRotation(this->omega, dt);

        // this->invInertia = this->pose.q * this->invInertia;
        
    }
}

void RigidBody::update(const double &dt) {

    if(this->isDynamic) {
        if(!skipVelocityUpdate) {
            this->vel = (this->pose.p - this->prevPose.p) / (float) dt; 

            glm::quat dq = this->pose.q * glm::conjugate(this->prevPose.q);
            
            this->omega = glm::vec3(dq.x * 2.0 / dt, dq.y * 2.0 / dt, dq.z * 2.0 / dt);

            if (dq.w < 0.0f)
                this->omega = glm::vec3(-this->omega.x, -this->omega.y, -this->omega.z); // @TODO just omega = -omega?

            // Dampening
            this->omega = this->omega * (1.0f - 1.0f * (float) dt);
            this->vel = this->vel * (1.0f - 1.0f * (float) dt);
        }

        this->updateGeometry();
        this->updateCollider();
    }
}

void RigidBody::applyCorrection(glm::vec3 corr, glm::vec3 pos, bool velocityLevel) {
    
    if(this->isDynamic) {
        
        glm::vec3 dq = glm::vec3(0.0f);

        if (glm::length(pos) < 0.00001f) { // @TODO check epsilon
            std::cout << "pos is zero!" << std::endl;
            dq = corr;
        } else {
            if (velocityLevel)
                this->vel += corr * this->invMass;
            else
                this->pose.p += corr * this->invMass;
            dq = pos - this->pose.p;
            dq = glm::cross(dq, corr);
        }
        
        this->pose.invRotate(dq);
        glm::vec3 dq2 = glm::vec3(
            this->invInertia.x * dq.x, 
            this->invInertia.y * dq.y, 
            this->invInertia.z * dq.z
        );
        this->pose.rotate(dq2);
        
        if (velocityLevel)
            // this->omega += 0.0fk;
            this->omega += dq2;
        else 
            this->applyRotation(dq2);
    }
}


void RigidBody::updateGeometry() {
    if(this->mesh) {
        this->mesh->_worldPosMatrixNeedsUpdate = true;
        this->mesh->position = this->pose.p;
        this->mesh->rotation = this->pose.q;
    }
}

void RigidBody::updateCollider() {

    switch(this->collider->colliderType) {
        case ColliderType::Plane :
            auto PC = static_cast<PlaneCollider*>(this->collider);
            PC->normal = this->pose.q * glm::vec3(0.0f, 0.0f, 1.0f); // @TODO store initial up direction of plane collider normal
            
        break;
        
    }
}

void RigidBody::rebuildPrecomputedValues() {
    // if(this->_massNeedsUpdate)
    //     this->_inverseMass = 1 / this->mass;

    // if(this->_rotationNeedsUpdate) {
    //     this->_inverseRotation = glm::inverse(this->rotation);
    // }

    // if(this->_inertiaNeedsUpdate) {
    //     // this->_inertiaTensorW = this->inertiaTensor * glm::mat3_cast(this->rotation); // Seems like world inertia is not needed
    //     this->_inverseInertiaTensor = glm::inverse(this->inertiaTensor);
    //     // this->_inverseInertiaTensorW = glm::inverse(this->_inertiaTensorW); // Seems like world inertia is not needed
    // }

    // this->_rotationNeedsUpdate = false;
    // this->_inertiaNeedsUpdate = false;
    // this->_massNeedsUpdate = false;
}

// // For applyForce, why not directly compute F.net and T?
// void RigidBody::applyForceW(RigidBodyForce force) {
//     this->applyForceL(force.force, CoordinateSystem::worldToLocal(force.position, this->_inverseRotation));
// }

// void RigidBody::applyForceW(glm::vec3 worldForce, glm::vec3 positionW) {
//     this->applyForceL(RigidBodyForce(worldForce, CoordinateSystem::worldToLocal(positionW, this->_inverseRotation)));
// }

// void RigidBody::applyForceL(glm::vec3 worldForce, glm::vec3 positionL) {
//     this->applyForceL(RigidBodyForce(worldForce, positionL));
// }

// void RigidBody::applyForceL(RigidBodyForce force) {
//     this->externalForces.push_back(force);
// }

// void RigidBody::applyLocalImpulse(const glm::vec3& impulse, const glm::vec3& position) {
    
//     std::cout << "applyLocalImpulse is not yet implemented" << std::endl;

// }

// void RigidBody::applyWorldImpulse(const glm::vec3& impulse, const glm::vec3& position) {
    
//     this->velocity += impulse;
    
//     glm::vec3 localImpulse = CoordinateSystem::worldToLocal(impulse, this->_inverseRotation, glm::vec3(0.0f));
//     glm::vec3 localPosition = CoordinateSystem::worldToLocal(position, this->_inverseRotation, this->position);
    
//     this->angularVelocity += this->_inverseInertiaTensor * glm::cross(localImpulse, -localPosition);

// }

// glm::vec3 RigidBody::getAngularVelocityW() {
//     return CoordinateSystem::localToWorld(this->angularVelocity, this->rotation);   
// }

glm::vec3 RigidBody::getPointVelocityL(const glm::vec3& localPoint) {
    return glm::cross(this->omega, localPoint);
}

glm::vec3 RigidBody::getPointVelocityW(glm::vec3 point, bool isPointInLocalSpace) {
    if(!isPointInLocalSpace) {
        // point = CoordinateSystem::worldToLocal(point, this->_inverseRotation, this->position); // @TODO check this?
        point -= this->pose.p;
    }

    return this->vel + CoordinateSystem::localToWorld(this->getPointVelocityL(point), this->pose.q);
}