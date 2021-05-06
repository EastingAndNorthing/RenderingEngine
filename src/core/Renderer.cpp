#include "core/Renderer.h"

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
        Log("GLFW window could not be created.", LogLevel::ERROR);
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->window);

    if(g_settings.vsync)
	    glfwSwapInterval(1);
    else 
	    glfwSwapInterval(0);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        Log("Failed to initialize OpenGL context.", LogLevel::ERROR);
        glfwTerminate();
    }

    this->camera = new Camera(this->window); // Requires the framebuffer to be set up.
    this->SetupFramebuffer(this->frameBufferWidth, this->frameBufferHeight);

    printf("Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("Supported GLSL version is %s.\n", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

    // // Breaks on MacOS?
    // glDebugMessageCallback(GlDebugMsg, nullptr);
    // glEnable(GL_DEBUG_OUTPUT);
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Wireframe ->> https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/
	// glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
    // glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    this->defaultShader = new Material("/shaders/Basic");
    
    this->debugVector = new Mesh(GeometryGenerator::Arrow());
    this->debugVector->setMaterial(defaultShader);
    this->overlayQueue.push_back(this->debugVector);

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
    } else {
        Log("No material assigned to mesh, skipped.", LogLevel::ERROR);
    }
}

void Renderer::BeginLoop() {

    WindowEventHandler::onFrame(this->window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    this->camera->update();

    this->DrawMeshes();
}

void Renderer::EndLoop() {
    glfwSwapBuffers(this->window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->DrawOverlays();
}

void Renderer::DrawMeshes() {
    if(g_settings.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

    for (auto& mesh : this->meshQueue) {

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
        }

        // @TODO add support for instanced meshes using glDrawArraysInstanced and glDrawElementsInstanced
    }
}

void Renderer::DrawOverlays() {

    // How about making a list of layers where geometry can be enqueued to?
    // Then just clear depth buffer foreach layer

    glClear(GL_DEPTH_BUFFER_BIT); // Always on top
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
    glDisable(GL_CULL_FACE);

    for (auto& mesh : this->overlayQueue) {
        mesh->Bind();
        glUniformMatrix4fv(mesh->material->shader->getUniformLocation("u_modelViewProjectionMatrix"), 1, GL_FALSE, 
            glm::value_ptr(this->camera->viewProjectionMatrix * mesh->getWorldPositionMatrix())
        );
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertexBuffer.getCount());
    }

    // for (const auto &nameMeshPair : this->debugOverlays) {
    //     nameMeshPair.second->Bind();
    //     glDrawArrays(GL_TRIANGLES, 0, nameMeshPair.second->vertexBuffer.getCount());
    // }
}

void Renderer::addDebugMesh(Mesh* mesh, const std::string& name) {
    if (debugOverlays.find(name) == debugOverlays.end()) {
        debugOverlays[name] = mesh;
    }
}

Mesh* Renderer::getDebugMesh(const std::string& name) {
    return this->debugOverlays.at(name);
}

void Renderer::Clear() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Camera* Renderer::getCamera() {
    return this->camera;
}

Renderer::~Renderer() {
    for (auto& mesh: this->meshQueue) {
        delete mesh;
    }

    glfwTerminate();

    Log("[RENDERER] MOIIIIIII\n", LogLevel::DEBUG);
}

static void GlDebugMsg(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log(message, LogLevel::ERROR);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            // Log(message, LogLevel::DEBUG);
            break;
    }
}
