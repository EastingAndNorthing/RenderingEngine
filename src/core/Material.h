#pragma once

#include <unordered_map>
#include "../common.h"
#include "core/Shader.h"
#include "core/Uniforms.h"

class Material {
public:
    Shader* shader = NULL;
    
    std::unordered_map<std::string, IUniform*> uniforms = {};

    Material();
    Material(const std::string shaderBasePath, std::vector<IUniform*> uniforms = {});
    ~Material() = default;

    void setShader(Shader* shader);

    void setUniform(IUniform* uniform);

    // template <typename T = int>
    // void set(std::string name, T value);

    void Bind();
};
