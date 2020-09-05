#include <vector>
#include "common.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vec4.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"
#include "core/IndexBuffer.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

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

    std::vector<Vertex> someQuad {
        v1, v2, v3, v4
    };

    std::vector<unsigned int> someQuadIndices = {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<Vertex> someTriangle {
        Vertex(-1,-1,0), Vertex(1,-1,1), Vertex(1,1,0),
    };

    Mesh* myQuad = new Mesh(someQuad, someQuadIndices);
    Mesh* myTriangle = new Mesh(someTriangle);

    Shader* basicShader = new Shader("shaders/Basic.vert", "shaders/Basic.frag");
    
    myQuad->assignShader(*basicShader);
    myTriangle->assignShader(*basicShader);

    renderer.Enqueue(*myQuad);
    renderer.Enqueue(*myTriangle);

    renderer.Clear();

    while (!glfwWindowShouldClose(renderer.window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.Draw();

        glfwSwapBuffers(renderer.window);
    }

    // delete mesh; // Clean up heap
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glfwTerminate();
    return 0;
}