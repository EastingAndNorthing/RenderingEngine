#pragma once

#include <vector>
#include "common.h"
#include "core/Vertex.h"

class VertexBuffer {
public:

	VertexBuffer(std::vector<Vertex> &vertices, int mode = GL_STATIC_DRAW);

	~VertexBuffer() = default;

	GLuint vao;
	GLuint vbuffer;

	void Bind();

	void Unbind();

	unsigned int getSize();

private:

	unsigned int size;

};
