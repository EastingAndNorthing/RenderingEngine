#include <vector>
#include "common.h"
#include "core/Vertex.h"
#include "core/IndexBuffer.h"

/*!
 * @param indices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
IndexBuffer::IndexBuffer(std::vector<unsigned int> &indices, int mode) {

    this->count = indices.size();

    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->count * sizeof(unsigned int), &indices[0], mode);
    
};

IndexBuffer::IndexBuffer() {
    
}

void IndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() {
    return this->count;
}