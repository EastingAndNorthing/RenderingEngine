#include <algorithm>
#include "common.h"
#include "core/Material.h"

Material::Material() {}

Material::Material(const std::string shaderBasePath, std::vector<IUniform*> uniforms) {
    this->shader = new Shader(shaderBasePath);
    assert(this->shader != NULL);
    assert(this->shader->program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        this->assignUniform(uniforms[i]);
}

void Material::setShader(Shader* shader) {
    this->shader = shader;
}

void Material::assignUniform(IUniform* uniform) {
    if(this->uniforms.find(uniform->name) == this->uniforms.end()) {
        uniform->location = this->shader->getUniformLocation(uniform->name);
        uniforms[uniform->name] = uniform;
    }
}

void Material::Bind() {
    this->shader->Bind();
    for (const auto &nameUniformPair : this->uniforms) {
        nameUniformPair.second->Bind();
    }

}
