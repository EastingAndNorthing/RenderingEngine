#pragma once

#include "../common.h"
#include "core/Vertex.h"
#include "core/Material.h"
#include "primitives/Mesh.h"

class BatchBuffer {
public:
    GLuint vao;
    GLuint vbuffer;
    GLuint ibuffer;
    
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;

    Material* material;

    BatchBuffer();
    ~BatchBuffer();

    void addGeometry(Mesh* mesh);
    void addGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void setMaterial(Material* material);
    void Bind();
    void UnBind();

private:
    void generateBuffers();
    unsigned int verticesCount = 0;
    unsigned int verticesSize = 0;
    unsigned int indicesCount = 0;
    unsigned int indicesSize = 0;
    unsigned int size = 1024;
};
