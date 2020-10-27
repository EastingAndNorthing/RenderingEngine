#pragma once

#include "../common.h"
#include "core/Shader.h"
#include "core/Uniforms.h"

class Material {
public:
    Shader* shader = NULL;
    
    std::unordered_map<std::string, IUniform*> uniforms = {};

    Material();
    Material(const std::string& shaderBasePath, std::vector<IUniform*> uniforms = {});
    ~Material() = default;

    void setShader(Shader* shader);

    template <typename T = int>
    void setUniform(const std::string& name, T value) {
        if (uniforms.find(name) == uniforms.end()) {
            this->assignUniform(new Uniform<T>(name, value));
        } else {
            static_cast<Uniform<T>*>(this->uniforms.at(name))->set(value);
        }
    }

    template <typename T = int>
    Uniform<T>* getUniform(const std::string& name) {
        return static_cast<Uniform<T>*>(this->uniforms.at(name));
    }

    void Bind();

    void assignUniform(IUniform* uniform);
private:
};
