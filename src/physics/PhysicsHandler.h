#pragma once

#include "physics/RigidBody.h"
#include "core/Time.h"

class PhysicsHandler {
public:

	static PhysicsHandler& Instance();
	PhysicsHandler(const PhysicsHandler&) = delete;
    ~PhysicsHandler() = default;

    void Enqueue(RigidBody* body);
    void Enqueue(Mesh* mesh);

    void update();

	std::vector<RigidBody*> bodies = {};
    
private:
    PhysicsHandler();

    void handleCollisions(RigidBody* body);

};
