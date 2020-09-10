#pragma once

#include <vector>
#include "../common.h"
#include "core/Vertex.h"

class IndexBuffer {
public:
	GLuint ibuffer = 0;
	std::vector<unsigned int> indices;
	
	IndexBuffer();
	IndexBuffer(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	~IndexBuffer();

	void setIndices(std::vector<unsigned int> indices, int mode = GL_STATIC_DRAW);
	unsigned int getCount();
	unsigned int getSize();
	void Bind();
	void Unbind();

private:
	unsigned int count = 0;
	unsigned int size = 0;
};
