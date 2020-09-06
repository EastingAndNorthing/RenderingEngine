#pragma once

#include <string>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "core/Uniforms.h"

class Material {
public:

    Shader* shader;
    
    std::vector<Uniform*> uniforms;

    Material();

    Material(Shader* shader);

    // Material(Shader&& shader);

    void setShader(Shader* shader);
    
    void setUniform(Uniform &uniform);

    void Bind();

private:

};
