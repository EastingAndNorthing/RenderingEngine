#pragma once

// #include <memory>
#include <vector>
#include "../common.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
public:
	GLFWwindow* window;

	int windowWidth = 0;
	int windowHeight = 0;
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	glm::mat4 projectionMatrix = glm::mat4(1.0f);

	static Renderer& Instance();

	Renderer(const Renderer&) = delete;
	~Renderer();

	void setupFramebuffer();
	void Enqueue(Mesh* mesh);
	void Draw();
	void Clear();

private:
	std::vector<Mesh*> renderQueue = {};
	Renderer();
	
};
