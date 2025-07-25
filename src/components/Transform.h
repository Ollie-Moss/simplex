#pragma once
#include "glm/glm.hpp"

struct Transform {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec2 size = glm::vec2(50.0f, 50.0f);
};
