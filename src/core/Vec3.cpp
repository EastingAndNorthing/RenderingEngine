#include "Vec3.h"

// http://psgraphics.blogspot.com/2015/02/a-c-vec3-class.html
Vec3::Vec3() { x = y = z = 0; };

Vec3::Vec3(float x, float y, float z)
    : x(x), y(y), z(z) {
};

void Vec3::reset() {
    x = y = z = 0;
};
