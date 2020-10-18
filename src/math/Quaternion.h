#pragma once

#include "common.h"

namespace Quaternion {
    glm::quat createFromUnitVectors(glm::vec3 vFrom, glm::vec3 vTo);
}
