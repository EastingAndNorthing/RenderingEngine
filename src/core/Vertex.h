#pragma once

#include "core/Vec3.h"
#include "core/Vec4.h"

struct Vertex {
    
    Vec3 position = { 0, 0, 0 };
    Vec3 normal   = { 0, 0, 0 };
    Vec4 color    = { 0, 0, 0, 0 };

    Vertex();
    Vertex(Vec3 position = {}, Vec3 normal = {}, Vec4 color = {});
    Vertex(float x = 0, float y = 0, float z = 0);
	~Vertex() = default;

    static void setAttribPointers();

};