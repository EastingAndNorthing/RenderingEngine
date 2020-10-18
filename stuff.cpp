#include <iostream>
#include <cmath>
#include "common.h"

const double pi = std::atan(1.0)*4.0;

glm::quat angular_velocity_to_quaternion(glm::quat const &q, double delta_t) {
    float length = glm::length(q);
    float angle = length * delta_t; 
    glm::quat axis = q/length;    
    return std::cos(angle) + std::sin(angle) * axis;
}

int main() {  
    glm::quat angular_velocity(0.0, 2*pi, 0.0, 0.0); // One full turn around x in one second  
    double const delta_t = 1.0/30; // 30 fps  
    glm::quat rotation_each_step = angular_velocity_to_quaternion(angular_velocity,delta_t);
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
    for (int i=0; i<=30; ++i) {
        std::cout << orientation << '\n';   
        orientation *= rotation_each_step; 
    }
}
