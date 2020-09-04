#include <vector>
#include "common.h"
#include "core/Vertex.h"

class IndexBuffer {
public:

	IndexBuffer(std::vector<unsigned int> &indices, int mode = GL_STATIC_DRAW);

	~IndexBuffer() = default;

	GLuint ibo;

	GLuint getID();

	void Bind();

	void Unbind();

private:


};
