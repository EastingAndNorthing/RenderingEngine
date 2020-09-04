#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "primitives/Mesh.h"

Mesh::Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer)
    : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer)
{}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) 
    : vertexBuffer(vertices), indexBuffer(indices)
{}

Mesh::Mesh(std::vector<Vertex> &vertices) 
    : vertexBuffer(vertices)
{}

Mesh::~Mesh() {
}

void Mesh::assignShader(const Shader &shader) {
    this->shader = shader;
}

void Mesh::Bind() {
    this->vertexBuffer.Bind();
    this->indexBuffer.Bind();
    this->shader.Bind();
}
