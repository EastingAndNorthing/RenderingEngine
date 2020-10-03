#pragma once

#include "physics/RigidBody.h"
#include "core/Time.h"

class PhysicsHandler {
public:

    float g = -9.81;

	static PhysicsHandler& Instance();
	PhysicsHandler(const PhysicsHandler&) = delete;
    ~PhysicsHandler() = default;

    void Enqueue(RigidBody* body);
    void Enqueue(Mesh* mesh);

    void update();

private:
    PhysicsHandler();

	std::vector<RigidBody*> bodies = {};
};
