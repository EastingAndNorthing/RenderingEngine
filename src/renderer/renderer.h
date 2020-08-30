#pragma once
#include "../common.h"

class Renderer {
private:

	Renderer();
	void init();

public:
	
	Renderer(Renderer const&) = delete; // Singleton, disable copy
  	~Renderer() = default;

	GLFWwindow* window;
    
	static Renderer& instance();

	void log();
	
};
