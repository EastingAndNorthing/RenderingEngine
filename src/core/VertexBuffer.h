#pragma once

#include <vector>
#include "../common.h"
#include "core/Vertex.h"

class VertexBuffer {
public:
	GLuint vao = 0;
	GLuint vbuffer = 0;

	VertexBuffer();
	VertexBuffer(std::vector<Vertex> &vertices, int mode = GL_STATIC_DRAW);
	~VertexBuffer();

	void setVertices(std::vector<Vertex> &vertices, int mode = GL_STATIC_DRAW);
	unsigned int getCount();
	unsigned int getSize();
	void Bind();
	void Unbind();

private:
	unsigned int count = 0;
	unsigned int size = 0;
};
