#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

VertexBuffer::VertexBuffer() {
    
}

/*!
 * @param vertices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
VertexBuffer::VertexBuffer(std::vector<Vertex> vertices, int mode) {
    this->setVertices(vertices, mode);
};

void VertexBuffer::setVertices(std::vector<Vertex> vertices, int mode) {

    this->vertices = vertices;
    this->count = vertices.size();
    this->size = this->count * sizeof(Vertex);
    
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->size, &vertices[0], mode);

    Vertex::setAttribPointers();
}

void VertexBuffer::Bind() {
    glBindVertexArray(this->vao); 
    // glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer); ////// uhhhhhhm, not using this->vbuffer? 
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int VertexBuffer::getCount() {
    return this->count;
}

unsigned int VertexBuffer::getSize() {
    return this->size;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &this->vbuffer);
    glDeleteBuffers(1, &this->vao);
}