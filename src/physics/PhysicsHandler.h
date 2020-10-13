#pragma once

#include "physics/RigidBody.h"
#include "core/Time.h"

class PhysicsHandler {
public:

    float minCollisionDistance = 0.001f;

	static PhysicsHandler& Instance();
	PhysicsHandler(const PhysicsHandler&) = delete;
    ~PhysicsHandler() = default;

    void Enqueue(RigidBody* body);
    void Enqueue(Mesh* mesh);

    void update();

	std::vector<RigidBody*> bodies = {};
    
private:
    PhysicsHandler();

    void collideSpherePlane(RigidBody* sphere, RigidBody* plane);
    void collideSphereSphere(RigidBody* A, RigidBody* B);

    void ElasticCollision(RigidBody* A, RigidBody* B, const glm::vec3 collisionPlane = glm::vec3(0.0f));

};
