#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* vertices, unsigned int size, int mode) {
    glGenVertexArrays(1, &m_vertexArrayObj);
    glBindVertexArray(m_vertexArrayObj);
    glGenBuffers(1, &m_vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayObj);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, mode);
};
