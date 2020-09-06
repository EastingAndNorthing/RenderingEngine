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
    : vertexBuffer(vertexBuffer), indexBuffer(indexBuffer), material(nullptr)
{}

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) 
    : vertexBuffer(vertices), indexBuffer(indices), material(nullptr)
{}

Mesh::Mesh(std::vector<Vertex> &vertices) 
    : vertexBuffer(vertices), indexBuffer(), material(nullptr)
{}

Mesh::~Mesh() {}

void Mesh::assignMaterial(Material* material) {
    this->material = material;
}

void Mesh::Bind() {
    this->vertexBuffer.Bind();
    this->indexBuffer.Bind();
    this->material->Bind();
}
