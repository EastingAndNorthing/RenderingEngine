#include "ContactSet.h"

ContactSet::ContactSet(
    RigidBody* A, 
    RigidBody* B, 
    const glm::vec3& touchPoint,
    const glm::vec3& collisionPlane,
    const float& signedDistance,
    const float compliance
) {
    this->A = A;
    this->B = B;
    this->P = touchPoint;
    this->N = collisionPlane;
    this->signedDistance = signedDistance;
    this->compliance = compliance;
}

ContactSet::~ContactSet() {}
