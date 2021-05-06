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

void RigidBody::applyForce(const glm::vec3& force, const glm::vec3& position) {
    this->force += force;
    this->torque += glm::cross(force, (this->pose.p - position));
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

        // Euler step
        this->vel += glm::vec3(0, this->gravity, 0) * dt;
        this->vel += this->force * this->invMass * dt;
        this->omega += this->torque * this->invInertia * dt;
        this->pose.p += this->vel * dt;
        this->applyRotation(this->omega, dt);
        
    }
}

void RigidBody::update(const double &dt) {

    if(this->isDynamic) {
        // if(!this->hasCollided) {
            this->vel = (this->pose.p - this->prevPose.p) / (float) dt; 

            glm::quat dq = this->pose.q * glm::conjugate(this->prevPose.q);
            
            this->omega = glm::vec3(dq.x * 2.0 / dt, dq.y * 2.0 / dt, dq.z * 2.0 / dt);

            if (dq.w < 0.0f)
                this->omega = glm::vec3(-this->omega.x, -this->omega.y, -this->omega.z); // @TODO just omega = -omega?

            // // Dampening
            // this->vel = this->vel * (1.0f - 1.0f * (float) dt);
            // this->omega = this->omega * (1.0f - 1.0f * (float) dt);
        // }

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
            this->omega += dq2;
        else 
            this->applyRotation(dq2);
    }
}

float RigidBody::getInverseMass(glm::vec3& normal, glm::vec3 pos) {
    
    glm::vec3 n = glm::vec3(0.0f);

    if (glm::length(pos) < 0.0001f) {
        // @TODO check epsilon
        std::cout << "pos is zero" << std::endl;
        n = normal;
    } else {
        n = pos - this->pose.p;
        n = glm::cross(n, normal);
    }

    this->pose.invRotate(n);
    float w = 
        n.x * n.x * this->invInertia.x +
        n.y * n.y * this->invInertia.y +
        n.z * n.z * this->invInertia.z;

    // if (glm::length(pos) > 0.0001f)
    if (glm::length(this->pose.p) > 0.0001f) // Surely this should be the check instead?
        w += this->invMass;

    return w;
}

glm::vec3 RigidBody::getVelocityAt(glm::vec3& pos) {	
    glm::vec3 vel = glm::vec3(0.0f);	
    
    if(this->isDynamic)
        vel = this->vel + glm::cross(this->omega, (pos - this->pose.p));

    return vel;
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
