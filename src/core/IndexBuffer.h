#pragma once

#include <vector>
#include "common.h"
#include "core/Vertex.h"

class IndexBuffer {
public:
	
	IndexBuffer();
	
	IndexBuffer(std::vector<unsigned int> &indices, int mode = GL_STATIC_DRAW);

	~IndexBuffer() = default;

	GLuint ibo;

	void Bind();

	void Unbind();

	unsigned int getCount();

private:

	unsigned int count;

};
