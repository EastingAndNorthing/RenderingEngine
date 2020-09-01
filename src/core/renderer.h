#pragma once

#include "../common.h"

class Renderer {
public:
	
	Renderer(Renderer const&) = delete;
  	~Renderer() = default;

	GLFWwindow* window;
    
	static Renderer& Instance();

	GLuint CompileShader(const std::string& source, unsigned int type);

	GLuint CompileShaders(const std::string& vertexShader, const std::string& fragmentShader);
	
private:

	Renderer();
	
	void Init();

};
