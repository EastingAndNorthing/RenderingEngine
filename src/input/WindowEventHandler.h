#pragma once

#include <functional>
#include "common.h"
#include "input/Mouse.h"

namespace WindowEventHandler {

    void init(GLFWwindow* window, bool initMouse = false, bool initKeyboard = false);

    void onFrame(GLFWwindow* window);

    static void onWindowResize(GLFWwindow* window, int width, int height);
    static void onFrameBufferResize(GLFWwindow* window, int width, int height);
    
    // static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
    static void onMouseButtonClick(GLFWwindow* window, int button, int action, int mods);
    
};
