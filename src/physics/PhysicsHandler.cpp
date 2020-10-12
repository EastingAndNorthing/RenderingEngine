#include "physics/PhysicsHandler.h"

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
                switch(otherBody->collider->colliderType) {
                    case ColliderType::Plane :

                        if(body->collider->colliderType == ColliderType::Sphere)
                            this->collideSpherePlane(body, otherBody);

                    case ColliderType::CustomGeometry :
                        
                        for(auto& polygon: otherBody->collider->polygons) {
                            
                        }

                    break;
                }
            // }
        }

        body->update(time.time, time.dt);

    }
}

void PhysicsHandler::collideSpherePlane(RigidBody* sphereBody, RigidBody* planeBody) {

    auto sphere = static_cast<SphereCollider*>(sphereBody->collider);
    auto plane = static_cast<PlaneCollider*>(planeBody->collider);

    if((sphereBody->position.y - sphere->radius) <= planeBody->position.y) {
        sphereBody->position.y = sphere->radius;
        sphereBody->velocity.y *= -sphereBody->bounciness;
        sphereBody->acceleration.y = 0;
    }
}

// glm::vec3 normalForce = this->externalForces[0].force;
// glm::vec3 perpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->velocity);
// float mu = (glm::length(this->velocity) > 0.1) ? this->dynamicFriction : this->staticFriction;
// this->velocity -= 1000 * mu * normalForce * (float) deltaTime;
