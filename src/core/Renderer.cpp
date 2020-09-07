#include <unordered_map>
#include "common.h"
#include "core/Renderer.h"
#include "util/Filesystem.h"
#include "core/Shader.h"
#include "primitives/Mesh.h"
#include "core/Uniforms.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer() {
	Init();
}

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

Renderer::~Renderer() {
    
    for (auto& mesh: this->renderQueue) {
        delete mesh;
    }

    std::printf("[RENDERER] MOIIIIIII\n");
}

//! @brief Creates and opens a native window and starts an OpenGL context using GLFW. Loads the OpenGL library functions using GLAD. 
void Renderer::Init() {
    glfwInit();

    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disables border / close button etc. 
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    // glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

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

    // int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
    
    GLuint base_vao;
    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);

    // Set base projection. @TODO reset this projection matrix when resizing the window
    this->projectionMatrix = glm::perspective(glm::radians(g_settings.fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.1f, 100.0f);
    // this->projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

}

void Renderer::Enqueue(Mesh* mesh) {
    if(mesh->material != NULL) {
        
        this->renderQueue.push_back(mesh);
        
        printf("[RENDERER] Mesh enqueued.\n");
        
    } else {
        printf("[ERR] No material assigned to mesh, skipped.\n");
    }
}

// void Renderer::Enqueue(std::unique_ptr<Mesh> &mesh) {
//     if(mesh->material != NULL) {
//         this->renderQueue.push_back(std::move(mesh));
//         // printf("[RENDERER] Mesh enqueued. Renderer took ownership of object.\n");
//     } else {
//         printf("[ERR] No material assigned to mesh, skipped enqueue.\n");
//     }
// }

void Renderer::Draw() {

    // glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    // glm::mat4 ViewTranslate = glm::translate(
    //     glm::mat4(1.0f),
    //     glm::vec3(0.0f, 0.0f, -Translate)
    // );
    // glm::mat4 ViewRotateX = glm::rotate(
    //     ViewTranslate,
    //     Rotate.y,
    //     glm::vec3(-1.0f, 0.0f, 0.0f)
    // );
    // glm::mat4 View = glm::rotate(
    //     ViewRotateX,
    //     Rotate.x,
    //     glm::vec3(0.0f, 1.0f, 0.0f)
    // );
    // glm::mat4 Model = glm::scale(
    //     glm::mat4(1.0f),
    //     glm::vec3(0.5f)
    // );
    // glm::mat4 MVP = Projection * View * Model;
    // glUniformMatrix4fv(LocationMVP, 1, GL_FALSE, glm::value_ptr(MVP));

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera
    const float camRadius = 1.5f;
    float camX = sin(glfwGetTime()) * camRadius;
    float camZ = cos(glfwGetTime()) * camRadius;
    glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 viewProjectionMatrix = this->projectionMatrix * view;

    for (auto& mesh: this->renderQueue) {

        mesh->Bind();

        glm::mat4 model = glm::translate(glm::mat4(1), mesh->position);
        model = model * mesh->rotation; // ?
        
        glm::mat4 mvp = viewProjectionMatrix * model; // projection * view * model * vec4(position, 1.0);
        
        glUniformMatrix4fv(mesh->material->shader->getProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(mvp));

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