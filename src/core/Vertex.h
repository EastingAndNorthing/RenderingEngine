#pragma once

#include "common.h"

struct Vertex {
    
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal   = glm::vec3(0.0f);
    glm::vec4 color    = glm::vec4(0.0f);

    Vertex();
    Vertex(glm::vec3 position = glm::vec3(0.0f), glm::vec3 normal = glm::vec3(0.0f), glm::vec4 color = glm::vec4(0.0f));
    Vertex(float x = 0, float y = 0, float z = 0);
	~Vertex() = default;

    static void setAttribPointers();

};