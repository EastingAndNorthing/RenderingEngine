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
    
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	// glEnable(GL_STENCIL_TEST);
	// glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// glEnable(GL_CULL_FACE);
}

GLuint Renderer::CompileShader(const std::string& shaderSource, unsigned int type) {

    Filesystem &fs = Filesystem::Instance();
    std::string shader_str = fs.getFileContents(shaderSource);

    printf("[INFO] Compiling shader: '%s'...\n", shaderSource.c_str());

    GLuint shaderId = glCreateShader(type);
    const char* src = shader_str.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int compilation_result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilation_result);

    if(compilation_result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        printf("[ERR] Could not compile shader: '%s'. \nOpenGL: %s", shaderSource.c_str(), message);
        
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

    int program_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (program_linked == GL_FALSE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &log_length, message);
        printf("[ERR] Shader linking error: %s\n", message);
        return 0;
    }

    printf("[SUCCESS] Shader compiled.\n");

    return program;
}