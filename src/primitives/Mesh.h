#pragma once

#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"

class Mesh {
public: 

    Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer);
    
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

    Mesh(std::vector<Vertex> &vertices);

    ~Mesh();

    VertexBuffer vertexBuffer; // Friends with Renderer?
    
    IndexBuffer indexBuffer;

    Shader shader;

    void assignShader(const Shader &shader);

    void Bind();

private:

};
