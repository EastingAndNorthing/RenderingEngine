#include "ContactSet.h"


ContactSet::ContactSet(
    RigidBody* A, 
    RigidBody* B, 
    const glm::vec3& touchPoint,
    const glm::vec3& collisionPlane,
    const glm::vec3& edgeA,
    const glm::vec3& edgeB,
    bool hasContact
) {
    this->A = A;
    this->B = B;
    this->p = touchPoint;
    this->N = collisionPlane;
    this->eA = edgeA;
    this->eB = edgeB;
    this->vf = hasContact;
}

ContactSet::~ContactSet() {}
