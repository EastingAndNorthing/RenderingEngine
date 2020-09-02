#include "common.h"
#include "util/Filesystem.h"
#include "core/Renderer.h"
#include "core/Vec3.h"
#include "core/Vertex.h"
#include "core/VertexBuffer.h"

Settings g_settings;

int main(int argc, char **argv) {

    Renderer &renderer = Renderer::Instance();

    Vertex v1 = { Vec3(-0.5f, -0.5f, 0.2f) };
    Vertex v2 = { Vec3( 0.0f,  0.5f, 0.5f) };
    Vertex v3 = { Vec3( 0.5f, -0.5f, 0.8f) };

    Vertex vertices[3] {
        v1, v2, v3
    };

    // // Build a new buffer to hold vertices
    // GLuint vertexArrayObj;
    // glGenVertexArrays(1, &vertexArrayObj);
    // glBindVertexArray(vertexArrayObj);
    // glGenBuffers(1, &vertexArrayObj);
    // glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObj);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    VertexBuffer* vbuffer = new VertexBuffer(vertices, sizeof(vertices), GL_STATIC_DRAW);

    GLuint shader = renderer.CompileShaders("shaders/Basic.vert", "shaders/Basic.frag");
    GLint vertexPosAtrib = glGetAttribLocation(shader, "position");
    glVertexAttribPointer(vertexPosAtrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // offsetof(Vertex, attribute)
    glEnableVertexAttribArray(vertexPosAtrib);
    glUseProgram(shader);
    
    while (!glfwWindowShouldClose(renderer.window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(renderer.window);
    }

    delete vbuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwTerminate();
    return 0;
}