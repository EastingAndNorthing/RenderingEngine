#include <vector>
#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

/*!
 * @param vertices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
VertexBuffer::VertexBuffer(std::vector<Vertex> &vertices, int mode) {
    
    // Generate and bind a vertex array object. This makes a connection between a buffer and a vertex layout (attribute locations)
    // @TODO Benchmark if using separate vao's is less efficient than having 1 global vao. 
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], mode);
    
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