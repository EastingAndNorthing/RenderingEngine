#pragma once

#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Shader.h"
#include "core/Material.h"

class Mesh {
public: 

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    Material* material = NULL;

    glm::vec3 position;
    glm::vec4 rotation;
    glm::vec3 scale;

    Mesh();

    Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer);
    
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

    Mesh(std::vector<Vertex> &vertices);
    
    Mesh(std::vector<Vertex> vertices);

    ~Mesh();

    void assignMaterial(Material* material);

    void Bind();

private:

};
