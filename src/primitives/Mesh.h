#pragma once

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

    Mesh();
    Mesh(const VertexBuffer &vertexBuffer, const IndexBuffer &indexBuffer);
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
    Mesh(std::vector<Vertex> &vertices);
    Mesh(std::vector<Vertex> vertices);
    ~Mesh();

    void assignMaterial(Material &material);
    void Bind();
    
    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 direction);

    glm::vec3 getPosition();
    glm::mat4 getRotation();
    glm::vec3 getScale();

    glm::mat4 getWorldPositionMatrix();

private:
    bool worldPosMatrixNeedsUpdate = true;

    glm::mat4 _worldPositionMatrix = glm::mat4(1.0f);
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 _rotation = glm::mat4(1.0f);
    glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
