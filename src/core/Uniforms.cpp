#include <iostream>
#include "common.h"
#include "core/Uniforms.h"
#include "core/Vec3.h"
#include "core/Vec4.h"

Uniform::Uniform(const std::string& name) : name(name) {}

void Uniform::bindLocation(const int location) {
    this->location = location;
}

void Uniform::Bind() {}

// Uniform3f ////////////////////////////////////////////////////////////
Uniform3f::Uniform3f(const std::string& name, const Vec3 &values) : Uniform(name), values(values) {}

void Uniform3f::Bind() {
    glUniform3f(this->location, this->values.x, this->values.y, this->values.z);
}

void Uniform3f::set(const Vec3 &values) {
    this->values = values;
    this->Bind();
}

// Uniform4f ////////////////////////////////////////////////////////////
Uniform4f::Uniform4f(const std::string& name, const Vec4 &values) : Uniform(name), values(values) {}

void Uniform4f::Bind() {
    glUniform4f(this->location, this->values.x, this->values.y, this->values.z, this->values.w);
}

void Uniform4f::set(const Vec4 &values) {
    this->values = values;
    this->Bind();
}

// UniformMatrix4f ////////////////////////////////////////////////////////
UniformMatrix4f::UniformMatrix4f(const std::string& name, const glm::mat4 &matrix) : Uniform(name), matrix(matrix) {}

void UniformMatrix4f::Bind() {
    glUniformMatrix4fv(this->location, 1, GL_FALSE, &this->matrix[0][0]);
}

void UniformMatrix4f::set(const glm::mat4 &matrix) {
    this->matrix = matrix;
    this->Bind();
}
