#pragma once
#include <string>
#include "common.h"

class Shader {
public:

    GLuint program = 0;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile = true);

    GLuint Compile(const std::string& shaderSource, unsigned int type);

    GLuint CreateProgram();

    GLuint getID();

    void Bind();
    
    void UnBind();
    
    ~Shader();

private:

};