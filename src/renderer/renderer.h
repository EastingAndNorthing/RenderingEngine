#pragma once
#include "../common.h"

class Renderer
{
public:
	
	Renderer();
	
	Renderer(Renderer const&) = delete; // Disallow copying
	
	~Renderer();

	GLFWwindow* get_window() const;

private:

	GLFWwindow* window;

	void init();
	
};
