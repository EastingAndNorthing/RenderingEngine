#pragma once

#include <memory>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

class Renderer {
public:
	
	Renderer(const Renderer&) = delete;
  	// ~Renderer() = default;

	~Renderer();

	GLFWwindow* window;
    
	static Renderer& Instance();

	void Enqueue(std::unique_ptr<Mesh> &mesh);

	void Draw();

	void Clear();

private:

	Renderer();
	
	void Init();

	std::vector<std::unique_ptr<Mesh>> renderQueue;

};
