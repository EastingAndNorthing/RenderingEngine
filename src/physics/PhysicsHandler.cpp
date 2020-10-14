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

        // @TODO Chunking / octree, prevent checking body pairs multiple times
        for (auto& otherBody: this->bodies) {
            
            if (!body->isDynamic && !otherBody->isDynamic)
                continue;

            if (body == otherBody)
                continue;

            // @TODO Bounding box proximity check
            // if(body->boundingBox.intersects(otherBody.boundingBox)) {}

            switch(body->collider->colliderType) {

                case ColliderType::Sphere :
                    switch(otherBody->collider->colliderType) {
                        case ColliderType::Plane  : this->collide_SPHERE_PLANE(body, otherBody); break;
                        case ColliderType::Sphere : this->collide_SPHERE_SPHERE(body, otherBody); break;
                    }
                break;

                case ColliderType::ConvexMesh :
                    switch(otherBody->collider->colliderType) {
                        case ColliderType::Plane  : this->collide_MESH_PLANE(body, otherBody); break;
                        // case ColliderType::Sphere : this->collide_MESH_MESH(body, otherBody); break;
                    }

                break;
            }
        }

        body->updatePhysics(time.dt);

    }
}

float PhysicsHandler::getPointToPlaneDistance(const glm::vec3& pointPos, const glm::vec3& planePos, const glm::vec3& planeNormal) {
    return glm::dot(planeNormal, (pointPos - planePos));
}

void PhysicsHandler::collide_SPHERE_PLANE(RigidBody* A, RigidBody* B) {

    auto SC = static_cast<SphereCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    float signedDistance = this->getPointToPlaneDistance(A->position, B->position, PC->normal) - SC->radius;

    if(signedDistance <= this->minCollisionDistance) {
        this->elasticCollision(A, B, PC->normal);
        A->position += PC->normal * (abs(signedDistance) + this->afterCollisionDistance + this->minCollisionDistance); // Set position to just outside of collision plane
    }
}

void PhysicsHandler::collide_SPHERE_SPHERE(RigidBody* A, RigidBody* B) {

    auto SCA = static_cast<SphereCollider*>(A->collider);
    auto SCB = static_cast<SphereCollider*>(B->collider);

    float signedDistance = glm::distance(A->position, B->position) - SCA->radius - SCB->radius;

    if(signedDistance <= this->minCollisionDistance) {
        glm::vec3 N = (A->position - B->position);
        this->elasticCollision(A, B, N);
        A->position += N * (abs(signedDistance) + this->afterCollisionDistance + this->minCollisionDistance); // Set position to just outside of collision plane
    }

}

void PhysicsHandler::collide_MESH_PLANE(RigidBody* A, RigidBody* B) {
    // https://en.m.wikipedia.org/wiki/Hyperplane_separation_theorem
    // https://unitylist.com/p/5uc/Unity-Separating-Axis-SAT
    // https://www.youtube.com/watch?v=EzD7FY62A20

    auto MC = static_cast<MeshCollider*>(A->collider);
    auto PC = static_cast<PlaneCollider*>(B->collider);

    for(auto& polygon: MC->polygons) {
        for(int i = 0; i < 2; i++) {
            // glm::vec3 A = glm::vec3(polygon.vertices[i].position);
            // glm::vec3 A = glm::vec3(polygon.vertices[i].position);
            // glm::vec3 A = glm::vec3(polygon.vertices[i].position);
        }
    }

}

void PhysicsHandler::elasticCollision(RigidBody* A, RigidBody* B, const glm::vec3 collisionPlane) {

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

    // @TODO: when one of the bodies is static, all velocity should go to the dynamic body
    float velocityRatio1 = m2/totalMass;
    float velocityRatio2 = m1/totalMass;

    glm::vec3 N = glm::normalize(collisionPlane);

    float combinedCOR = (A->bounciness + B->bounciness) / 2.0f; // https://en.m.wikipedia.org/wiki/Coefficient_of_restitution

    if(A->isDynamic) {
        new_v1 = v1 - ( velocityRatio1 * 2.0f * glm::dot( v1-v2,  N) / glm::length2( N) *  N ); // Reflection of vector along collision normal
        A->velocity = new_v1 * combinedCOR;
    }
    
    if(B->isDynamic) {
        new_v2 = v2 - ( velocityRatio2 * 2.0f * glm::dot( v2-v1, -N) / glm::length2(-N) * -N ); // Reflection of vector along collision normal
        B->velocity = new_v2 * combinedCOR;
    }
}

// glm::vec3 normalForce = this->externalForces[0].force;
// glm::vec3 perpendicular = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->velocity);
// float mu = (glm::length(this->velocity) > 0.1) ? this->dynamicFriction : this->staticFriction;
// this->velocity -= 1000 * mu * normalForce * (float) deltaTime;
