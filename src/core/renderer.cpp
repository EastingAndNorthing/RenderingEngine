#include <fstream>
#include <sstream>
#include <string>
#include "Renderer.h"
#include "../common.h"

Renderer::Renderer() {
	Init();
}

Renderer& Renderer::Instance() {
    static Renderer instance;
    return instance;
}

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
        printf("[ERR] [RENDERER] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("[ERR] [RENDERER] Failed to initialize OpenGL context");
    }

    glViewport(0, 0, g_settings.window_width, g_settings.window_height);

    printf("[INFO] [RENDERER] Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("[INFO] [RENDERER] Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}

GLuint Renderer::CompileShader(const std::string& shaderSource, unsigned int type) {

    printf("[INFO] [RENDERER] Compiling shader: '%s'...\n", shaderSource.c_str());

    // stream.open("main.app.dSYM/Contents/...");
    std::ifstream stream(shaderSource);
    std::string shader_str((std::istreambuf_iterator<char>(stream)),
                          std::istreambuf_iterator<char>());

    GLuint shaderId = glCreateShader(type);
    const char* src = shader_str.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        printf("[ERR] [RENDERER] Could not compile shader: '%s'. \nOpenGL: %s", shaderSource.c_str(), message);
        
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

GLuint Renderer::CompileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    
    GLuint vert = CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint frag = CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    if(vert == 0 || frag == 0) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    printf("[INFO] [RENDERER] Shader compiled.\n");

    return program;
}