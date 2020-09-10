#pragma once

#include <vector>
#include "../common.h"
#include "core/Vertex.h"
#include "core/Material.h"
#include "primitives/Mesh.h"

class BatchBuffer {
public:
    GLuint vbuffer;
    GLuint vao;
    GLuint ibo;
    
    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;

    Material* material;

    BatchBuffer();
    ~BatchBuffer();

    void updateBuffer();
    void addGeometry(Mesh* mesh);
    void addGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void setMaterial(Material* material);
    void Bind();
    void UnBind();

private:
    GLuint dirtycontainer;
    unsigned int indicesCount;
    unsigned int vertexCount;
};
