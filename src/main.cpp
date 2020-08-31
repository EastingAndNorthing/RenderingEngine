#include "common.h"
#include "core/renderer.h"
#include <string>

Settings g_settings;

int main(void)
{

    Renderer &renderer = Renderer::Instance();
    
    // Vertex positions, 2 floats per vertex
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f, 
         0.5f, -0.5f
    };

    // Build a new buffer to hold vertices
    GLuint vertexArrayObj;
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);
    glGenBuffers(1, &vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    std::string vs = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main() { \n"
            "gl_Position = position;\n"
        "}\n";

    std::string fs = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main() { \n"
            "color = vec4(1.0, 1.0, 0.0, 1.0);\n"
        "}\n";

    GLuint shader = renderer.CompileShaders(vs, fs);

    GLint vertexPosAtrib = glGetAttribLocation(shader, "position");
    // Define that we are using 2d vertex positions,
    // GLuint index, Abstract into Vertex struct and use offsetof(Vertex, attribute)
    // GLint size, 
    // GLenum type, 
    // GLboolean normalized: convert values to be within [0, 1], could be used for color processing
    // GLsizei stride: 0, assume tightly packed (position only, no other vertex data like colors).
    // const void *pointer: Abstract into Vertex struct and use offsetof(Vertex, attribute)
    glVertexAttribPointer(vertexPosAtrib, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), 0); 
    glEnableVertexAttribArray(vertexPosAtrib);

    glUseProgram(shader);
    
    while (!glfwWindowShouldClose(renderer.window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 3, GL_FLOAT, NULL);

        // Swap the screen buffers
        glfwSwapBuffers(renderer.window);
    }

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    glfwTerminate();
    return 0;
}