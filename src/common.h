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

// @TODO look into SSE2 support flags for performance
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "general/Settings.h"

#define LOG_LEVEL DEBUG
#include "general/Log.h"

// Pre-define important classes, prevents declaration errors
class Mouse;
class Time;
class Renderer;
class RigidBody;
