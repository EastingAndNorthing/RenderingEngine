#pragma once

class Vec2 {
public: 
    double x = 0;
    double y = 0;

    Vec2();

    Vec2(double x, double y);

    ~Vec2() = default;

    double magnitude();
    
    void normalize();

};
