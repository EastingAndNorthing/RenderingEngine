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

    glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    GLuint base_vao;
    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);

    this->setupFramebuffer();

    glfwSetWindowSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
        Renderer &renderer = Renderer::Instance();
        renderer.windowWidth = width;
        renderer.windowHeight = height;
    });

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
        Renderer &renderer = Renderer::Instance();
        renderer.setupFramebuffer();
    });

}

void Renderer::setupFramebuffer() {
    glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
    glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);
    this->projectionMatrix = glm::perspective(glm::radians(g_settings.fov), (float) this->frameBufferWidth / (float) this->frameBufferHeight, 0.1f, 100.0f);
}

void Renderer::Enqueue(Mesh* mesh) {
    if(mesh->material != NULL) {
        
        this->renderQueue.push_back(mesh);
        
        printf("[RENDERER] Mesh enqueued.\n");
        
    } else {
        printf("[ERR] No material assigned to mesh, skipped.\n");
    }
}

void Renderer::Draw() {

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
        model *= mesh->rotation;
        
        glm::mat4 u_mvp = viewProjectionMatrix * model;
        
        glUniformMatrix4fv(mesh->material->shader->getProjectionMatrixLocation(), 1, GL_FALSE, glm::value_ptr(u_mvp));

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

Renderer::~Renderer() {
    
    for (auto& mesh: this->renderQueue) {
        delete mesh;
    }

    std::printf("[RENDERER] MOIIIIIII\n");
}