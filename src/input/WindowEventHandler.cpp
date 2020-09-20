#include "common.h"
#include "input/WindowEventHandler.h"
#include "core/Renderer.h"

namespace WindowEventHandler {

    GLFWwindow* window;

    bool initialized = false;

    Mouse &mouse = Mouse::Instance();

    void init(GLFWwindow* window, bool initMouse, bool initKeyboard) {
        if(!initialized) {
            
            glfwSetWindowSizeCallback(window, WindowEventHandler::onWindowResize);
            glfwSetFramebufferSizeCallback(window, WindowEventHandler::onFrameBufferResize);

            if(initMouse) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetMouseButtonCallback(window, WindowEventHandler::onMouseButtonClick);
                // glfwSetCursorPosCallback(window, WindowEventHandler::onMouseMove);
            }

            initialized = true;
        }
    }

    void onFrame(GLFWwindow* window) {
        glfwPollEvents();
        mouse.update(window);
    }

    static void onWindowResize(GLFWwindow* window, int width, int height) {
        Renderer &renderer = Renderer::Instance();
        renderer.windowWidth = width;
        renderer.windowHeight = height;
    }

    static void onFrameBufferResize(GLFWwindow* window, int width, int height) {
        Renderer &renderer = Renderer::Instance();
        renderer.SetupFramebuffer(width, height);
    }

    // static void onMouseMove(GLFWwindow* window, double xpos, double ypos){
    //     Mouse &mouse = Mouse::Instance();
    //     mouse.update(window, xpos, ypos);
    // }

    static void onMouseButtonClick(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}
