#pragma once

#include "glm/glm.hpp"
#include <string>

struct Sprite {
    std::string texture;
    glm::vec3 postition;
    glm::vec2 size;
    glm::vec4 color;
};
