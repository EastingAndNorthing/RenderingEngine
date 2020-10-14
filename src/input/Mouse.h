#pragma once

#include "common.h"

class Mouse {
public:

    glm::vec2 pos = glm::vec2(0.0f);
    glm::vec2 absolutePos = glm::vec2(0.0f);
    glm::vec2 scrollpos = glm::vec2(0.0f);

    float sensitivity = 75.0f;

	static Mouse& Instance();

	Mouse(const Mouse&) = delete;
    ~Mouse() = default;

    glm::vec2 getDelta();

    void update(GLFWwindow* window, double& dt);

private:

    Mouse() = default;

    int windowWidth;
    int windowHeight;

    glm::vec2 lastPos = { 0, 0 };
    glm::vec2 deltaPosOffset = { 0, 0 };

    double deltaTime = 0;

};
