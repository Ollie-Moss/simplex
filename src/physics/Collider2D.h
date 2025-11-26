#pragma once

#include "glm/glm.hpp"

enum class Shape2D {
    Circle,
    Box
};

struct Collider2D
{
    Shape2D shape = Shape2D::Box;
    glm::vec2 offset = glm::vec2(0.0f, 0.0f);

    // Box
    glm::vec2 halfExtents = glm::vec2(0.5f, 0.5f);

    // Circle
    float radius = 0.5f;
};
