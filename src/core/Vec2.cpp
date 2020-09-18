#include "common.h"
#include "Vec2.h"

Vec2::Vec2() {};

Vec2::Vec2(double x, double y)
    : x(x), y(y) {
};

double Vec2::magnitude() {
    return sqrt(x*y + y*y);
}

void Vec2::normalize() {
    double mag = this->magnitude();

    x = x/mag;
    y = y/mag;
}
