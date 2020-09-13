#pragma once

#include <unordered_map>
#include <string>
#include "../common.h"
#include "core/Uniforms.h"

class Shader {
public:
    GLuint program = 0;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    int u_mvp_location = -1;

    Shader(); 
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile = true);
    Shader(const std::string& vertexShaderBasePath, bool autoCompile = true);
    ~Shader();
    
    GLuint Compile(const std::string& shaderSource, unsigned int type);
    GLuint CreateProgram();

    int getUniformLocation(const std::string &name);
    // int getProjectionMatrixLocation();
    
    const void Bind();
    const void UnBind();

private:
    std::unordered_map<std::string, int> uniformLocationCache = {};
};
