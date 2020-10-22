#pragma once

#include <unordered_map>
#include "../common.h"
#include "util/Filesystem.h"
#include "core/Shader.h"
#include "core/Uniforms.h"
#include "core/Camera.h"
#include "primitives/Mesh.h"
#include "primitives/PrimitiveGenerator.h"
#include "input/WindowEventHandler.h"

class Renderer {
public:
	GLFWwindow* window = NULL;
	Camera* camera = NULL;
	Mesh* debugVector = NULL;
    Material* defaultShader = NULL;

	int windowWidth = 0;
	int windowHeight = 0;
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	static Renderer& Instance();

	Renderer(const Renderer&) = delete;
	~Renderer();
	
	bool isActive();
	void SetupFramebuffer(int width, int height);
	
	void Enqueue(Mesh* mesh);
	void BeginLoop();
	void EndLoop();
	void Clear();

	Camera* getCamera();

private:
	std::vector<Mesh*> meshQueue = {};
	std::vector<Mesh*> overlayQueue = {};
	Renderer();

	void DrawMeshes();
	void DrawOverlays();
	
};

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
