#include "renderer.h"

Renderer::Renderer() {
	init();
}

Renderer& Renderer::instance() {
    static Renderer instance;
    return instance;
}

// GLFWwindow* Renderer::get_window() const {
// 	return window;
// }

void Renderer::log() {
    printf("[RENDERER] JAAAAA");
}

void Renderer::init() {
    glfwInit();

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

    std::printf("[RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}
