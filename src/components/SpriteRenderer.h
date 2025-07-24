#pragma once

#include "glm/glm.hpp"
#include <string>

struct SpriteRenderer {
    std::string texture;
    glm::vec2 size;
    glm::vec4 color;
};
