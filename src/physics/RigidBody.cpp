#include "RigidBody.h"

RigidBody::RigidBody() {}

RigidBody::RigidBody(Mesh* mesh) {
    this->mesh = mesh;
    this->mesh->managedByRigidBody = true;

    this->position = mesh->position;
    // this->rotation = mesh->rotation;
}

void RigidBody::update(double &time, double &deltaTime, float &gravity) {

    this->acceleration.y = gravity;

    this->velocity += this->acceleration * (float) deltaTime;
    glm::vec3 newPos = this->position + this->velocity * (float) deltaTime;

    if((newPos.y - this->boundingBox.y/2) < 0) {
        this->acceleration.y = 0;
        this->velocity.y = 0;
        this->position.y = this->boundingBox.y/2;
        this->updateGeometry();
    } else {
        this->position = newPos;
    }

    if(glm::length(this->velocity) >= 0.000001) {
        this->updateGeometry();
    }

}

void RigidBody::updateGeometry() {
    this->mesh->_worldPosMatrixNeedsUpdate = true;
    this->mesh->position = this->position;
//  this->mesh->rotation = this->rotation;
}
