#pragma once

class Vec4 {
public: 
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;

    Vec4();
    
    Vec4(float x, float y, float z, float w);
	
    ~Vec4() = default;

};
