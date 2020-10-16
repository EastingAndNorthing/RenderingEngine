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

#define GLM_FORCE_SIMD_AVX2
#define GLM_FORCE_SSE2
#define GLM_FORCE_ALIGNED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "general/Settings.h"

#define LOG_LEVEL DEBUG
#include "general/Log.h"

// Pre-define important classes, prevents declaration errors
class Mouse;
class Time;
class Renderer;
class RigidBody;
