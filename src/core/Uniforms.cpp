#include <iostream>
#include "common.h"
#include "core/Uniforms.h"
#include "core/Vec4.h"

Uniform::Uniform(const std::string& name) {}

void Uniform::bindLocation(const int location) {
    this->location = location;
    assert(location != -1);
}

void Uniform::Bind() {
}

Uniform4f::Uniform4f(const std::string& name, const Vec4 &values) : Uniform(name), values(values) {}

void Uniform4f::set(const Vec4 &values) {
    this->values = values;
    this->Bind();
}

void Uniform4f::Bind() {
    glUniform4f(this->location, values.x, values.y, values.z, values.w);
}
