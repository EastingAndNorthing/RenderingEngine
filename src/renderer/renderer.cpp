#include "renderer.h"

Renderer::Renderer()
{
	init();
}

GLFWwindow* Renderer::get_window() const
{
	return window;
}

void Renderer::init()
{
    glfwInit();

    window = glfwCreateWindow(settings.window_width, settings.window_height, settings.window_title, NULL, NULL);

    if (!window) {
        printf("GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to initialize OpenGL context");
    }

    glViewport(0, 0, settings.window_width, settings.window_height);

    std::printf("Renderer set up. OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}

Renderer::~Renderer() { }