#include "common.h"
#include "renderer/renderer.h"
#include <string>

Settings g_settings;

GLuint CompileShader(const std::string& source, unsigned int type) {
    GLuint shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        std::cout << message << std::endl;
        
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{

    Renderer &renderer = Renderer::instance();

    // Vertex positions, 2 floats per vertex
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.0f, 
         0.5f, -0.5f
    };

    // Build a new buffer to hold verticies
    GLuint buffer;
    glGenVertexArrays(1, &buffer);
    glBindVertexArray(buffer);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    // Define that we are using 2d vertex positions,
    // GLuint index, Abstract into Vertex struct and use offsetof(Vertex, attribute)
    // GLint size, 
    // GLenum type, 
    // GLboolean normalized: convert values to be within [0, 1], could be used for color processing
    // GLsizei stride: 0, assume tightly packed (position only, no other vertex data like colors).
    // const void *pointer: Abstract into Vertex struct and use offsetof(Vertex, attribute)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), 0); 
    glEnableVertexAttribArray(0);

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

    GLuint shader = CreateShader(vs, fs);

    glUseProgram(shader);
    
    while (!glfwWindowShouldClose(renderer.window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // // Clear the colorbuffer
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