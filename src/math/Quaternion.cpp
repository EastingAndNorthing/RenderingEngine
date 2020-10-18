#include "Quaternion.h"

glm::quat Quaternion::createFromUnitVectors(glm::vec3 vFrom, glm::vec3 vTo) {
    // https://github.com/Mugen87/three.js/blob/master/src/math/Quaternion.js

    vFrom = glm::normalize(vFrom);
    vTo = glm::normalize(vTo);
    
    float r;
    float EPS = 0.000001;
    glm::vec3 v1 = glm::vec3();

    r = glm::dot(vFrom, vTo ) + 1.0f;

    if ( r < EPS ) {
        r = 0;

        if ( abs( vFrom.x ) > abs( vFrom.z ) ) {

            v1 = { -vFrom.y, vFrom.x, 0 };

        } else {

            v1 = { 0, - vFrom.z, vFrom.y };

        }

    } else {

        v1 = glm::cross( vFrom, vTo );

    }

    glm::quat result;

    result.x = v1.x;
    result.y = v1.y;
    result.z = v1.z;
    result.w = r;

    return glm::normalize(result);    
}
