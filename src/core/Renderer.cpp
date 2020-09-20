#include <unordered_map>
#include "common.h"
#include "core/Renderer.h"
#include "util/Filesystem.h"
#include "core/Shader.h"
#include "core/Camera.h"
#include "primitives/Mesh.h"
#include "core/Uniforms.h"
#include "input/WindowEventHandler.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

Renderer::Renderer() {

    this->windowWidth = g_settings.window_width;
    this->windowHeight = g_settings.window_height;

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

    this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, g_settings.window_title, NULL, NULL);

    if (!this->window) {
        printf("[ERR] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("[ERR] Failed to initialize OpenGL context");
    }

    this->camera = new Camera(this->window); // Requires the framebuffer to be set up.

    this->SetupFramebuffer(this->frameBufferWidth, this->frameBufferHeight);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    WindowEventHandler::init(this->window, true);
    WindowEventHandler::onFrame(this->window);

}

bool Renderer::isActive(){
    return !glfwWindowShouldClose(this->window);
}

void Renderer::SetupFramebuffer(int width, int height) {

    this->frameBufferWidth = width;
    this->frameBufferHeight = height;

    if(width ==0 || height == 0) {
        glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
    }
    
    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);

    this->camera->setProjection(this->frameBufferWidth, this->frameBufferHeight);
}

void Renderer::Enqueue(Mesh* mesh) {
    if(mesh->material != NULL) {
        this->meshQueue.push_back(mesh);
        printf("[RENDERER] Mesh enqueued.\n");
    } else {
        printf("[ERR] No material assigned to mesh, skipped.\n");
    }
}

void Renderer::BeginLoop() {

    WindowEventHandler::onFrame(this->window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->camera->update();

    this->DrawMeshes();
}

void Renderer::EndLoop() {
    glfwSwapBuffers(this->window);
}

void Renderer::DrawMeshes() {
    for (auto& mesh: this->meshQueue) {

        mesh->Bind();
        
        glUniformMatrix4fv(mesh->material->shader->getUniformLocation("u_modelViewMatrix"), 1, GL_FALSE, 
            glm::value_ptr(this->camera->viewMatrix * mesh->getWorldPositionMatrix())
        );
        glUniformMatrix4fv(mesh->material->shader->getUniformLocation("u_modelViewProjectionMatrix"), 1, GL_FALSE, 
            glm::value_ptr(this->camera->viewProjectionMatrix * mesh->getWorldPositionMatrix())
        );

        if(mesh->indexBuffer.getCount() > 0) {
            glDrawElements(GL_TRIANGLES, mesh->indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, mesh->vertexBuffer.getCount());
            // glDrawArrays(GL_POINTS, 0, mesh->vertexBuffer.getCount());
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

Camera* Renderer::getCamera() {
    return this->camera;
}

Renderer::~Renderer() {
    for (auto& mesh: this->meshQueue) {
        delete mesh;
    }

    glfwTerminate();

    std::printf("[RENDERER] MOIIIIIII\n");
}