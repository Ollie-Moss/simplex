#pragma once

#include "glm/glm.hpp"

struct RenderData
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec2 size = glm::vec2(0, 0);
    glm::vec4 color = glm::vec4(0, 0, 0, 0);
};
