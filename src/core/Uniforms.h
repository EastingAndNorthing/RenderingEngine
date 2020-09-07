#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../common.h"
#include "core/Vec4.h"

class Uniform {
public:

    std::string name;

    int location;

    Uniform(const std::string& name);
    
    ~Uniform() = default;

    void bindLocation(const int location);

    virtual void Bind();

    void set();

};

class Uniform4f : public Uniform {
public:
    Vec4 values;
    Uniform4f(const std::string& name, const Vec4 &values = {});
    virtual void set(const Vec4 &values = {});
    void Bind() override;
};

class UniformMatrix4f : public Uniform {
public:
    glm::mat4 matrix;
    UniformMatrix4f(const std::string& name, const glm::mat4 &matrix = {});
    virtual void set(const glm::mat4 &matrix = {});
    void Bind() override;
};
