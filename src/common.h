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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "general/Settings.h"

#define LOG_LEVEL NOTICE
#include "general/Log.h"

// Pre-define important classes, prevents declaration errors
class Renderer;
class Mesh;
