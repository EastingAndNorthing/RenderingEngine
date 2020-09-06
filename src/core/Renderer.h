#pragma once

// #include <memory>
#include <vector>
#include "common.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
public:

	int frameBufferWidth = 0;
	int frameBufferHeight = 0;
	
	GLFWwindow* window;

	glm::mat4 u_mvp = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

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
