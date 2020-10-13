#include "physics/PhysicsHandler.h"
#include <glm/gtx/norm.hpp>

PhysicsHandler& PhysicsHandler::Instance() {
    static PhysicsHandler instance;
    return instance;
}

PhysicsHandler::PhysicsHandler() {}

void PhysicsHandler::Enqueue(Mesh* mesh) {
    this->Enqueue(new RigidBody(mesh));
}

void PhysicsHandler::Enqueue(RigidBody* body) {
    if(body->mesh->material != NULL) {
        this->bodies.push_back(body);
    } else {
        Log("No material assigned to mesh, skipped.", LogLevel::ERROR);
    }
}

// This entire function should probably be running on a separate thread.
void PhysicsHandler::update() {

    Time &time = Time::Instance();
    
    for (auto& body: this->bodies) {

        // This could be inefficient, checking the same body pairs multiple times. 
        for (auto& otherBody: this->bodies) {
            
            if (!body->isDynamic && !otherBody->isDynamic)
                continue;

            if (body == otherBody)
                continue;

                // if(glm::distance(body->position, otherBody->position) < 2.0f) { // o c t r e e 
                switch(body->collider->colliderType) {

                    case ColliderType::Sphere :
                        switch(otherBody->collider->colliderType) {
                            case ColliderType::Plane :  this->collideSpherePlane(body, otherBody); break;
                            case ColliderType::Sphere : this->collideSphereSphere(body, otherBody); break;
                        }
                    break;

                    case ColliderType::CustomGeometry :
                        for(auto& polygon: otherBody->collider->polygons) {}
                    break;
                }
        }

        body->update(time.time, time.dt);

    }
}

void PhysicsHandler::collideSpherePlane(RigidBody* A, RigidBody* B) {

    auto sC = static_cast<SphereCollider*>(A->collider);
    auto pC = static_cast<PlaneCollider*>(B->collider);

    // if(glm::distance(A->position, B->position) <= sC->radius) { // Need point-to-plane distance
    if((A->position.y - sC->radius) <= B->position.y) {
        this->ElasticCollision(A, B, pC->normal);
    }
}

void PhysicsHandler::collideSphereSphere(RigidBody* A, RigidBody* B) {

    auto sCA = static_cast<SphereCollider*>(A->collider);
    auto sCB = static_cast<SphereCollider*>(B->collider);

    if(glm::distance(A->position, B->position) <= (sCA->radius + sCB->radius) ) {
        glm::vec3 collisionPlane = A->position - B->position;
        this->ElasticCollision(A, B, A->position - B->position);
    }

}

void PhysicsHandler::ElasticCollision(RigidBody* A, RigidBody* B, const glm::vec3 collisionPlane) {

    // https://en.m.wikipedia.org/wiki/Elastic_collision
    glm::vec3 new_v1 = A->velocity;
    glm::vec3 new_v2 = B->velocity;
    
    glm::vec3& v1 = A->velocity;
    glm::vec3& v2 = B->velocity;
    glm::vec3& x1 = A->position;
    glm::vec3& x2 = B->position;

    float& m1 = A->mass;
    float& m2 = B->mass;
    float totalMass = A->mass + B->mass;

    float velocityRatio1 = (B->isDynamic) ? m1/totalMass : 1.0f;
    float velocityRatio2 = (A->isDynamic) ? m2/totalMass : 1.0f;

    glm::vec3 N = glm::normalize(collisionPlane);

    // new_v2 = v2 - (2*m1)/totalMass * glm::dot(v2-v1, x2-x1) / glm::length2(x2-x1) * (x2-x1);
    // new_v1 = v1 - (2*m2)/totalMass * glm::dot(v1-v2, x1-x2) / glm::length2(x1-x2) * (x1-x2);

    new_v1 = v1 - ( 2 * velocityRatio1 * glm::dot(v1-v2, N) / glm::length2(N) * N ); // Reflection of vector along normal
    new_v2 = v2 - ( 2 * velocityRatio2 * glm::dot(v2-v1, -N) / glm::length2(-N) * -N ); // Reflection of vector along normal

    if(A->isDynamic)
        A->velocity = new_v1 * A->bounciness;
    
    if(B->isDynamic)
        B->velocity = new_v2 * B->bounciness;
}

// https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
// https://www.youtube.com/watch?v=EzD7FY62A20
// https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT

// glm::vec3 normalForce = this->externalForces[0].force;
// glm::vec3 perpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->velocity);
// float mu = (glm::length(this->velocity) > 0.1) ? this->dynamicFriction : this->staticFriction;
// this->velocity -= 1000 * mu * normalForce * (float) deltaTime;
