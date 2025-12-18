#include "Quad.h"

Quad::Quad() {}

Quad::Quad(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}

bool Quad::InBounds(glm::vec2 position)
{
    return (position.x >= min.x && position.y >= min.y) &&
           (position.x <= max.x && position.y <= max.y);
}
