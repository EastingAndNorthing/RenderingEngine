#include "CoordinateSystem.h"

glm::vec3 CoordinateSystem::localToWorld(const glm::vec3& localVector, const glm::quat& rotation, const glm::vec3& translation) {
    return (rotation * localVector) + translation;
}

// glm::vec3 CoordinateSystem::localToWorld(const glm::vec3& localVector, const glm::vec3& translation) {
//     return localVector + translation;
// }

glm::vec3 CoordinateSystem::worldToLocal(const glm::vec3& worldVector, const glm::quat& inverseRotation, const glm::vec3& translation) {
    return inverseRotation * (worldVector - translation);
}

// glm::vec3 CoordinateSystem::worldToLocal(const glm::vec3& worldVector, const glm::vec3& translation) {
//     return worldVector - translation;
// }
