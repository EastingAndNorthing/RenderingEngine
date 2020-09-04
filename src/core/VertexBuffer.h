#include <vector>
#include "common.h"
#include "core/Vertex.h"

class VertexBuffer {
public:

	VertexBuffer(std::vector<Vertex> &vertices, int mode = GL_STATIC_DRAW);

	~VertexBuffer() = default;

	GLuint vao;
	GLuint vbuffer;

	GLuint getID();

	void Bind();

	void Unbind();

private:


};
