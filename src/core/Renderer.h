#pragma once

#include "../common.h"
#include "core/Shader.h"
#include "core/Camera.h"
#include "primitives/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer {
public:
	GLFWwindow* window;
	Camera* camera;

	int windowWidth = 0;
	int windowHeight = 0;
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	static Renderer& Instance();

	Renderer(const Renderer&) = delete;
	~Renderer();
	
	bool isActive();
	void SetupFramebuffer();
	void Enqueue(Mesh* mesh);
	void BeginLoop();
	void EndLoop();
	void Clear();

private:
	std::vector<Mesh*> meshQueue = {};
	Renderer();

	void DrawMeshes();
	
};
