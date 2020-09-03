#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

/*!
 * @param vertices
 * @param size Should be sizeof(vertices)
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
VertexBuffer::VertexBuffer(const void* vertices, unsigned int size, int mode) {
    
    // Generate and bind a vertex array object. This makes a connection between a buffer and a vertex layout (attribute locations)
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, mode);
    
};

void VertexBuffer::Bind() {
    glBindVertexArray(this->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vao);
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VertexBuffer::getID() {
    return this->vao;
}