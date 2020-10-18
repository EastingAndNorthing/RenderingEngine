#pragma once

#include "common.h"

namespace CoordinateSystem {

    glm::vec3 localToWorld(const glm::vec3& localVector, const glm::quat& localRotation = {}, const glm::vec3& localTranslation = {});
    glm::vec3 worldToLocal(const glm::vec3& worldVector, const glm::quat& invertedRotation = {}, const glm::vec3& worldTranslation = {});

}