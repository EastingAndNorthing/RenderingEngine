#include <vector>
#include "common.h"
#include "core/Vertex.h"
#include "core/IndexBuffer.h"

IndexBuffer::IndexBuffer()
    : count(0) 
{
    this->count = 0;
}

/*!
 * @param indices
 * @param mode GL_STATIC_DRAW or GL_DYNAMIC_DRAW. See glBufferData() for more options.
 */
IndexBuffer::IndexBuffer(std::vector<unsigned int> &indices, int mode) {
    this->setIndices(indices, mode);
};

void IndexBuffer::setIndices(std::vector<unsigned int> &indices, int mode) {

    this->count = indices.size();
    this->size = this->count * sizeof(unsigned int);

    glGenBuffers(1, &this->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getSize(), &indices[0], mode);

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

unsigned int IndexBuffer::getSize() {
    return this->size;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &this->ibo);
}
