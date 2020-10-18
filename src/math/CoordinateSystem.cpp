#include "CoordinateSystem.h"

glm::vec3 CoordinateSystem::localToWorld(const glm::vec3& localVector, const glm::quat& localRotation, const glm::vec3& localTranslation) {
    return (localRotation * localVector) + localTranslation;
}

glm::vec3 CoordinateSystem::worldToLocal(const glm::vec3& worldVector, const glm::quat& invertedRotation, const glm::vec3& worldTranslation) {
    return (invertedRotation * worldVector) - worldTranslation;
}
