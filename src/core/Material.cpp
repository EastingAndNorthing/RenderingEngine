#include "common.h"
#include "core/Shader.h"
#include "core/Material.h"
#include "core/Uniforms.h"


Material::Material() {}

// Material::Material(Shader &shader)
//     : shader(shader) 
// {}

Material::Material(Shader* shader)
    : shader(shader) 
{}

// Material::Material(Shader&& shader)
//     : shader(shader) 
// {
//     this->shader = shader;
//     std::cout << "Material with Shader in constructor, GL program ID = " << this->shader.program << std::endl;
// }

void Material::setShader(Shader* shader) {
    this->shader = shader;
}

void Material::setUniform(Uniform &uniform) {
    this->shader->Bind();
    uniform.bindLocation(this->shader->getUniformLocation(uniform.name));
    this->uniforms.push_back(&uniform);
    // std::cout << "Material now has " << this->uniforms.size() << " Uniforms attached." << std::endl;
}

void Material::Bind() {

    this->shader->Bind();

    // for (auto& uniform: this->uniforms) does not load the correct bind() method. 'auto' loads the base class instead of derived class... 
    for (int i = 0; i < this->uniforms.size(); i++) {
        this->uniforms[i]->Bind(); 
    }
}
