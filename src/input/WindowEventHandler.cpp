#include "common.h"
#include "WindowEventHandler.h"

WindowEventHandler& WindowEventHandler::Instance(){
    static WindowEventHandler instance;
    return instance;
}

WindowEventHandler::WindowEventHandler() {}
WindowEventHandler::~WindowEventHandler() {}

void WindowEventHandler::bindWindow(GLFWwindow* window) {
    this->window = window;
    glfwSetWindowUserPointer(this->window, reinterpret_cast<void*>(this));
    glfwSetCursorPosCallback(this->window, WindowEventHandlerFunctions::mouse_callback);
}

// https://stackoverflow.com/questions/15128444/c-calling-a-function-from-a-vector-of-function-pointers-inside-a-class-where-t
template<typename Function>
void WindowEventHandler::addMousePositionCallback(Function && fn) {
    mousePosCallStack.push_back(std::forward<Function>(fn)); 
}

void WindowEventHandler::_mousePosCallback(double xpos, double ypos) {
    for(auto && fn : mousePosCallStack)
        fn();
}

namespace WindowEventHandlerFunctions {
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos){
        WindowEventHandler* windowEventHandler = reinterpret_cast<WindowEventHandler *>(glfwGetWindowUserPointer(window));
        windowEventHandler->_mousePosCallback(xpos, ypos);
    }
}
