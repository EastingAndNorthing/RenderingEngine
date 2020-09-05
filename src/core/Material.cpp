#include "common.h"
#include "core/Shader.h"
#include "core/Material.h"
#include "core/Uniforms.h"


Material::Material() {}

// Material::Material(Shader &shader)
//     : shader(shader) 
// {}

Material::Material(Shader shader)
    : shader(shader) 
{}

void Material::setShader(Shader &shader) {
    this->shader = shader;
}

void Material::setUniform(Uniform &uniform) {
    this->shader.Bind();
    uniform.bindLocation(this->shader.getUniformLocation(uniform.name));
    this->uniforms.push_back(&uniform);
}

void Material::Bind() {
    this->shader.Bind();
    
    for (auto& uniform: this->uniforms) {
        uniform->Bind();
    }
}
