#pragma once

#include <string>
#include <vector>
#include "../common.h"
#include "core/Shader.h"
#include "core/Uniforms.h"

class Material {
public:

    Shader* shader = NULL;
    
    std::vector<Uniform*> uniforms = {};

    Material();

    Material(Shader* shader, std::vector<Uniform*>uniforms = {});

    Material(const std::string shaderBasePath, std::vector<Uniform*>uniforms = {});

    ~Material() = default;

    void setShader(Shader* shader);
    
    void setUniform(Uniform &uniform);

    void Bind();

private:

};
