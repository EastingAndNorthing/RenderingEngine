#include "ContactSet.h"


ContactSet::ContactSet(
    RigidBody* A, 
    RigidBody* B, 
    const glm::vec3& touchPoint,
    const glm::vec3& collisionPlane,
    const float& signedDistance
) {
    this->A = A;
    this->B = B;
    this->p = touchPoint;
    this->N = collisionPlane;
    this->d = signedDistance;
}

ContactSet::~ContactSet() {}
