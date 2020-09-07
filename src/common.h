#pragma once
#include <cassert>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLAD_DEBUG

// General
#include "general/Settings.h"
// #include "general/log.h"

// Pre-define important classes, prevents declaration errors
class Renderer;
class Mesh;
