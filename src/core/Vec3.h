#pragma once

// http://psgraphics.blogspot.com/2015/02/a-c-vec3-class.html
class Vec3 {
public: 
    float x, y, z;

    Vec3();

    Vec3(float x, float y, float z);

    void reset();
};
