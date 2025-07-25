#pragma once

#include "glm/glm.hpp"
#include <string>

struct SpriteRenderer {
    std::string texture = "";
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};
