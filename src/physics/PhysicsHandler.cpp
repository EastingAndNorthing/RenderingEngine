#include "physics/PhysicsHandler.h"
#include "physics/PhysicsSolver.h"

PhysicsHandler& PhysicsHandler::Instance() {
    static PhysicsHandler instance;
    return instance;
}

PhysicsHandler::PhysicsHandler() {}

void PhysicsHandler::Enqueue(Mesh* mesh) {
    this->Enqueue(new RigidBody(mesh));
}

void PhysicsHandler::Enqueue(RigidBody* body) {
    this->bodies.push_back(body);
}

// This entire function should probably be running on a separate thread.
// https://medium.com/@cancerian0684/singleton-design-pattern-and-how-to-make-it-thread-safe-b207c0e7e368
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
                        case ColliderType::Plane      : PhysicsSolver::collide_SPHERE_PLANE(body, otherBody);   break;
                        case ColliderType::Sphere     : PhysicsSolver::collide_SPHERE_SPHERE(body, otherBody);  break;
                        case ColliderType::ConvexMesh : PhysicsSolver::collide_SPHERE_MESH(body, otherBody);    break;
                    }
                break;

                case ColliderType::ConvexMesh :
                    switch(otherBody->collider->colliderType) {
                        case ColliderType::Plane      : PhysicsSolver::collide_MESH_PLANE(body, otherBody);     break;
                    }

                break;
            }
        }

        body->updatePhysics(time.dt);

    }
}
