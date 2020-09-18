#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLAD_DEBUG

#include "general/Settings.h"
// #include "general/log.h"

// Pre-define important classes, prevents declaration errors
class Renderer;
class Mesh;
