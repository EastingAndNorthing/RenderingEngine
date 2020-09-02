#include "common.h"
#include "core/Vertex.h"

class VertexBuffer {
public:

	VertexBuffer(const void* vertices, unsigned int size, int mode = GL_STATIC_DRAW);

	~VertexBuffer() = default;

	GLuint m_vertexArrayObj;

private:


};
