#pragma once
#include "../common.h"

class Renderer {
private:

	Renderer();
	void init();

public:
	
	Renderer(Renderer const&) = delete; // Don't copy me :)
  	~Renderer() = default;

	GLFWwindow* window;
    
	static Renderer& instance();

	void log(const char* msg);
	
};
