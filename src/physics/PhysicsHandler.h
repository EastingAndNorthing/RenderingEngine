#pragma once

#include "physics/XPBDSolver.h"
#include "physics/RigidBody.h"
#include "physics/Collider.h"
#include "core/Time.h"

enum SolverType {
    XPBD
};

class PhysicsHandler {
public:

    const SolverType solverType = SolverType::XPBD;

	std::vector<RigidBody*> bodies = {};

	static PhysicsHandler& Instance();
	PhysicsHandler(const PhysicsHandler&) = delete;
    ~PhysicsHandler() = default;

    void Enqueue(RigidBody* body);
    void Enqueue(Mesh* mesh);

    void update();
    
private:
    PhysicsHandler();

};
