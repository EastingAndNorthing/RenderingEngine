#include <string>
#include "common.h"
#include "util/Filesystem.h"
#include "Renderer.h"

Renderer::Renderer() {
	Init();
}

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

/*! @brief Creates a native window and starts an OpenGL context using GLFW. 
 * Loads the OpenGL library functions using GLAD. 
 *
 * @param[in] timeout The maximum amount of time, in seconds, to wait.
 *
 */
void Renderer::Init() {
    glfwInit();

    #ifdef __APPLE__
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    window = glfwCreateWindow(g_settings.window_width, g_settings.window_height, g_settings.window_title, NULL, NULL);

    if (!window) {
        printf("[ERR] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("[ERR] Failed to initialize OpenGL context");
    }

    glViewport(0, 0, g_settings.window_width, g_settings.window_height);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    GLuint base_vao;
    glGenVertexArrays(1, &base_vao);
    glBindVertexArray(base_vao);
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}

