#pragma once

#include <string>
#include "core/Vec4.h"

class Uniform {
public:

    std::string name;

    int location;

    Uniform(const std::string& name);
    
    ~Uniform() = default;

    void bindLocation(const int location);

    void set();
    
    virtual void Bind();

};

class Uniform4f : public Uniform {
public:

    Vec4 values;

    Uniform4f(const std::string& name, const Vec4 &values = {});

    virtual void set(const Vec4 &values = {});

    void Bind() override;

};
