#pragma once

#include "../common.h"
#include "core/Vec3.h"
#include "core/Vec4.h"

class IUniform {
public:

    std::string name;
    int location = -1;
    int value;

    IUniform(const std::string& name)
        : name(name) {};

    virtual ~IUniform() = default;

    virtual void Bind() {};

protected:
    bool _update = true;
};

template <typename T>
class Uniform : public IUniform {
public:

    Uniform(const std::string& name, const T &value = {}) 
        : IUniform(name), value(value) {}

    virtual void Bind();

    virtual void set(T value) { 
        this->_update = true; 
        this->value = value;
    }

private:
    T value;

};
