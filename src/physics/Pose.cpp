#include "Pose.h"

Pose::Pose(glm::vec3 p, glm::quat q)
    :p(p), q(q) {}

void Pose::copy(Pose pose) {
    // this.p.copy(pose.p);
    // this.q.copy(pose.q);
    this->p = pose.p;
    this->q = pose.q;
}

Pose Pose::clone() {
    // var newPose = new Pose();
    // newPose.p = this.p.clone();
    // newPose.q = this.q.clone();
    Pose newPose = Pose(this->p, this->q);
    return newPose;
}

void Pose::translate(glm::vec3& v) {
    // v.add(this.p);
    v += this->p;
}

void Pose::invTranslate(glm::vec3& v) {
    // v.sub(this.p);
    v -= this->p;
}

void Pose::rotate(glm::vec3& v) {
    // v.applyQuaternion(this.q);
    v = v * this->q;
}

void Pose::invRotate(glm::vec3& v) {
    // let inv = this.q.clone();
    // inv.conjugate();
    // v.applyQuaternion(inv);
    v = v * glm::conjugate(this->q);
}

void Pose::transform(glm::vec3& v) {
    // v.applyQuaternion(this.q);
    // v.add(this.p);
    this->rotate(v);
    this->translate(v);
}

void Pose::invTransform(glm::vec3& v) {
    // v.sub(this.p);
    // this.invRotate(v);
    this->invTranslate(v);
    this->invRotate(v);
}

void Pose::transformPose(Pose& pose) {
    // pose.q.multiplyQuaternions(this.q, pose.q);
    // this.rotate(pose.p);
    // pose.p.add(this.p);
    pose.q = this->q * pose.q;
    pose.p = pose.p * this->q;
    pose.p = pose.p + this->p;
}
