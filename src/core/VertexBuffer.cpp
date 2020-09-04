#include <vector>
#include "common.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

/*!
 * @param vertices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
VertexBuffer::VertexBuffer(std::vector<Vertex> &vertices, int mode) {

    this->size = vertices.size() * sizeof(Vertex);

    std::cout << vertices.size() * sizeof(Vertex) << std::endl;
    
    // Generate and bind a vertex array object. This makes a connection between a buffer and a vertex layout (attribute locations)
    // @TODO Benchmark if using separate vao's is less efficient than having 1 global vao. 
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->size, &vertices[0], mode);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
};

void VertexBuffer::Bind() {
    glBindVertexArray(this->vao); 
    glBindBuffer(GL_ARRAY_BUFFER, this->vao);
}

void VertexBuffer::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getSize() {
    return this->size;
}