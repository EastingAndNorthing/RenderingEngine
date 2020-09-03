#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/Shader.h"

Settings g_settings;

int main(int argc, char **argv) {

    Renderer &renderer = Renderer::Instance();

    Vertex v1 = {
        Vec3(-0.5f, -0.5f, 0.2f),
        Vec3(1.0f, 0.0f, 0.0f),
    };

    Vertex v2 = {
        Vec3( 0.0f,  0.5f, 0.5f),
        Vec3(0.0f, 1.0f, 0.0f),
    };

    Vertex v3 = {
        Vec3( 0.5f, -0.5f, 0.8f),
        Vec3(0.0f, 0.0f, 1.0f),
    };

    Vertex vertices[3] {
        v1, v2, v3
    };

    VertexBuffer* myTriangle = new VertexBuffer(vertices, sizeof(vertices), GL_STATIC_DRAW);

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
        basicShader->Bind();
        myTriangle->Bind();
        
        // indexBuffer->Bind();
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // @TODO: IndexBuffer

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ///////////////////////////////////////////////////////////////////////
        glfwSwapBuffers(renderer.window);
    }

    delete myTriangle;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glfwTerminate();
    return 0;
}