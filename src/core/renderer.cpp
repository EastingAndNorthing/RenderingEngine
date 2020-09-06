#include <unordered_map>
#include "common.h"
#include "core/Renderer.h"
#include "util/Filesystem.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"

Renderer::Renderer() {
	Init();
}

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

Renderer::~Renderer() {
    std::printf("[RENDERER] MOIIIIIII\n");
}

//! @brief Creates and opens a native window and starts an OpenGL context using GLFW. Loads the OpenGL library functions using GLAD. 
void Renderer::Init() {
    glfwInit();

    #ifdef __APPLE__
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    this->window = glfwCreateWindow(g_settings.window_width, g_settings.window_height, g_settings.window_title, NULL, NULL);

    if (!this->window) {
        printf("[ERR] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("[ERR] Failed to initialize OpenGL context");
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(this->window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
    
    GLuint base_vao;
    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);
}

// void Renderer::Enqueue(std::unique_ptr<Mesh> &mesh) {
//     if(mesh->material != NULL) {
//         this->renderQueue.push_back(std::move(mesh));
//         // printf("[RENDERER] Mesh enqueued. Renderer took ownership of object.\n");
//     } else {
//         printf("[ERR] No material assigned to mesh, skipped enqueue.\n");
//     }
// }

void Renderer::Enqueue(Mesh* mesh) {
    if(mesh->material != NULL) {
        this->renderQueue.push_back(mesh);
        printf("[RENDERER] Mesh enqueued.\n");
    } else {
        printf("[ERR] No material assigned to mesh, skipped enqueue.\n");
    }
}


void Renderer::Draw() {
    
    for (auto& mesh: this->renderQueue) {

        mesh->Bind();

        // Draw using an index buffer if available. Otherwise, simply draw all vertices.
        if(mesh->indexBuffer.getCount() > 0) {

            glDrawElements(GL_TRIANGLES, mesh->indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);

        } else {

            glDrawArrays(GL_TRIANGLES, 0, mesh->vertexBuffer.getSize());

        }

    }
}

void Renderer::Clear() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}