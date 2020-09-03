#pragma once

#include "common.h"

class Renderer {
public:
	
	Renderer(Renderer const&) = delete;
  	~Renderer() = default;

	GLFWwindow* window;
    
	static Renderer& Instance();

	GLuint CompileShader(const std::string& shaderSource, unsigned int type);
	
	GLuint CompileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

private:

	Renderer();
	
	void Init();

};
