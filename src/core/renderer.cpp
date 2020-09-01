#include <fstream>
#include <string>
#include "renderer.h"
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
        printf("[RENDERER] GLFW window could not be created.");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
	glfwSwapInterval(g_settings.vsync);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
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

GLuint Renderer::CompileShader(const std::string& shaderSource, unsigned int type) {
    GLuint shaderId = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        printf("[ERR] [RENDERER] Could not compile shader: %s", message);
        
        glDeleteShader(shaderId);
        return 0;
    }

    return shaderId;
}

GLuint Renderer::CompileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	
    std::string vert;
    std::string frag;

    std::ifstream f_shader_file;
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::stringstream f_shader_stream;

	try
	{
		f_shader_file.open(vertexShaderPath);
		f_shader_stream << f_shader_file.rdbuf();
		f_shader_file.close();

        std::cout << "Ye boi." << std::endl;

        vert = f_shader_stream.str();
		// return f_shader_stream.str();
	}
	catch (std::ifstream::failure& e)
	{
        std::cout << "[RENDERER] Failed to load shader: " << vertexShaderPath << std::endl;
		throw e;
	}
    
    // std::ifstream stream(vertexShaderPath);

    // std::string line;
    // std::stringstream ss[2];
    // while (getline(stream, line)) {
    //     if(line.find("#version") != std::string::npos) {
    //         std::cout << "found it" << std::endl;
    //     }
    // }

    // std::ifstream ifs_v(vertexShaderPath);
    // std::string vert ( (std::istreambuf_iterator<char>(ifs_v) ), (std::istreambuf_iterator<char>()) );

    // std::cout << vert << std::endl;

    // std::ifstream ifs_f(fragmentShaderPath);
    // std::string frag ( (std::istreambuf_iterator<char>(ifs_f) ), (std::istreambuf_iterator<char>()) );

    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vert, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(frag, GL_FRAGMENT_SHADER);

    if(vs == 0 || fs == 0) 
        return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}