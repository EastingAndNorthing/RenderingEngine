#include <algorithm>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "core/Material.h"
#include "core/Uniforms.h"


Material::Material() {}

Material::Material(Shader* shader, std::vector<Uniform*>uniforms)
    : shader(shader) 
{}

Material::Material(const std::string shaderBasePath, std::vector<Uniform*>uniforms)
{
    this->shader = new Shader(shaderBasePath);
    assert(this->shader != NULL);
    assert(this->shader->program != 0);
}

void Material::setShader(Shader* shader) {
    this->shader = shader;
}

void Material::setUniform(Uniform &uniform) {
    this->shader->Bind();

    if(std::find(this->uniforms.begin(), this->uniforms.end(), &uniform) == this->uniforms.end()) {
        uniform.bindLocation(this->shader->getUniformLocation(uniform.name));
        this->uniforms.push_back(&uniform);
    }

    std::cout << "Material now has " << this->uniforms.size() << " Uniforms attached." << std::endl;
}

void Material::Bind() {

    this->shader->Bind();

    // for (auto& uniform: this->uniforms) does not load the correct bind() method. 'auto' loads the base class instead of derived class... 
    for (int i = 0; i < this->uniforms.size(); i++) {
        this->uniforms[i]->Bind(); 
    }
}
