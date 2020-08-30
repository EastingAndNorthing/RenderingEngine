#include "renderer.h"

Renderer::Renderer() {
	init();
}

Renderer& Renderer::instance() {
    static Renderer instance;
    return instance;
}

void Renderer::init() {
    glfwInit();

#ifdef __APPLE__
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(g_settings.window_width, g_settings.window_height, g_settings.window_title, NULL, NULL);

    if (!window) {
        printf("[RENDERER] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("[RENDERER] Failed to initialize OpenGL context");
    }

    glViewport(0, 0, g_settings.window_width, g_settings.window_height);

    printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}

void Renderer::log(const char* msg) {
    printf("[RENDERER] %s\n", msg);
}
