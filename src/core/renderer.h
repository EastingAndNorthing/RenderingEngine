#pragma once

// #include <memory>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

class Renderer {
public:
	
	GLFWwindow* window;
	
	static Renderer& Instance();
	
	Renderer(const Renderer&) = delete;
  	// ~Renderer() = default;

	~Renderer();

	// void Enqueue(std::unique_ptr<Mesh> &mesh);
	void Enqueue(Mesh* mesh);

	void Draw();

	void Clear();

private:

	// std::vector<std::unique_ptr<Mesh>> renderQueue;
	std::vector<Mesh*> renderQueue = {};
	
	Renderer();
	
	void Init();

};
