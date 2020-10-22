#include "primitives/InstancedMesh.h"

InstancedMesh::InstancedMesh(const PrimitiveMesh& primitiveMesh, unsigned int instanceCount) 
    : Mesh(primitiveMesh), instanceCount(instanceCount) {

    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for(int y = -10; y < 10; y += 2)
    {
        for(int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    glGenBuffers(1, &this->instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2); // 2?
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Why unbind here?
    glVertexAttribDivisor(2, 1);  

}

void InstancedMesh::Bind() {
    this->vertexBuffer.Bind();
    this->indexBuffer.Bind();
    this->material->Bind();

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->instanceCount);

}

void InstancedMesh::Unbind() {
    this->vertexBuffer.Unbind();
    this->indexBuffer.Unbind();
}