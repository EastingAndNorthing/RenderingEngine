#pragma once

#include <vector>
#include "common.h"
#include "core/Vertex.h"

class VertexBuffer {
public:

	GLuint vao;

	GLuint vbuffer;

	VertexBuffer(std::vector<Vertex> &vertices, int mode = GL_STATIC_DRAW);

	~VertexBuffer();

	void Bind();

	void Unbind();

	unsigned int getSize();

private:

	unsigned int size;

};
