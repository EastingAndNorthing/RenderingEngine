#pragma once
#include <string>
#include "common.h"
#include "core/Uniforms.h"

class Shader {
public:

    GLuint program = 0;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    Shader(); 

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile = true);

    Shader(const std::string& vertexShaderBasePath, bool autoCompile = true);
    
    GLuint Compile(const std::string& shaderSource, unsigned int type);

    GLuint CreateProgram();

    int getUniformLocation(const std::string &name);

    const void Bind();
    
    const void UnBind();
    
    ~Shader();

private:

};
