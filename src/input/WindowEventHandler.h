#pragma once

#include <functional>
#include "common.h"
#include "input/Mouse.h"

class WindowEventHandler {
public:
    
	static WindowEventHandler& Instance();

	WindowEventHandler(const WindowEventHandler&) = delete;
    ~WindowEventHandler();

    void bindWindow(GLFWwindow* window);

    template<typename Function>
    void addMousePositionCallback(Function && fn) ;

    void _mousePosCallback(double xpos, double ypos);
    
    friend Mouse;
    friend void Mouse::update(); 

private:
    WindowEventHandler();

    GLFWwindow* window;

    std::vector<std::function<void()>> mousePosCallStack;

    WindowEventHandler(GLFWwindow* window);

};

namespace WindowEventHandlerFunctions {
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};
