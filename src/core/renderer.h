#pragma once

#include <unordered_map>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

class Renderer {
public:
	
	Renderer(Renderer const&) = delete;
  	~Renderer() = default;

	GLFWwindow* window;
    
	static Renderer& Instance();

	void Enqueue(Mesh &mesh);

	void Draw();

	void Clear();

private:

	Renderer();
	
	void Init();

	// std::unordered_map<Mesh, Shader> renderQueue;
	std::vector<Mesh*> renderQueue;

};
