#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Shader.h"

Settings g_settings;

int main(int argc, char **argv) {

    Renderer &renderer = Renderer::Instance();

    Vertex v1 = {
        Vec3(-0.5f, -0.5f, 0.0f),
        Vec4(1.0f, 0.0f, 0.0f, 0.0f),
    };

    Vertex v2 = {
        Vec3(0.5f, -0.5f, 0.0f),
        Vec4(0.0f, 1.0f, 0.0f, 0.0f),
    };

    Vertex v3 = {
        Vec3(0.5f,  0.5f, 0.0f),
        Vec4(0.0f, 0.0f, 1.0f, 0.0f),
    };

    Vertex v4 = {
        Vec3(-0.5f,  0.5f, 0.0f),
        Vec4(1.0f, 1.0f, 0.0f, 0.0f),
    };

    std::vector<Vertex> vertices {
        v1, v2, v3, v4
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };

    VertexBuffer* myQuad = new VertexBuffer(vertices, GL_STATIC_DRAW);
    IndexBuffer* myQuadIBO = new IndexBuffer(indices, GL_STATIC_DRAW);

    Shader* basicShader = new Shader("shaders/Basic.vert", "shaders/Basic.frag");

    GLint vertexPosAtrib = glGetAttribLocation(basicShader->getID(), "position"); // Note that this is based on the currently active vertex array object (VertexBuffer)
    glVertexAttribPointer(vertexPosAtrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Look into offsetof(Vertex, attribute)
    glEnableVertexAttribArray(vertexPosAtrib);

    GLint vertexColAtrib = glGetAttribLocation(basicShader->getID(), "color"); // Note that this is based on the currently active vertex array object (VertexBuffer)
    glVertexAttribPointer(vertexColAtrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    glEnableVertexAttribArray(vertexColAtrib);
    
    // Reset loaded data for now
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // RENDERING LOOP /////////////////////////////////////////////////////

        myQuadIBO->Bind();
        basicShader->Bind();
        myQuad->Bind();
        
        glDrawArrays(GL_TRIANGLES, 0, 3); // Without ibo
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_INT,   // type
            nullptr            // offset in buffer to start drawing
        );
        ///////////////////////////////////////////////////////////////////////
        glfwSwapBuffers(renderer.window);
    }

    // delete myTriangle;
    delete myQuad;
    delete myQuadIBO;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glfwTerminate();
    return 0;
}