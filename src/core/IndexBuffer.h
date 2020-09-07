#pragma once

#include <vector>
#include "../common.h"
#include "core/Vertex.h"

class IndexBuffer {
public:
	
	GLuint ibo = 0;
	
	IndexBuffer();
	
	IndexBuffer(std::vector<unsigned int> &indices, int mode = GL_STATIC_DRAW);

	~IndexBuffer();

	void setIndices(std::vector<unsigned int> &indices, int mode = GL_STATIC_DRAW);

	void Bind();

	void Unbind();

	unsigned int getCount();

private:

	unsigned int count = 0;

};
