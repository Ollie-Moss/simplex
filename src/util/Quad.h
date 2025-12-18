#pragma once

#include "glm/glm.hpp"

class Quad
{
  public:
    Quad();
    Quad(glm::vec2 min, glm::vec2 max);

    bool InBounds(glm::vec2 position);

    glm::vec2 min;
    glm::vec2 max;
};
