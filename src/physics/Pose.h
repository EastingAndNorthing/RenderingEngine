#pragma once

#include "../common.h"

class Pose {
public:

    glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat q = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);		

    Pose() = default; 
    Pose(glm::vec3 p, glm::quat q); 

    void copy(Pose pose);

    Pose clone();
    
    void translate(glm::vec3& v);
    void invTranslate(glm::vec3& v);

    void rotate(glm::vec3& v);
    void invRotate(glm::vec3& v);

    void transform(glm::vec3& v);
    void invTransform(glm::vec3& v);

    void transformPose(Pose& pose);  

};
