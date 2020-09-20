#include <algorithm>
#include "common.h"
#include "core/Shader.h"
#include "core/Material.h"
#include "core/Uniforms.h"

Material::Material() {}

Material::Material(Shader* shader, std::vector<Uniform*> uniforms)
    : shader(shader)
{
    for (int i = 0; i < uniforms.size(); i++)
        this->setUniform(uniforms[i]);
}

Material::Material(const std::string shaderBasePath, std::vector<Uniform*> uniforms)
{
    this->shader = new Shader(shaderBasePath);
    assert(this->shader != NULL);
    assert(this->shader->program != 0);

    for (int i = 0; i < uniforms.size(); i++)
        this->setUniform(uniforms[i]);
}

void Material::setShader(Shader* shader) {
    this->shader = shader;
}

void Material::setUniform(Uniform* uniform) {
    this->shader->Bind();

    if(this->uniforms.find(uniform->name) == this->uniforms.end()) {
        uniforms[uniform->name] = uniform;
    }
}

void Material::Bind() {

    this->shader->Bind();

    for (const auto & [ key, uniform ] : this->uniforms) {
        uniform->bindLocation(this->shader->getUniformLocation(uniform->name));
        uniform->Bind();
    }
}
