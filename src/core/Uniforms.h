#pragma once

#include <string>
#include "../common.h"
#include "core/Vec3.h"
#include "core/Vec4.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

class Uniform1i : public Uniform {
public:
    int value;

    Uniform1i(const std::string& name, const int &value = 0);

    virtual void set(const int &value = 0);
    void Bind() override;
};

class Uniform1f : public Uniform {
public:
    float value;

    Uniform1f(const std::string& name, const float &value = 0.0f);

    virtual void set(const float &value = 0.0f);
    void Bind() override;
};

class Uniform3f : public Uniform {
public:
    Vec3 values;

    Uniform3f(const std::string& name, const Vec3 &values = {});

    virtual void set(const Vec3 &values = {});
    void Bind() override;
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
