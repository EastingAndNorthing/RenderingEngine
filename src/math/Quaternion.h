#pragma once

#include "common.h"

namespace Quaternion {
    glm::quat createFromTwoVectors(glm::vec3 vFrom, glm::vec3 vTo);
}
