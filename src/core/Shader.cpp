#include <string>
#include "core/Shader.h"
#include "util/Filesystem.h"

Shader::Shader() {}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, bool autoCompile) 
    : vertexShaderPath(vertexShaderPath), fragmentShaderPath(fragmentShaderPath)
{
    if(autoCompile) {
        this->CreateProgram();
    }
}

Shader::~Shader() {
    glDeleteProgram(program);
}

GLuint Shader::Compile(const std::string& shaderSource, unsigned int type) {

    Filesystem &fs = Filesystem::Instance();
    std::string shader_str = fs.getFileContents(shaderSource);

    printf("[INFO] Compiling shader: '%s'...\n", shaderSource.c_str());

    GLuint shaderId = glCreateShader(type);
    const char* src = shader_str.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int compilation_result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilation_result);

    if(compilation_result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        printf("[ERR] Could not compile shader: '%s'. \nOpenGL: %s", shaderSource.c_str(), message);
        
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

GLuint Shader::CreateProgram() {

    GLuint vert = this->Compile(this->vertexShaderPath, GL_VERTEX_SHADER);
    GLuint frag = this->Compile(this->fragmentShaderPath, GL_FRAGMENT_SHADER);

    if(vert == 0 || frag == 0) {
        return 0;
    }

    this->program = glCreateProgram();
    glAttachShader(this->program, vert);
    glAttachShader(this->program, frag);
    glLinkProgram(this->program);
    glValidateProgram(this->program);
    glDeleteShader(vert);
    glDeleteShader(frag);

    int program_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked == GL_FALSE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(this->program, 1024, &log_length, message);
        printf("[ERR] Shader linking error: %s\n", message);
        return 0;
    }

    printf("[SUCCESS] Shader compiled.\n");

    return this->program;
}

void Shader::Bind() {
    glUseProgram(this->program);
}

void Shader::UnBind() {
    glUseProgram(0);
}
